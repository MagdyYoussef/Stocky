
#include "Utility.h"
#include "RestClient.h"  

RestClient::RestClient()
{
	this->baseUrl = L"http://localhost/terminal/";
}

RestClient::RestClient(std::wstring baseUrl)
{
	this->baseUrl = baseUrl;
}


RestClient::~RestClient()
{

}

std::wstring RestClient::Login(std::wstring username, std::wstring password)
{
	try
	{
		http_client client(this->baseUrl + L"login");
		json::value requestBody;
		requestBody[L"username"] = json::value(username);
		requestBody[L"password"] = json::value(password);
		http_request request;
		request.set_body(requestBody);
		request.set_method(methods::POST);
		request.headers().set_content_type(L"application/json");
		std::wstring  serverResponse;
		client.request(request).then([&serverResponse](http_response response)
		{
			if (response.status_code() == status_codes::OK)
			{
				auto jsonResponse = response.extract_json().get();
				if (jsonResponse[L"response"].as_string() == L"ok")
					serverResponse = L"logged in successfully";
			}
			else
				serverResponse = L"Login Failed";
		}).wait();

		return  serverResponse;
	}
	catch (std::exception& e)
	{
		TRACE(e.what(), 1);
	}
}

std::wstring RestClient::Register(std::wstring username, std::wstring password, std::wstring balance)
{
	try
	{
		http_client client(this->baseUrl + L"register");
		json::value requestBody;
		requestBody[L"username"] = json::value(username);
		requestBody[L"password"] = json::value(password);
		requestBody[L"balance"] = json::value(balance);
		http_request request;
		request.set_body(requestBody);
		request.set_method(methods::POST);
		request.headers().set_content_type(L"application/json");
		std::wstring  serverResponse;
		client.request(request).then([&serverResponse](http_response response)
		{
			auto jsonResponse = response.extract_json().get();
			serverResponse = jsonResponse[L"response"].as_string();
			if (response.status_code() == status_codes::OK && serverResponse == L"ok")
				serverResponse = L"Registered successfully";
		}).wait();
		return  serverResponse;
	}
	catch (std::exception& e)
	{
		TRACE(e.what(), 1);
	}
}

std::wstring RestClient::Quote(std::wstring username, std::wstring password, std::wstring stock_code)
{
	try
	{
		http_client client(this->baseUrl + L"quote");
		json::value requestBody;
		requestBody[L"username"] = json::value(username);
		requestBody[L"password"] = json::value(password);
		requestBody[L"stock_code"] = json::value(stock_code);
		http_request request;
		request.set_body(requestBody);
		request.set_method(methods::POST);
		request.headers().set_content_type(L"application/json");
		std::wstring  serverResponse;
		client.request(request).then([&serverResponse](http_response response)
		{
			auto jsonResponse = response.extract_json().get();
			serverResponse = jsonResponse[L"response"].as_string();
			if (response.status_code() == status_codes::OK && serverResponse == L"ok")
				serverResponse = jsonResponse[L"stock_price"].as_string();

		}).wait();
		return  serverResponse;
	}
	catch (std::exception& e)
	{
		TRACE(e.what(), 1);
	}
}

std::wstring RestClient::Portofolio(std::wstring username, std::wstring password)
{
	try
	{
		http_client client(this->baseUrl + L"portofolio");
		json::value requestBody;
		requestBody[L"username"] = json::value(username);
		requestBody[L"password"] = json::value(password);
		http_request request;
		request.set_body(requestBody);
		request.set_method(methods::POST);
		request.headers().set_content_type(L"application/json");
		std::wstring  serverResponse;
		client.request(request).then([&serverResponse](http_response response)
		{
			auto jsonResponse = response.extract_json().get();
			serverResponse = jsonResponse[L"response"].as_string();
			if (response.status_code() == status_codes::OK && serverResponse == L"ok")
			{
				serverResponse = L"";
				for (auto item : jsonResponse[L"portofolio"].as_array())
				{
					serverResponse += L"Code: " + item[L"stock_code"].as_string()  + L"\t\t\t"
						L"Quantity: " + item[L"quantity"].as_string() + L"\t\t\t"
						L"Price: " + item[L"stock_price"].as_string() + L"\r\n";
				}
			}

		}).wait();
		return  serverResponse;
	}
	catch (std::exception& e)
	{
		TRACE(e.what(), 1);
	}
}

