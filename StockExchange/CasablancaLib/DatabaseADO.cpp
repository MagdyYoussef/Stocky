
#include "Utility.h"
#include "DatabaseADO.h"

DatabaseADO::DatabaseADO(std::string sqlServer, std::string database, std::string username, std::string password)
{
	try
	{
		CoInitialize(NULL);
		cn.CreateInstance("ADODB.Connection");
		rs.CreateInstance("ADODB.Recordset");
		std::string connectionString = "PROVIDER=SQLOLEDB;SERVER=" + sqlServer + ";DATABASE=" + database + ";";
		cn->Open(connectionString.c_str(), username.c_str(), password.c_str(), adConnectUnspecified);
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}

}

DatabaseADO::~DatabaseADO()
{
	try
	{
		if (rs->State != 0)
			rs->Close();

		if (cn->State != 0)
			cn->Close();

		rs = NULL;
		cn = NULL;
		CoUninitialize();
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

bool DatabaseADO::UsernameExists(std::wstring& username)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec UserExists @username = '%s';", username.c_str());
		rs = cn->Execute(sqlQuery, NULL, adCmdText);

		bool isFound = false;

		if (!rs->adoEOF)
		{
			_variant_t var = rs->Fields->GetItem(0L)->Value;
			isFound = (bool)var;
		}

		return isFound;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

bool DatabaseADO::AddUser(std::wstring& username, std::wstring& password, std::wstring& balance)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec AddTrader @username = '%s', @password = '%s', @balance = '%s';", username.c_str(), password.c_str(), balance.c_str());
		variant_t recordsAffected;
		cn->Execute(sqlQuery, &recordsAffected, adCmdText);
		return recordsAffected.intVal;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
		return false;
	}
}

std::wstring DatabaseADO::GetStockPrice(std::wstring& stockCode)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec GetStockPrice @stockCode = '%s';", stockCode.c_str());
		rs = cn->Execute(sqlQuery, NULL, adCmdText);

		std::wstring price = L"0";

		if (!rs->adoEOF)
		{
			_variant_t var = rs->Fields->GetItem(0L)->Value;
			price = (_bstr_t)var;
		}

		return price;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

int DatabaseADO::VerifyUserLogin(std::wstring& username, std::wstring& password)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec VerifyUser @username = '%s', @password = '%s';", username.c_str(), password.c_str());
		rs = cn->Execute(sqlQuery, NULL, adCmdText);

		int userID = 0;

		if (!rs->adoEOF)
			userID = rs->Fields->GetItem(0L)->Value.intVal;

		return userID;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

std::vector< std::vector<std::wstring> > DatabaseADO::GetPortfolio(int& userID)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec GetPortofolio @userID = %i;", userID);
		rs = cn->Execute(sqlQuery, NULL, adCmdText);

		std::vector< std::vector<std::wstring> > result;
		while (!rs->adoEOF)
		{
			std::vector<std::wstring> row;
			for (int i = 0; i < rs->Fields->GetCount(); i++)
			{
				_variant_t var = rs->Fields->GetItem((long)i)->Value;
				std::wstring val = (_bstr_t)var;
				row.push_back(val);
			}
			result.push_back(row);
			rs->MoveNext();
		}
		return result;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

std::vector< std::vector<std::wstring> > DatabaseADO::GetTransactions(int& userID)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec GetTransactions @userID = %i;", userID);
		rs = cn->Execute(sqlQuery, NULL, adCmdText);

		std::vector< std::vector<std::wstring> > result;
		while (!rs->adoEOF)
		{
			std::vector<std::wstring> row;
			for (int i = 0; i < rs->Fields->GetCount(); i++)
			{
				_variant_t var = rs->Fields->GetItem((long)i)->Value;
				std::wstring val = (_bstr_t)var;
				row.push_back(val);
			}
			result.push_back(row);
			rs->MoveNext();
		}
		return result;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

float DatabaseADO::GetTraderBalance(int& userID)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec GetTraderBalance @userID = %i;", userID);
		rs = cn->Execute(sqlQuery, NULL, adCmdText);

		float balance = 0;

		if (!rs->adoEOF)
		{
			_variant_t var = rs->Fields->GetItem(0L)->Value;
			balance = (float)var;
		}

		return balance;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

bool DatabaseADO::SetTraderBalance(int& userID, float balance)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec SetTraderBalance @userID = %i, @balance = %f;", userID, balance);
		variant_t recordsAffected;
		cn->Execute(sqlQuery, &recordsAffected, adCmdText);
		return recordsAffected.intVal;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

bool DatabaseADO::SetStockPrice(std::wstring& stockCode, std::wstring& stockPrice)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec SetStockPrice @stockCode = '%s', @stockPrice = %s;", stockCode.c_str(), stockPrice.c_str());
		variant_t recordsAffected;
		cn->Execute(sqlQuery, &recordsAffected, adCmdText);
		return recordsAffected.intVal;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}


bool DatabaseADO::AddToPortfolio(int& userID, std::wstring& stockCode, std::wstring& stockQuantity)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec AddToPortfolio @userID = %i, @stockCode = %s, @stockQuantity = %s;", userID, stockCode.c_str(), stockQuantity.c_str());
		variant_t recordsAffected;
		cn->Execute(sqlQuery, &recordsAffected, adCmdText);
		return recordsAffected.intVal;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

bool DatabaseADO::StockExists(std::wstring& stockCode)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec StockExists @stockCode = '%s';", stockCode);
		rs = cn->Execute(sqlQuery, NULL, adCmdText);

		bool isFound = false;

		if (!rs->adoEOF)
			isFound = rs->Fields->GetItem(0L)->Value.boolVal;

		return isFound;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

bool DatabaseADO::AddTransaction(int& userID, std::wstring& stockCode, std::wstring& stockQuantity, std::wstring transactionType, std::wstring& price)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec AddTransaction @userID = %i, @stockCode = '%s', @stockQuantity = '%s', @transactionType = '%s', @price = %s;", userID, stockCode.c_str(), stockQuantity.c_str(), transactionType.c_str(), price.c_str());
		variant_t recordsAffected;
		cn->Execute(sqlQuery, &recordsAffected, adCmdText);
		return recordsAffected.intVal;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

int DatabaseADO::GetStockQuantity(int& userID, std::wstring& stockCode)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec GetStockQuantity @userID = %i, @stockCode = '%s';", userID, stockCode.c_str());
		rs = cn->Execute(sqlQuery, NULL, adCmdText);
		int quantity = 0;
		
		if (!rs->adoEOF)
			quantity = rs->Fields->GetItem(0L)->Value.intVal;
		
		return quantity;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}

bool DatabaseADO::RemoveFromPortfolio(int& userID, std::wstring& stockCode, int stockQuantity)
{
	try
	{
		wchar_t sqlQuery[200];
		swprintf_s(sqlQuery, L"exec RemoveFromPortfolio @userID = %i, @stockCode = %s, @stockQuantity = %i;", userID, stockCode.c_str(), stockQuantity);
		variant_t recordsAffected;
		cn->Execute(sqlQuery, &recordsAffected, adCmdText);
		return recordsAffected.intVal;
	}
	catch (_com_error & ce)
	{
		TRACE(ce.Description(), 1);
	}
}
