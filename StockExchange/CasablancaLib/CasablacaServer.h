
#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class DatabaseADO;

class CasablancaServer
{
private:	
	DatabaseADO* dbInstance;
	int currentUserID;
	http_request request;
	std::wstring requestAction;
	std::wstring requestMethod;
	std::wstring requestContentType;
	json::value jsonObj;

	//helper functions
	bool SendReply(bool success, std::wstring message, status_code statusCode, json::value responseJson);
	bool ValidateRequest();
	bool ParseRequestJson();
	bool Authenticate();
	bool Register();
	bool Quote();
	bool Buy();
	bool Sell();
	bool Transactions();
	bool Portfolio();
	bool Login();

public:
	CasablancaServer(std::string dbServer, std::string dbName, std::string dbUsername, std::string dbPassword);
	CasablancaServer(DatabaseADO* databaseInstance);
	~CasablancaServer();
	bool HandleRequest(http_request request);
};

