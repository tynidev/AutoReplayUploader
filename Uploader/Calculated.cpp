#include "Calculated.h"

#include "HttpClient.h"
#include "Utils.h"

using namespace std;

Calculated::Calculated(string userAgent, string uploadBoundary, void(*log)(void* object, string message), void(*NotifyUploadResult)(void* object, bool result), void* client)
{
	this->UserAgent = userAgent;
	this->uploadBoundary = uploadBoundary;
	this->Log = log;
	this->NotifyUploadResult = NotifyUploadResult;
	this->Client = client;
}

void CalculatedRequestComplete(HttpRequestObject* ctx)
{
	auto calculated = (Calculated*)ctx->Requester;

	calculated->Log(calculated->Client, "Calculated::UploadCompleted with status: " + to_string(ctx->Status));
	calculated->NotifyUploadResult(calculated->Client, (ctx->Status >= 200 && ctx->Status < 300));

	delete[] ctx->ReqData;
	delete[] ctx->RespData;
	delete ctx;
}

/**
* Posts the replay file to Calculated.gg
*/
void Calculated::UploadReplay(string replayPath, string playerId)
{
	if (UserAgent.empty() || replayPath.empty())
	{
		Log(Client, "Calculated::UploadReplay Parameters were empty.");
		Log(Client, "UserAgent: " + UserAgent);
		Log(Client, "ReplayPath: " + replayPath);
		return;
	}

	string path = AppendGetParams("api/upload", { {"player_id", playerId}, {"visibility", *visibility} });
	if (tags->size() > 0)
	{
		string tagParam = "&tags=" + *tags;
		ReplaceAll(tagParam, ",", "&tags=");
		path += tagParam;
	}

	// Fire new thread and make request, dont't wait for response
	HttpFileUploadAsync(
		"calculated.gg",
		path,
		UserAgent,
		replayPath,
		"replays",
		"",
		uploadBoundary,
		1,
		this,
		&CalculatedRequestComplete);
}

Calculated::~Calculated()
{
}