std::wstring RestClient::Transactions(std::wstring username, std::wstring password)
{
	try
	{
		http_client client(this->baseUrl + L"transactions");
		json::value requestBody;
		requestBody[L"username"] = json::value(username);
		requestBody[L"password"] = json::value(password);
		http_request request;
		request.set_body(requestBody);
		request.set_method(methods::POST);
		request.headers().set_content_type(L"application/json");
		std::wstring  serverResponse;
		client.request(request).then([&serverResponse](http_response response)
		{
			auto jsonResponse = response.extract_json().get();
			serverResponse = jsonResponse[L"response"].as_string();
			if (response.status_code() == status_codes::OK && serverResponse == L"ok")
			{
				serverResponse = L"";
				for (auto item : jsonResponse[L"transactions"].as_array())
				{
					serverResponse +=	L"Code: " + item[L"stock_code"].as_string() + L"\t\t\t"
										L"Quantity: " + item[L"quantity"].as_string() + L"\t\t\t"
										L"Price: " + item[L"price"].as_string() + L"\t\t\t"
										L"Type: " + item[L"transaction_type"].as_string() + L"\t\t"
										L"Time: " + item[L"transaction_time"].as_string() + L"\r\n";
				}
			}

		}).wait();
		return  serverResponse;
	}
	catch (std::exception& e)
	{
		TRACE(e.what(), 1);
	}
}


std::wstring RestClient::Buy(std::wstring username, std::wstring password, std::wstring stock_code, std::wstring stock_quantity, std::wstring stock_price)
{
	try
	{
		http_client client(this->baseUrl + L"buy");
		json::value requestBody;
		requestBody[L"username"] = json::value(username);
		requestBody[L"password"] = json::value(password);
		requestBody[L"stock_code"] = json::value(stock_code);
		requestBody[L"stock_quantity"] = json::value(stock_quantity);
		requestBody[L"stock_price"] = json::value(stock_price);
		http_request request;
		request.set_body(requestBody);
		request.set_method(methods::POST);
		request.headers().set_content_type(L"application/json");
		std::wstring  serverResponse;
		client.request(request).then([&serverResponse](http_response response)
		{
			auto jsonResponse = response.extract_json().get();
			serverResponse = jsonResponse[L"response"].as_string();
			if (response.status_code() == status_codes::OK && serverResponse == L"ok")
				serverResponse = L"Operation Successfull.";
		}).wait();
		return  serverResponse;
	}
	catch (std::exception& e)
	{
		TRACE(e.what(), 1);
	}
}

std::wstring RestClient::Sell(std::wstring username, std::wstring password, std::wstring stock_code, std::wstring stock_quantity, std::wstring stock_price)
{
	try
	{
		http_client client(this->baseUrl + L"sell");
		json::value requestBody;
		requestBody[L"username"] = json::value(username);
		requestBody[L"password"] = json::value(password);
		requestBody[L"stock_code"] = json::value(stock_code);
		requestBody[L"stock_quantity"] = json::value(stock_quantity);
		requestBody[L"stock_price"] = json::value(stock_price);
		http_request request;
		request.set_body(requestBody);
		request.set_method(methods::POST);
		request.headers().set_content_type(L"application/json");
		std::wstring  serverResponse;
		client.request(request).then([&serverResponse](http_response response)
		{
			auto jsonResponse = response.extract_json().get();
			serverResponse = jsonResponse[L"response"].as_string();
			if (response.status_code() == status_codes::OK && serverResponse == L"ok")
				serverResponse = L"Operation Successfull.";
		}).wait();
		return  serverResponse;
	}
	catch (std::exception& e)
	{
		TRACE(e.what(), 1);
	}
}





