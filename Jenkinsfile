#! groovy

def COLOR_MAP = [
    'SUCCESS': 'good',
    'FAILURE': 'danger',
    'ABORTED': 'danger',
    'UNSTABLE': 'warning'
]
// settings
String buildName = 'GURaaS-Default-Name'
PluginsToBuild = [ 'GURaaS' ]
String UEInstallsRoot = "G:\\UE"
// 7zip settings
boolean useSFX = false
boolean needsPublishToGithub = false
String zipVolumeSize = '1800M'
String zipCompressionLevel = '5'
String zipCompressionMethod = 'LZMA2'
// local folders
String localBuildFolder = 'Plugin_Build'
String localPackageFolder = 'Plugin_Package'

String cURLexe = "C:\\Program Files\\Git\\mingw64\\bin\\curl.exe"

UEInstalls = []

def BuildPlugin(String root, String pluginName, String installName, String resultLocation) 
{
    dir(root + "\\" + installName + "\\Engine\\Binaries\\DotNET\\AutomationTool")
    {

        bat( "IF not exist \"${WORKSPACE}\\${resultLocation}\\${installName}\\${pluginName}\" mkdir \"${WORKSPACE}\\${resultLocation}\\${installName}\\${pluginName}\"" )

        // print paths for debugging
        bat( "echo Plugin: ${WORKSPACE}\\GURaaSDev\\Plugins\\${pluginName}\\${pluginName}.uplugin")

        bat( "AutomationTool.exe BuildPlugin -Plugin=\"${WORKSPACE}\\GURaaSDev\\Plugins\\${pluginName}\\${pluginName}.uplugin\" -Package=\"${WORKSPACE}\\${resultLocation}\\${installName}\\${pluginName}\" -TargetPlatforms=Win64" )
    }
}

def SwitchEngineVersion(String root, String installName) 
{
    dir("C:\\Program Files (x86)\\Epic Games\\Launcher\\Engine\\Binaries\\Win64")
    {
        bat( "UnrealVersionSelector.exe -switchversionsilent \"${WORKSPACE}\\GURaaSDev\\GURaaSDev.uproject\" \"${root}\\${installName}\"" )
    }
}

def ScanUEInstallFolder(String root) 
{
    def result = []
    dir(root)
    {
        def allDirs = bat returnStdout: true, script: 'dir . /b /A:D'
        allDirs = allDirs.tokenize('\n').collect() { it }
        allDirs = allDirs.drop(2)
        for (dir in allDirs) 
        {
            if ( dir.startsWith('UE_') )
            {
                result.add(dir.trim())
            }
        }
    }
    return result
}

def scanSFXFiles(String location) 
{
    def result = []
    
    String root = "${WORKSPACE}\\${location}\\sfx"
    dir(root)
    {
        def allFiles = bat returnStdout: true, script: 'dir . /b /A:-D'
        allFiles = allFiles.tokenize('\n').collect() { it }
        allFiles = allFiles.drop(2)
        for (file in allFiles) 
        {
            echo "Found file: ${file}"
            result.add(file.trim())
        }
    }
    return result
}

def PublishToGithub(String buildName, String location) 
{
    //def sfxFiles = scanSFXFiles(location)

    bat("gh release create ${buildName} -t \"${buildName}\" --draft --generate-notes")

    //for(file in sfxFiles)
    //{
    bat("gh release upload ${buildName} \"${WORKSPACE}\\${location}\\${buildName}.zip\"")
    //}
}

