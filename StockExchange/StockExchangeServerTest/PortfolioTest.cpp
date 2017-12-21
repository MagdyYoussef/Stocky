
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "CasablacaServer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockExchangeServerTest
{

	TEST_CLASS(PortfolioTest)
	{
	public:
		CasablancaServer* server;

		PortfolioTest()
		{
			server = new CasablancaServer(DATABASE_SERVER, DATABASE_TEST, DATABASE_USERNAME, DATABASE_PASSWORD);
		}

		~PortfolioTest()
		{
			delete server;
		}
		TEST_METHOD(TestPortfolioWithValidParameter)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/portofolio");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));

			request.set_body(requestBody);

			Assert::AreEqual(true, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 200); //OK
			Assert::AreEqual(L"{\"portofolio\":[{\"quantity\":\"5\",\"stock_code\":\"DX100\",\"stock_price\":\"5000\"},{\"quantity\":\"50\",\"stock_code\":\"DT400\",\"stock_price\":\"1000\"}],\"response\":\"ok\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}

		TEST_METHOD(TestPortfolioWithInvalidParameter)
		{
			http_request request;
			request.set_request_uri(L"http://localhost/portofolio");
			request.set_method(methods::POST);
			request.headers().set_content_type(L"application/json");


			json::value requestBody;
			requestBody[L"username"] = json::value(Utility::StringToWString("test2"));
			requestBody[L"password"] = json::value(Utility::StringToWString("test"));

			request.set_body(requestBody);

			
			Assert::AreEqual(false, server->HandleRequest(request));
			Assert::AreEqual((int)request.get_response().get().status_code(), 400); //Bad Request
			Assert::AreEqual(L"{\"response\":\"Empty Portfolio.\"}", request.get_response().get().extract_json().get().to_string().c_str());

		}
	};
}