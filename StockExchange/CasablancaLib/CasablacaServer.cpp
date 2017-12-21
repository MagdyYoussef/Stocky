
#include "Utility.h"
#include "DatabaseADO.h"
#include "SHA1.h"
#include "BadRequestException.h"
#include "CasablacaServer.h"
using namespace std;

CasablancaServer::CasablancaServer(string dbServer, string dbName, string dbUsername, string dbPassword)
{
	TRACE("Calling CasablancaServer", 2);
	this->currentUserID = 0;
	this->dbInstance = new DatabaseADO(dbServer, dbName, dbUsername, dbPassword);
}

CasablancaServer::CasablancaServer(DatabaseADO* databaseInstance)
{
	//for mocking
	this->currentUserID = 0;
	this->dbInstance = databaseInstance;
}

bool CasablancaServer::SendReply(bool success, wstring message, status_code statusCode, json::value responseJson = 0)
{
	TRACE("Calling SendReply", 2);

	if (responseJson == 0)
		responseJson = json::value();

	TRACE(message, 1);
	
	responseJson[L"response"] = json::value(message);

	this->request.reply(statusCode, responseJson);
	return success;
}

CasablancaServer::~CasablancaServer()
{
	TRACE("Calling ~CasablancaServer", 2);
	delete this->dbInstance;
}

bool CasablancaServer::ValidateRequest()
{
	TRACE("Calling ValidateRequest", 2);

	this->requestAction = this->request.relative_uri().to_string().erase(0, 1);
	this->requestMethod = this->request.method();
	this->requestContentType = this->request.headers().content_type();

	TRACE("\nHandling incoming request..", 1);
	TRACE(L"Action: " + this->requestAction, 1);
	TRACE(L"Method: " + this->requestMethod, 1);
	TRACE(L"Content-Type: " + this->requestContentType, 1);

	if (this->requestContentType != L"application/json" || this->requestMethod != L"POST")
		throw BadRequestException("Request ContentType or Method is invalid.");

	return true;
}

bool CasablancaServer::ParseRequestJson()
{
	TRACE("Calling ParseRequestJson", 2);

	auto ret = this->request.extract_json();
	this->jsonObj = ret.get();
	return true;

}

bool CasablancaServer::Authenticate()
{
	TRACE("Calling Authenticate", 2);

	if ( !this->jsonObj.has_field(L"username") || !this->jsonObj.has_field(L"password") )
	{
		throw BadRequestException("missing parameters");
	}

	wstring username = this->jsonObj[L"username"].as_string();
	wstring password = this->jsonObj[L"password"].as_string();

	if (username.length() < 4 || username.length() > 20
		|| password.length() < 4 || password.length() > 20)
	{
		throw BadRequestException("invalid parameters length");
	}


	TRACE(L"Authenticating: " + username + L"...", 1);

	string hashed_password = sha1(Utility::WStringToString(password));
	this->currentUserID = this->dbInstance->VerifyUserLogin(username, Utility::StringToWString(hashed_password));

	if (this->currentUserID == 0)
	{
		throw BadRequestException("Authentication Failed.");
	}

	TRACE("Authenticated Successfully.", 1);
	return true;

}

bool CasablancaServer::Quote()
{
	TRACE("Calling Quote", 2);

	if (!this->jsonObj.has_field(L"stock_code"))
	{
		throw BadRequestException("missing parameters");
	}

	wstring stockCode = this->jsonObj[L"stock_code"].as_string();


	if (stockCode.length() < 1 || stockCode.length() > 20)
	{
		throw BadRequestException("invalid parameters length");
	}


	if (!this->dbInstance->StockExists(stockCode))
	{
		throw BadRequestException("Can't find a stock with that code.");
	}

	wstring price = this->dbInstance->GetStockPrice(stockCode);

	json::value result;
	result[L"stock_price"] = json::value(price);
	return this->SendReply(true, L"ok", status_codes::OK, result);
}