def PublishToNexus(boolean buseSFX, boolean bneedsPublishToGithub, String cUrl, String sbuildName, String location ) 
{
    withCredentials([string(credentialsId: 'NEXUS_CREDENTIALS', variable: 'NEXUS_CREDENTIALS')])
    {
        // if( buseSFX || bneedsPublishToGithub)
        // {
        //     // we need to upload several files
        //     def sfxFiles = scanSFXFiles(location)

        //     for(file in sfxFiles)
        //     {
        //         bat("\"${CUrl}\" -X POST \"http://localhost:8081/service/rest/v1/components?repository=UEGURaaS-Releases\" -H \"accept: application/json\" -H \"Authorization: Basic $NEXUS_CREDENTIALS\" -F \"raw.directory=${sbuildName}-packaged\" -F \"raw.asset1=@${WORKSPACE}\\${location}\\sfx\\${file}\" -F \"raw.asset1.filename=${file}\"")
        //     }
        // }

        if ( !buseSFX )
        {
            bat("\"${CUrl}\" -X POST \"http://localhost:8081/service/rest/v1/components?repository=UEGURaaS-Releases\" -H \"accept: application/json\" -H \"Authorization: Basic $NEXUS_CREDENTIALS\" -F \"raw.directory=${sbuildName}\" -F \"raw.asset1=@${WORKSPACE}\\${location}\\${sbuildName}.zip;type=application/x-zip-compressed\" -F \"raw.asset1.filename=${sbuildName}.zip\"")
        }
    }
}

