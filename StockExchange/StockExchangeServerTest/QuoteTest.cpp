
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "CasablacaServer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockExchangeServerTest
{

	TEST_CLASS(QuoteTest)
	{
	public:
		CasablancaServer* server;

		QuoteTest()
		{
			server = new CasablancaServer(DATABASE_SERVER, DATABASE_TEST, DATABASE_USERNAME, DATABASE_PASSWORD);
		}

		~QuoteTest()
		{
			delete server;
		}
		TEST_METHOD(TestQuoteWithValidParameter)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/quote");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("DX100"));

			request.set_body(requestBody);

			
			Assert::AreEqual(true, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 200); //OK
			Assert::AreEqual(L"{\"response\":\"ok\",\"stock_price\":\"5000\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestQuoteWithInvalidParameter)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/quote");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("FR400"));

			request.set_body(requestBody);

			
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Can't find a stock with that code.\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}


		TEST_METHOD(TestQuoteWithMissingStockCode)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/quote");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");

			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));

			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"missing parameters\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestQuoteWithIncorrectLengthParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/quote");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");

			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString(""));

			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"invalid parameters length\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

	};
}