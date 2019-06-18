#pragma once

#include <iostream>

using namespace std;

class Calculated
{
private:
	string UserAgent;
	string uploadBoundary;

public:
	Calculated(string userAgent, string uploadBoundary, void(*log)(void* object, string message), void(*NotifyUploadResult)(void* object, bool result), void* client);
	~Calculated();

	shared_ptr<string> visibility = make_shared<string>("DEFAULT");
	shared_ptr<string> tags = make_shared<string>("");

	void(*Log)(void* object, string message);
	void(*NotifyUploadResult)(void* object, bool result);
	void* Client;

	void UploadReplay(string replayPath, string playerId);
};

