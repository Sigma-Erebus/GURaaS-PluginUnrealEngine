# How to use

## Installation

### Precompiled plugin
(Windows only)

1. Download the latest release from the [release page]( https://github.com/BredaUniversityResearch/GURaaS-PluginUnrealEngine/releases )
2. Unzip the downloaded file
3. Copy the `GURaaS` folder to the `Plugins` folder of your Unreal Engine project
```
UnrealGame/
├───Content/
├───Plugins/
│   └───GURaaS/ <-- Copy the GURaaS folder here
├───UnrealGame.uproject
```

### Compiling from source
Two options are available to compile the plugin from source:

#### 1. Using the project included in this repository
1. Clone this repository
2. Generate C++ build files using the `GURaaSDev/GURaaSDev.uproject` file
3. Build the project using the generated build files and open the project in Unreal Engine
4. Use the plugin window to package the plugin for your project

#### 2. Adding the plugin source to your project
1. Create a new C++ project using Unreal Engine or convert an existing project to a C++ project
2. Clone this repository
3. Copy the `GURaaS` folder located in `GURaaSDev/Plugins/` to the `Plugins` folder of your Unreal Engine project
4. Build the project.

## Usage

GURaaS is a plugin that can be used to create a connection between Unreal Engine and the GURaaS server. The plugin can be used to send and receive data from the GURaaS server. To use the plugin, the plugin must be enabled in the Unreal Engine project. This can be done by opening the project in Unreal Engine and enabling the plugin in the plugin window.

### Sending data to the GURaaS server
This can only be done when the game is running. Either through the editor or a packaged game. To enable this the Unreal game instance must derive from UGURaaSGameInstance(C++) or GURaaSGameInstance (Blueprint & Editor). The game instance can be set in the project settings under Maps & Modes. You can create you own GameInstance class that derives from GURaaSGameInstance or use the one provided by the plugin if you do not require any custom functionality.

Before you can log data the API needs to be initialized using `UGLog::Initialize(FString GameID, UGURaaSGameInstance* GameInstance)` or using the blueprint node see below. The GameID can be found in the GURaaS web interface. 

Now that the API is initialized you can start a session using `UGLog::StartSession(FString PlayerID, FString context)` or using the blueprint node see below. The PlayerID is a unique identifier for the player. The context is a string that can be used to identify the current context of the game in channels.
`CloseSession()` can be used to close the session.

To log data to the GURaaS server you can use the `UGLog::Log(EGURaaSVerboseLevel VerboseLevel, FString Message, FString Tag1, FString Tag2, FString Tag3)` 

### Getting data from the GURaaS server

Getting data from the GURaaS does not require you to have the logging API initialized. Getting the data is a single HTTP Post call to the GURaaS server. using `GetLogsFromGURaaS( FString GameID, FString JSonQuery, const FGURaaSGetLogsDelegate& Delegate)` Unreal will call the delegate when the data is received or if an error occurred. The data is returned as an TArray of FStrings in the log JSON format. 
The query is a JSON string that can be used to filter the data. The query is a JSON object with the following fields:

```
{
    "sessions_id": "74b4ee75-4494-5067-4d58-34a1793c6604" // unused
    "tag1": "Event", // Tag1 is used as verbose level
    "tag2": "GLog",  // Tag2 = tag1 ... etc
    "tag3": "Test",
    "tag4": "Test",
    "sdate": "2023-04-20 09:32:30.0"
    "edate": "2023-04-20 07:32:30.0"
}```

The database of GURaaS does not store verbose levels. This causes the mis-alignment of the field names.

### Custom Channels