pipeline
{
    agent
    {
        node
        {
            label 'windows'
        }
    }
    stages
    {
        stage('Preparation')
        {
            steps
            {
                echo 'Preparation'
                script
                {
                    try
                    {
                        //fetch tags
                        bat("git fetch --all --tags")

                        // get tag name with git describe --exact-match --tags HEAD
                        def gitTag = bat(returnStdout: true, script: '@git describe --exact-match --tags HEAD').trim()
                        echo "Git tag: ${gitTag}"
                        buildName = gitTag
                        needsPublishToGithub = true
                    }
                    catch( e )
                    {
                        echo "No git tag found, use commit short hash and time stamp for local build"

                        // get commit short hash with git rev-parse --short HEAD
                        def gitCommit = bat(returnStdout: true, script: '@git rev-parse --short HEAD').trim()

                        buildName = "GURaaS-" + new Date().format("yyyy-MM-dd-HH-mm-ss") + "-" + gitCommit
                        needsPublishToGithub = false
                    }
                    finally
                    {
                        echo "Build name: ${buildName}"
                    }

                    try 
                    {
                        //scan for unreal installs
                        UEInstalls = ScanUEInstallFolder( UEInstallsRoot )

                        //filter install to latest 3 and 5.0 or above
                        UEInstalls = UEInstalls.sort()
                        UEInstalls = UEInstalls.reverse()

                        for (install in UEInstalls) {
                            echo "Found Unreal Engine ${install} in ${UEInstallsRoot}"
                        }

                        UEInstalls = UEInstalls.subList(0, 3)
                        UEInstalls = UEInstalls.findAll { it.startsWith('UE_5') }
                        
                        UEInstalls = UEInstalls.reverse()

                        for (install in UEInstalls) {
                            echo "Unreal Engine ${install} targeted for build"
                        }

                        if (UEInstalls.size() == 0) {
                            currentBuild.result = 'ABORTED'
                            throw new Exception("No Unreal Engine 5.0 or above found in ${UEInstallsRoot}")
                        }
                    }
                    catch (Exception e)
                    {
                        echo 'Preparation failed'
                        echo "Reason: ${e}"
                        currentBuild.result = 'FAILURE'
                        throw e
                    }
                }
            }
        }
        stage('Build')
        {
            steps
            {
                echo 'Build'
                script
                {
                    try 
                    {
                        for (install in UEInstalls) {
                            echo "Building for Unreal Engine ${install}"
                            SwitchEngineVersion(UEInstallsRoot, install)
                            for (plugin in PluginsToBuild) {
                                echo "Building ${plugin}"
                                BuildPlugin(UEInstallsRoot, plugin, install, localBuildFolder)
                            }
                        }
                    }
                    catch (Exception e)
                    {
                        echo 'Build failed'
                        echo "Reason: ${e}"
                        currentBuild.result = 'FAILURE'
                        throw e
                    }
                }
            }
        }
        stage('Package')
        {
            when
            {
                expression { currentBuild.result == 'SUCCESS' || currentBuild.result == null }
            }
            steps
            {
                echo 'Package'
                script
                {
                    try 
                    {
                       // if (useSFX || needsPublishToGithub) 
                       // {
                       //     bat( "7z a -mx${zipCompressionLevel} -v${zipVolumeSize} -sfx -r \"${WORKSPACE}\\${localPackageFolder}\\sfx\\${buildName}.exe\" \"${WORKSPACE}\\${localBuildFolder}\\*\"  ")
                       // } 

                        if (!useSFX)
                        {
                            bat( "7z a -tzip -mx${zipCompressionLevel} -r \"${WORKSPACE}\\${localPackageFolder}\\${buildName}.zip\" \"${WORKSPACE}\\${localBuildFolder}\\*\" ")
                        }
                    }
                    catch (Exception e)
                    {
                        echo 'Package failed'
                        echo "Reason: ${e}"
                        currentBuild.result = 'FAILURE'
                        throw e
                    }
                }
            }
        }
        stage('Upload')
        {
            when
            {
                expression { currentBuild.result == 'SUCCESS' || currentBuild.result == null }
            }
            steps 
            {
                echo 'Upload'
                script
                {
                    try
                    {
                        if( needsPublishToGithub )
                        {
                            PublishToGithub(buildName, localPackageFolder)
                        }

                        PublishToNexus(useSFX, needsPublishToGithub, cURLexe, buildName, localPackageFolder)
                    }
                    catch (Exception e)
                    {
                        echo 'Upload failed'
                        echo "Reason: ${e}"
                        currentBuild.result = 'FAILURE'
                        throw e
                    }
                }
            }
        }
    }
    post
    {
        always
        {
            script
            {
                if (currentBuild.result == 'SUCCESS')
                {
                    if(needsPublishToGithub)
                    {   
                        slackSend color: COLOR_MAP[currentBuild.result], message: "${env.JOB_NAME} ${currentBuild.result}, ${buildName} published to Github as draft and available on Nexus"
                    }
                    else
                    {
                        slackSend color: COLOR_MAP[currentBuild.result], message: "${env.JOB_NAME} ${currentBuild.result}, ${buildName} available on Nexus for internal testing"
                    }
                }
                else
                {
                    slackSend color: COLOR_MAP[currentBuild.result], message: "${env.JOB_NAME} ${currentBuild.result}, ${buildName} not published, see: ${env.BUILD_URL}"
                    
                    if(needsPublishToGithub)
                    {
                        try
                        {
                            bat("gh release delete ${buildName} --yes")
                        }
                        catch (Exception e)
                        {
                            echo "${e}"
                        }
                    }
                }
            }
        }
        cleanup
        {
            echo 'Clean up time!'
            
            // remove project files
            bat( "IF exist \"${WORKSPACE}\\GURaaSDev\" rmdir \"${WORKSPACE}\\GURaaSDev\" /S /Q" )

            // remove build folder
            bat( "IF exist \"${WORKSPACE}\\${localBuildFolder}\" rmdir \"${WORKSPACE}\\${localBuildFolder}\" /S /Q" )

            // remove package folder
            bat( "IF exist \"${WORKSPACE}\\${localPackageFolder}\" rmdir \"${WORKSPACE}\\${localPackageFolder}\" /S /Q" )

            // remove .git
            bat( "IF exist \"${WORKSPACE}\\.git\" rmdir \"${WORKSPACE}\\.git\" /S /Q" )

            // remove .github
            bat( "IF exist \"${WORKSPACE}\\.github\" rmdir \"${WORKSPACE}\\.github\" /S /Q" )

            //clear remaining files
            bat( "del \"${WORKSPACE}\\*\" /F /Q" )

            echo 'Clean up done!'
        }
    }
}
