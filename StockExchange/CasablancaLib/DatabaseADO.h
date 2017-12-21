
#pragma once
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")

class DatabaseADO
{
private:
	_ConnectionPtr  cn;
	_RecordsetPtr   rs;
public:
	DatabaseADO(std::string sqlServer, std::string database, std::string username, std::string password);
	~DatabaseADO();

	//traders
	int VerifyUserLogin(std::wstring& username, std::wstring& password);
	bool UsernameExists(std::wstring& username);
	bool AddUser(std::wstring& username, std::wstring& password, std::wstring& balance);
	float GetTraderBalance(int& userID);
	bool SetTraderBalance(int& userID, float balance);

	//stocks
	bool StockExists(std::wstring& stockCode);
	std::wstring GetStockPrice(std::wstring& stockCode);
	bool SetStockPrice(std::wstring& stockCode, std::wstring& stockPrice);
	
	//portofolio
	int GetStockQuantity(int& userID, std::wstring& stockCode);
	std::vector< std::vector<std::wstring> > GetPortfolio(int& userID);
	bool RemoveFromPortfolio(int& userID, std::wstring& stockCode, int stockQuantity);
	bool AddToPortfolio(int& userID, std::wstring& stockCode, std::wstring& stockQuantity);

	//transactions
	bool AddTransaction(int& userID, std::wstring& stockCode, std::wstring& stockQuantity, std::wstring transactionType, std::wstring& price);

	std::vector< std::vector<std::wstring> > GetTransactions(int& userID); 

};