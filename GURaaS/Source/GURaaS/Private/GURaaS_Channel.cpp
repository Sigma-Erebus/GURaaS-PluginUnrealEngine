#include "GURaaS_Channel.h"

#include "GURaaS_API.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

DEFINE_LOG_CATEGORY(LogGLog);


void UGLogChannel::StartSession(FSessionInfo SessionInfo)
{
	m_SessionInfo = SessionInfo;
}


UGLogChannelGURaaS::UGLogChannelGURaaS() 
{
	auto* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	world->GetTimerManager().SetTimerForNextTick(this, &UGLogChannelGURaaS::Process);
}

void UGLogChannelGURaaS::Process()
{
	Flush();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UGLogChannelGURaaS::Process);
}

void UGLogChannelGURaaS::Log(EGURaaSVerboseLevel VerboseLevel, const FString& Message, const FString& Tag1, const FString& Tag2, const FString& Tag3)
{
	m_LogEntries.Add(LogEntry{ VerboseLevel, Message, Tag1, Tag2, Tag3, FDateTime::UtcNow() });
}

void UGLogChannelGURaaS::Flush() 
{
	if (m_LogEntries.Num() == 0)
	{
		return;
	}
	//check if session data is valid
	if (m_SessionInfo.m_SessionId.IsEmpty() || m_SessionInfo.m_GameId.IsEmpty() )
	{
		UE_LOG(LogGLog, Error, TEXT("Session data is not valid. Cannot send log entries."));
		return;
	}
	
	//create post request
	auto Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(m_Url + "/v1/games/" + m_SessionInfo.m_GameId + "/data");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	
	//create json object
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("id_session", m_SessionInfo.m_SessionId);
	JsonObject->SetStringField("id_player", m_SessionInfo.m_PlayerId);
	JsonObject->SetStringField("version", FString::Printf(TEXT("UE%d.%d"), ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION));
	JsonObject->SetStringField("start", m_SessionInfo.m_SessionStart.ToString(TEXT("%Y-%m-%d %H:%M:%S")));
	JsonObject->SetStringField("end", FDateTime::UtcNow().ToString(TEXT("%Y-%m-%d %H:%M:%S")));
	JsonObject->SetStringField("context", m_SessionInfo.m_Context);
	
	//create json array
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for (auto& LogEntry : m_LogEntries)
	{
		TSharedPtr<FJsonObject> LogEntryObject = MakeShareable(new FJsonObject());
		LogEntryObject->SetStringField("time", LogEntry.m_TimeStamp.ToString(TEXT("%Y-%m-%d %H:%M:%S")));
		LogEntryObject->SetStringField("tag1", UGLog_API::ConvertToString(LogEntry.m_VerboseLevel));
		LogEntryObject->SetStringField("tag2", LogEntry.m_Tag1);
		LogEntryObject->SetStringField("tag3", LogEntry.m_Tag2);
		LogEntryObject->SetStringField("tag4", LogEntry.m_Tag3);
		LogEntryObject->SetStringField("data", LogEntry.m_Message);
		JsonArray.Add(MakeShareable(new FJsonValueObject(LogEntryObject)));
	}

	JsonObject->SetArrayField("data", JsonArray);
	
	
	//set request content
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	Request->SetContentAsString(OutputString);
	
	//send request
	auto ProcessResponse = [this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{	
		if (!bWasSuccessful)
		{
			UE_LOG(LogGLog, Error, TEXT("Failed to send log entries."));
			return;
		}
		if (!(Response->GetResponseCode() >= 200 && Response->GetResponseCode() < 300))
		{
			UE_LOG(LogGLog, Error, TEXT("Failed to send log entries. Response code: %d Responce: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
			return;
		}
		UE_LOG(LogGLog, Log, TEXT("Log entries sent successfully."));
	};

	Request->OnProcessRequestComplete().BindLambda(ProcessResponse);

	Request->ProcessRequest();
	m_LogEntries.Empty();
	
}

void UGLogChannelConsole::Log(EGURaaSVerboseLevel VerboseLevel, const FString& Message, const FString& Tag1, const FString& Tag2, const FString& Tag3)
{
	UE_LOG(LogGLog, Log, TEXT("Time: %s, %s, (%s, %s, %s): %s "), *FDateTime::UtcNow().ToString(), *UGLog_API::ConvertToString(VerboseLevel), *Tag1, *Tag2, *Tag3, *Message);
}





