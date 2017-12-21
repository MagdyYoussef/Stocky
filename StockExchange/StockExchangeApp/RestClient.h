
#pragma once
#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

class RestClient
{
public:
	RestClient(std::wstring baseUrl);
	RestClient();
	~RestClient();
	std::wstring RestClient::Login(std::wstring username, std::wstring password);
	std::wstring Register(std::wstring username, std::wstring password, std::wstring balance);
	std::wstring Quote(std::wstring username, std::wstring password, std::wstring stock_code);
	std::wstring RestClient::Portofolio(std::wstring username, std::wstring password);
	std::wstring RestClient::Transactions(std::wstring username, std::wstring password);
	std::wstring Buy(std::wstring username, std::wstring password, std::wstring stock_code, std::wstring stock_quantity, std::wstring stock_price);
	std::wstring Sell(std::wstring username, std::wstring password, std::wstring stock_code, std::wstring stock_quantity, std::wstring stock_price);
private:
	std::wstring baseUrl;
};

