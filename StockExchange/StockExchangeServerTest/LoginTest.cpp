
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "CasablacaServer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockExchangeServerTest
{

	TEST_CLASS(LoginTest)
	{
	public:
		CasablancaServer* server;

		LoginTest()
		{
			server = new CasablancaServer(DATABASE_SERVER, DATABASE_TEST, DATABASE_USERNAME, DATABASE_PASSWORD);
		}

		~LoginTest()
		{
			delete server;
		}
		TEST_METHOD(TestAuthenticationWithValidCredentials)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/login");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));

			request.set_body(requestBody);

			Assert::AreEqual(true, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 200); //OK
			Assert::AreEqual(L"{\"response\":\"ok\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}

		TEST_METHOD(TestAuthenticationWithInvalidCredentials)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/login");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("koko"));
			requestBody[L"password"] = json::value(Utility::StringToWString("koko"));

			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Authentication Failed.\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestLoginWithMissingParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/login");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");

			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("nopassword"));

			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"missing parameters\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestLoginWithIncorrectLengthParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/login");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");

			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("tes"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));

			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"invalid parameters length\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}
	};
}