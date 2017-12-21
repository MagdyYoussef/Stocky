
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "CasablacaServer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockExchangeServerTest
{

	TEST_CLASS(RequestValidationTest)
	{
	public:
		CasablancaServer* server;

		RequestValidationTest()
		{
			server = new CasablancaServer(DATABASE_SERVER, DATABASE_TEST, DATABASE_USERNAME, DATABASE_PASSWORD);
		}

		~RequestValidationTest()
		{
			delete server;
		}

		TEST_METHOD(TestRequestWithInvalidMethod)
		{
			http_request request;
			request.set_method(methods::GET);
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Request ContentType or Method is invalid.\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}

		TEST_METHOD(TestRequestWithInvalidContentType)
		{
			http_request request;
			request.headers().set_content_type(U("text/plain"));
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Request ContentType or Method is invalid.\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}

		TEST_METHOD(TestRequestWithInvalidAction)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/terminal/koko");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");

			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Invalid Action\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}
	};
}