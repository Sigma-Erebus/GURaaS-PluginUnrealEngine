//@Library('CradleSharedLibrary') _ // Loaded implicitly

String Node = ''
String WorkingDir = ''
//Assign a node to run the pipeline
node('WindowsNode') {
    echo "Running on ${env.NODE_NAME} in ${env.WORKSPACE}"
    Node = env.NODE_NAME
    WorkingDir = env.WORKSPACE
}

// Config section

String gitHubRepo = "BredaUniversityResearch/GURaaS-PluginUnrealEngine"
String gitHubBranch = "main"
String discordFriendlyName = "GURaaS-Plugin-Build"

String nexusRepo = "UEGURaaS-Releases"

String projectName = "GURaaSDev"
String pluginName = "GURaaS"

String discordWebhook = 'MONITORING_DISCORD_WEBHOOK'

Boolean splitArchive = false

// End of Config section

Boolean cleanupBefore = false
Boolean cleanupAfter = false

failureReason = [:]
String tags = ""
String commit = ""
try {
    stage('Clone') {
        if (cleanupBefore) {
            node(Node) {
                dir(WorkingDir) {
                    script {
                        deleteDir()
                    }
                }
                cleanWs()
            }
        }
        node(Node) {
            git.sparseCheckout("https://github.com/${gitHubRepo}", "${gitHubBranch}", "GURaaSDev", 'CRADLE_WEBMASTER_CREDENTIALS')
            try {
                tags = git.fetchTags('CRADLE_WEBMASTER_CREDENTIALS')
            } catch (Exception e) {
                echo "Tags not found"
                commit = git.fetchCommitHash('CRADLE_WEBMASTER_CREDENTIALS')
            }
        }
    }

    node(Node) {
        script {
            stage('Build Plugin GURaaS') {
                try {
                build job: 'Library/WindowsUnrealPluginBuild',
                parameters: [
                    string(name: 'NODE', value: Node),
                    string(name: 'WORKING_DIR', value: WorkingDir),
                    string(name: 'PROJECTPATH', value: "%CD%\\${projectName}\\${projectName}.uproject"),
                    string(name: 'PLUGINPATH', value: "%CD%\\${projectName}\\Plugins\\${pluginName}\\${pluginName}.uplugin"),
                    string(name: 'OUTPUTDIR', value: "%CD%\\Output"),
                    //choice params
                    string(name: 'UNREAL_VERSIONS', value: '2'),
                    string(name: 'PLATFORM', value: 'Win64'),
                    string(name: 'FLAGS_TEMPLATE', value: 'Standard'),
                    //
                    string(name: 'DISCORD_WEBHOOK', value: discordWebhook)
                ]
                } catch (Exception e) {
                    catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                        error()
                    }
                    failureReason["GURaaS"] = readFile("BuildFailures-GURaaS.txt")
                }
            }
        }
    }

    stage('Deploy') {
        node(Node) {
            script {
                def pluginVersions = bat(label: "Collect folders built for Project", script: "@dir /b /ad Output\\UE_*", returnStdout: true).split("\r\n")
                for (String version : pluginVersions) {
                    def checkForHostProject = []
                    for (String project : bat(script: "@dir /b /ad Output\\${version}\\*", returnStdout: true).split("\r\n")) {
                        echo project
                        try {
                            checkForHostProject += bat(label: "Check for host project", script: "@dir /b /ad Output\\${version}\\${project}", returnStdout: true).split("\r\n")
                        } catch (Exception e) {
                            echo "Failed to check for host project due to:"
                            echo "${e}"
                        }
                        echo "${checkForHostProject}"
                        if (checkForHostProject.contains("HostProject")){
                            echo "HostProject found for ${version}, meaning the plugin was not built successfully"
                            continue
                        }
                        echo "HostProject not found for ${version}, meaning the plugin was built successfully"
                    }
                    String buildName = sanitizeinput.buildName("GURaaS-${version}", "${currentBuild.number}", commit, "zip")
                    if (splitArchive) {
                        zip.packSplitArchive("./Output/${version}", buildName, "1920M")
                        def fileParts = bat(label: "Collect file parts", script: "@dir /b /a-d Output\\${version}\\${buildName}*", returnStdout: true).split("\r\n")
                        for (String filePart : fileParts) {
                            nexus.upload("${nexusRepo}", filePart, "application/x-zip-compressed", "Windows", 'NEXUS_CREDENTIALS')
                        }
                        //nexus.upload("${nexusRepo}", buildName, "application/x-zip-compressed", "Windows", 'NEXUS_CREDENTIALS')
                    } else {
                        zip.pack("./Output/${version}", buildName)
                        nexus.upload("${nexusRepo}", buildName, "application/x-zip-compressed", "Windows", 'NEXUS_CREDENTIALS')
                    }
                }
            }
        }
    }
} catch (InterruptedException e) {
    catchError(buildResult: 'ABORTED', stageResult: 'ABORTED') {
        error()
    }
    throw (e)
} catch (Exception e) {
    catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
        error()
    }
    node(Node) {
        script {
            failureReason["MainBuild"] = "${e}"
            //discord.failed(discordWebhook, "PlanV-Plugin-Build", "${e}")
        }
    }
    throw (e)
} finally {
    try {
        stage('Report-Results') {
            node(Node) {
                script {
                    def pluginVersions = bat(label: "Collect folders built for Project", script: "@dir /b /ad Output\\UE_*", returnStdout: true).split("\r\n")
                    switch (currentBuild.result) {
                        case "SUCCESS":
                            String downloadLinks = ""
                            for (String version : pluginVersions) {
                                buildName = sanitizeinput.buildName("GURaaS-${version}", "${currentBuild.number}", commit, "zip")
                                downloadLinks += "[Download Windows ${version} Build from Nexus](https://nexus.cradle.buas.nl/#browse/browse:${nexusRepo}:Windows%%2F${buildName})\n"
                            }
                            discord.succeeded(discordWebhook, "${discordFriendlyName}", "${downloadLinks}")
                            break
                        case "UNSTABLE":
                            echo "Build was unstable"
                            break
                        case "FAILURE":
                            echo "Build failed"
                            break
                        case "ABORTED":
                            echo "Build was aborted"
                            break
                        default:
                            echo "Unknown result, assuming build was successful"
                            String downloadLinks = ""
                            for (String version : pluginVersions) {
                                buildName = sanitizeinput.buildName("GURaaS-${version}", "${currentBuild.number}", commit, "zip")
                                downloadLinks += "[Download Windows ${version} Build from Nexus](https://nexus.cradle.buas.nl/#browse/browse:${nexusRepo}:Windows%%2F${buildName})\n"
                            }
                            discord.succeeded(discordWebhook, "${discordFriendlyName}", "${downloadLinks}")
                            break
                    }
                }
            }
        }
    } catch (InterruptedException e) {
        catchError(buildResult: 'ABORTED', stageResult: 'ABORTED') {
            error()
        }
        throw (e)
    } catch (Exception e) {
        catchError(buildResult: currentBuild.currentResult, stageResult: 'FAILURE') {
            error()
        }
        throw (e)
    } finally {
        stage('Cleanup') {
            if (cleanupAfter) {
                try {
                    node(Node) {
                        dir(WorkingDir) {
                            script {
                                deleteDir()
                            }
                        }
                        cleanWs()
                    }
                } catch (Exception e) {
                    echo "Unexpected failure during cleanup, retrying once..."
                    node(Node) {
                        dir(WorkingDir) {
                            script {
                                deleteDir()
                            }
                        }
                        cleanWs()
                    }
                    throw (e)
                }
            }
        }
    }
}