bool CasablancaServer::Portfolio()
{
	TRACE("Calling Portfolio", 2);

	auto portfolioList = this->dbInstance->GetPortfolio(this->currentUserID);

	if (portfolioList.empty())
	{
		throw BadRequestException("Empty Portfolio.");
	}

	vector<json::value> result;
	wstring keys[] = { L"stock_code", L"quantity", L"stock_price" };
	for (int i = 0; i < portfolioList.size(); i++)
	{
		json::value obj;

		for (int j = 0; j < portfolioList[i].size(); j++)
			obj[keys[j]] = json::value(portfolioList[i][j]);

		result.push_back(obj);
	}

	json::value v = json::value::array(result);
	json::value obj = json::value::object();
	obj[L"portofolio"] = v;
	this->SendReply(true, L"ok", status_codes::OK, obj);
}

bool CasablancaServer::Transactions()
{
	TRACE("Calling Transactions", 2);

	auto transactionsList = this->dbInstance->GetTransactions(this->currentUserID);

	if (transactionsList.empty())
	{
		throw BadRequestException("No Transactions has been made yet.");
	}

	vector<json::value> result;
	wstring keys[] = { L"stock_code", L"quantity", L"transaction_time", L"transaction_type", L"price" };
	for (int i = 0; i < transactionsList.size(); i++)
	{
		json::value obj;

		for (int j = 0; j < transactionsList[i].size(); j++)
			obj[keys[j]] = json::value(transactionsList[i][j]);

		result.push_back(obj);
	}

	json::value v = json::value::array(result);
	json::value obj = json::value::object();
	obj[L"transactions"] = v;
	this->SendReply(true, L"ok", status_codes::OK, obj);
}

bool CasablancaServer::Register()
{
	TRACE("Calling Register", 2);

	if (!this->jsonObj.has_field(L"username")
		|| !this->jsonObj.has_field(L"password")
		|| !this->jsonObj.has_field(L"balance"))
	{
		throw BadRequestException("missing parameters");
	}

	wstring username = this->jsonObj[L"username"].as_string();
	wstring password = this->jsonObj[L"password"].as_string();
	wstring balance = this->jsonObj[L"balance"].as_string();

	if (username.length() < 4 || username.length() > 20 
		|| password.length() < 4 || password.length() > 20)
	{
		throw BadRequestException("invalid parameters length");
	}

		std::wstringstream ss;
		float fBalance = 0;
		ss << balance;
		ss >> fBalance;

		if (fBalance < 1000 || fBalance > 100000)
		{
			throw BadRequestException("invalid balance value");
		}

	if (this->dbInstance->UsernameExists(username))
	{
		throw BadRequestException("username already exists");
	}

	//hash password
	string hashed_password = sha1(Utility::WStringToString(password));
	bool userAdded = this->dbInstance->AddUser(username, Utility::StringToWString(hashed_password), balance);

	if (!userAdded)
	{
		throw BadRequestException("user is not added");
	}

	return this->SendReply(true, L"ok", status_codes::OK);
}

bool CasablancaServer::Login()
{
	//if reached here, you are authenticated
	TRACE("Calling Login", 2);
	return this->SendReply(true, L"ok", status_codes::OK);
}

bool CasablancaServer::HandleRequest(http_request request)
{
	TRACE("Calling HandleRequest", 2);
	try
	{
		this->request = request;

		if (!this->ValidateRequest())
			return false;

		if (!this->ParseRequestJson())
			return false;

		if (this->requestAction == L"register")
			return this->Register();

		if (!this->Authenticate())
			return false;

		if (this->requestAction == L"login")
			return this->Login();
		else if (this->requestAction == L"quote")
			return this->Quote();
		else if (requestAction == L"portofolio")
			return this->Portfolio();
		else if (requestAction == L"buy")
			return this->Buy();
		else if (requestAction == L"sell")
			return this->Sell();
		else if (requestAction == L"transactions")
			return this->Transactions();

		throw BadRequestException("Invalid Action");
	}
	catch (BadRequestException &e)
	{
		return this->SendReply(false, Utility::StringToWString(e.what()), status_codes::BadRequest);
	}
	catch (exception& e)
	{
		return this->SendReply(false, Utility::StringToWString(e.what()), status_codes::InternalError);

	}
}

