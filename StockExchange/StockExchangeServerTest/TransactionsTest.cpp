
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "CasablacaServer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockExchangeServerTest
{

	TEST_CLASS(TransactionsTest)
	{
	public:
		CasablancaServer* server;

		TransactionsTest()
		{
			server = new CasablancaServer(DATABASE_SERVER, DATABASE_TEST, DATABASE_USERNAME, DATABASE_PASSWORD);
		}

		~TransactionsTest()
		{
			delete server;
		}
		TEST_METHOD(TestTransactionsWithValidParameter)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/transactions");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));

			request.set_body(requestBody);

			
			Assert::AreEqual(true, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 200); //OK
			Assert::AreEqual(L"{\"response\":\"ok\",\"transactions\":[{\"price\":\"500\",\"quantity\":\"5\",\"stock_code\":\"DX100\",\"transaction_time\":\"1/1/2016\",\"transaction_type\":\"buy\"},{\"price\":\"1000\",\"quantity\":\"2\",\"stock_code\":\"DX100\",\"transaction_time\":\"2/10/2016\",\"transaction_type\":\"sell\"}]}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestTransactionsWithInvalidParameter)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/transactions");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test2"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));

			request.set_body(requestBody);

			
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"No Transactions has been made yet.\"}", request.get_response().get().extract_json().get().to_string().c_str());
		}
	};
}