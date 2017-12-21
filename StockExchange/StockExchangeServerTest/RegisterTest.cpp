
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "CasablacaServer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockExchangeServerTest
{

	TEST_CLASS(RegisterTest)
	{
	public:
		CasablancaServer* server;

		RegisterTest()
		{
			server = new CasablancaServer(DATABASE_SERVER, DATABASE_TEST, DATABASE_USERNAME, DATABASE_PASSWORD);
		}

		~RegisterTest()
		{
			delete server;
		}
		TEST_METHOD(TestRegisterWithValidParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/register");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			std::string randomTime;
			std::stringstream ss;
			ss << time(0);
			ss >> randomTime;
			requestBody[L"username"] = json::value(Utility::StringToWString("test" + randomTime));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"balance"] = json::value(Utility::StringToWString("10000"));


			request.set_body(requestBody);

			
			Assert::AreEqual(true, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 200); //OK
			Assert::AreEqual(L"{\"response\":\"ok\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestRegisterWithMissingParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/register");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("nopassword"));
			requestBody[L"balance"] = json::value(Utility::StringToWString("10000"));


			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"missing parameters\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}
		TEST_METHOD(TestRegisterWithIncorrectLengthParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/register");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("tes"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"balance"] = json::value(Utility::StringToWString("10000"));


			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"invalid parameters length\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestRegisterWithExistedParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/register");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"balance"] = json::value(Utility::StringToWString("10000"));


			request.set_body(requestBody);
			
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"username already exists\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestRegisterWithInvalidBalanceValue)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/register");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"balance"] = json::value(Utility::StringToWString("test"));


			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"invalid balance value\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}
	};
}