bool CasablancaServer::Buy()
{
	TRACE("Calling Buy", 2);

	if (!this->jsonObj.has_field(L"stock_code")
		|| !this->jsonObj.has_field(L"stock_quantity")
		|| !this->jsonObj.has_field(L"stock_price"))
	{
		throw BadRequestException("missing parameters");
	}

	wstring stockCode = this->jsonObj[L"stock_code"].as_string();
	wstring stockQuantity = this->jsonObj[L"stock_quantity"].as_string();
	wstring stockPrice = this->jsonObj[L"stock_price"].as_string();
	

	if (stockCode.length() < 1 || stockCode.length() > 20
		|| stockQuantity.length() < 1 || stockQuantity.length() > 20
		|| stockPrice.length() < 3 || stockPrice.length() > 20)
	{
		throw BadRequestException("invalid parameters length");
	}

	std::wstringstream ss;
	float fPrice = 0;
	int iQuantity = 0;
	ss << stockPrice;
	ss >> fPrice;
	ss.clear();
	ss << stockQuantity;
	ss >> iQuantity;

	if (fPrice < 100 || fPrice > 100000 || iQuantity < 1 || iQuantity > 100000)
	{
		throw BadRequestException("invalid price or quantity value");
	}


	if (!this->dbInstance->StockExists(stockCode))
	{
		throw BadRequestException("Can't find a stock with that code.");
	}

	float traderBalance = this->dbInstance->GetTraderBalance(this->currentUserID);

	int quantity;
	float price;
	ss.clear();
	ss << stockQuantity.c_str();
	ss >> quantity;
	ss.clear();
	ss << stockPrice.c_str();
	ss >> price;

	if (traderBalance < (quantity * price))
	{
		throw BadRequestException("Insufficient Balance.");
	}

	float newBalance = traderBalance - (quantity * price);
	this->dbInstance->SetTraderBalance(this->currentUserID, newBalance);

	this->dbInstance->SetStockPrice(stockCode, stockPrice);
	this->dbInstance->AddToPortfolio(this->currentUserID, stockCode, stockQuantity);
	this->dbInstance->AddTransaction(this->currentUserID, stockCode, stockQuantity, L"buy", stockPrice);

	return this->SendReply(true, L"ok", status_codes::OK);
}

bool CasablancaServer::Sell()
{
	TRACE("Calling Sell", 2);

	if (!this->jsonObj.has_field(L"stock_code")
		|| !this->jsonObj.has_field(L"stock_quantity")
		|| !this->jsonObj.has_field(L"stock_price"))
	{
		throw BadRequestException("missing parameters");
	}

	wstring stockCode = this->jsonObj[L"stock_code"].as_string();
	wstring stockQuantity = this->jsonObj[L"stock_quantity"].as_string();
	wstring stockPrice = this->jsonObj[L"stock_price"].as_string();


	if (stockCode.length() < 1 || stockCode.length() > 20)
	{
		throw BadRequestException("invalid parameters length");
	}

	std::wstringstream ss;
	float fPrice = 0;
	int iQuantity = 0;
	ss << stockPrice;
	ss >> fPrice;
	ss.clear();
	ss << stockQuantity;
	ss >> iQuantity;

	if (fPrice < 100 || fPrice > 100000 || iQuantity < 1 || iQuantity > 100000)
	{
		throw BadRequestException("invalid price or quantity value");
	}

	if (!this->dbInstance->StockExists(stockCode))
	{
		throw BadRequestException("Can't find a stock with that code.");
	}

	int currentQuantity = this->dbInstance->GetStockQuantity(this->currentUserID, stockCode);

	int quantity;
	float price;
	ss.clear();
	ss << stockQuantity.c_str();
	ss >> quantity;
	ss.clear();
	ss << stockPrice.c_str();
	ss >> price;

	if (currentQuantity < quantity)
	{
		throw BadRequestException("Insufficient quantity.");
	}

	float currentBalance = this->dbInstance->GetTraderBalance(this->currentUserID);
	this->dbInstance->SetTraderBalance(this->currentUserID, currentBalance + (quantity*price));
	this->dbInstance->SetStockPrice(stockCode, stockPrice);
	this->dbInstance->RemoveFromPortfolio(this->currentUserID, stockCode, currentQuantity - quantity);
	this->dbInstance->AddTransaction(this->currentUserID, stockCode, stockQuantity, L"sell", stockPrice);

	return this->SendReply(true, L"ok", status_codes::OK);
}

