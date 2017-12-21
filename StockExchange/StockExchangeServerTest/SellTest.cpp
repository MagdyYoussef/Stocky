
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "CasablacaServer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockExchangeServerTest
{

	TEST_CLASS(SellTest)
	{
	public:
		CasablancaServer* server;

		SellTest()
		{
			server = new CasablancaServer(DATABASE_SERVER, DATABASE_TEST, DATABASE_USERNAME, DATABASE_PASSWORD);
		}

		~SellTest()
		{
			delete server;
		}

		TEST_METHOD(TestSellWithInvalidStockCode)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/sell");
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

		TEST_METHOD(TestSellWithInsufficientStockQuantity)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/sell");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test4"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("DX100"));
			requestBody[L"stock_quantity"] = json::value(Utility::StringToWString("5"));
			requestBody[L"stock_price"] = json::value(Utility::StringToWString("5000"));

			request.set_body(requestBody);

			
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Insufficient quantity.\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}

		TEST_METHOD(TestSellWithSufficientStockQuantity)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/sell");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test3"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));
			requestBody[L"stock_code"] = json::value(Utility::StringToWString("TT500"));
			requestBody[L"stock_quantity"] = json::value(Utility::StringToWString("1"));
			requestBody[L"stock_price"] = json::value(Utility::StringToWString("1000"));

			request.set_body(requestBody);

			
			Assert::AreEqual(true, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 200); //OK
			Assert::AreEqual(L"{\"response\":\"ok\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}

		TEST_METHOD(TestSellWithMissingParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/sell");
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
		TEST_METHOD(TestSellWithIncorrectLengthParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/sell");
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
			Assert::AreEqual(L"{\"response\":\"invalid price or quantity value\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestSellWithIncorrectQuantityOrPriceParameters)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/sell");
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