
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "CasablacaServer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockExchangeServerTest
{

	TEST_CLASS(BuyTest)
	{
	public:
		CasablancaServer* server;

		BuyTest()
		{
			server = new CasablancaServer(DATABASE_SERVER, DATABASE_TEST, DATABASE_USERNAME, DATABASE_PASSWORD);
		}

		~BuyTest()
		{
			delete server;
		}
		TEST_METHOD(TestBuyWithInvalidStockCode)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/buy");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test3"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("ABC"));  
			requestBody[L"stock_quantity"] = json::value(Utility::StringToWString("5"));
			requestBody[L"stock_price"] = json::value(Utility::StringToWString("1000"));

			request.set_body(requestBody);

			
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Can't find a stock with that code.\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}

		TEST_METHOD(TestBuyWithInsufficientBalance)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/buy");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test4"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("MM1000"));
			requestBody[L"stock_quantity"] = json::value(Utility::StringToWString("5"));
			requestBody[L"stock_price"] = json::value(Utility::StringToWString("5000"));

			request.set_body(requestBody);

			
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Insufficient Balance.\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}

		TEST_METHOD(TestBuyWithSufficientBalance)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/buy");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test3"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("TT500"));
			requestBody[L"stock_quantity"] = json::value(Utility::StringToWString("5"));
			requestBody[L"stock_price"] = json::value(Utility::StringToWString("1000"));

			request.set_body(requestBody);

			
			Assert::AreEqual(true, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 200); //OK
			Assert::AreEqual(L"{\"response\":\"ok\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}

		TEST_METHOD(TestBuyWithMissingParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/buy");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test3"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("TT500"));

			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"missing parameters\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}
		TEST_METHOD(TestBuyWithIncorrectLengthParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/buy");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test3"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("TT500"));
			requestBody[L"stock_quantity"] = json::value(Utility::StringToWString("5"));
			requestBody[L"stock_price"] = json::value(Utility::StringToWString("10"));


			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"invalid parameters length\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}
		TEST_METHOD(TestBuyWithIncorrectQuantityOrPriceParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/buy");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test3"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("TT500"));
			requestBody[L"stock_quantity"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_price"] = json::value(Utility::StringToWString("test"));


			request.set_body(requestBody);

			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"invalid price or quantity value\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}
	};
}