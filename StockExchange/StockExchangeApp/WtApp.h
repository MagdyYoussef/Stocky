
#pragma once

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WGroupBox>
#include <Wt/WPanel>
#include <Wt/WHBoxLayout>
#include <Wt/WTextArea>
#include <Wt/WDateTime>
#include <Wt/WBootstrapTheme>
#include <Wt/WCssTheme>
#include <Wt/WEnvironment>
#include <Wt/WTabWidget>
#include <Wt/WMenuItem>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
#include <Wt/WTemplate>
#include <Wt/WIntValidator>
#include <Wt/WDoubleValidator>
#include <Wt/WLengthValidator>
#include <Wt/WPopupMenu>
#include <Wt/WPopupMenu> 
#include <Wt/WNavigationBar>
#include <Wt/WMessageBox>

class RestClient;

class WtApp : public Wt::WApplication
{
private:
	RestClient* client;
	const int MAX_LENGTH = 20;
	const int MIN_LENGTH = 4;
	Wt::WBootstrapTheme bs_;
	Wt::WAnimation *animation;
	std::wstring username;
	std::wstring password;
	
	//Logout
	void ResetFields();
	void LogoutBtnClickHandler();

	//Dashboard Groups
	void InitializeGroups();
	Wt::WGridLayout *dashboardGrid;
	Wt::WGroupBox  *exchangePage;
	Wt::WGroupBox  *loginPage;

	//Nav Bar
	void InitializeNavigationBar();
	Wt::WContainerWidget *navContainer;
	Wt::WStackedWidget *navContents;
	Wt::WMenu  *leftMenu;
	Wt::WMenu *rightMenu;

	//Login
	void InitializeLogin();
	void LoginBtnClickHandler();
	bool ValidateLogin();
	Wt::WContainerWidget *loginWidget;
	Wt::WLineEdit *loginUsername;
	Wt::WLineEdit *loginPassword;
	Wt::WText *loginUsernameValMsg;
	Wt::WText *loginPasswordValMsg;
	Wt::WText *loginResponse;
	Wt::WPushButton *loginBtn;

	//Register
	void RegisterButtonClickHandler();
	bool ValidateRegistration();
	void InitializeRegister();
	Wt::WContainerWidget *regWidget;
	Wt::WLineEdit *registerUsername;
	Wt::WLineEdit *registerPassword;
	Wt::WLineEdit *registerbalance;
	Wt::WText *registerPasswordValMsg;
	Wt::WText *registerUsernameValMsg;
	Wt::WText *registerBalanceValMsg;
	Wt::WText *registerResponse;
	Wt::WPushButton *registerBtn;

	//Quote
	void InitializeQuote();
	void QouteButtonClickHandler();
	Wt::WLineEdit *qouteCode;
	Wt::WText *qouteCodeValMsg;
	Wt::WPushButton *qouteBtn;

	//Buy
	void BuyButtonClickHandler();
	void InitializeBuy();
	bool ValidateBuy();
	Wt::WLineEdit *buyCode;
	Wt::WText *buyCodeValMsg;
	Wt::WLineEdit *buyQty;
	Wt::WText *buyQtyValMsg;
	Wt::WLineEdit *buyPrice;
	Wt::WText *buyPriceValMsg;
	Wt::WPushButton *buyBtn;

	//Sell
	void InitializeSell();
	void SellButtonClickHandler();
	bool ValidateSell();
	Wt::WLineEdit *sellCode;
	Wt::WText *sellCodeValMsg;
	Wt::WLineEdit *sellQty;
	Wt::WText *sellQtyValMsg;
	Wt::WLineEdit *sellPrice;
	Wt::WText *sellPriceValMsg;
	Wt::WPushButton *sellBtn;

	//Portofolio
	Wt::WPanel *portofolioPanel;
	Wt::WTextArea* portfolioTextArea;
	void GetPortfolio();
	void InitializePortofolio();

	//Transactions
	Wt::WPanel *transactionsPanel;
	Wt::WTextArea* transactionsTextArea;
	void GetTransactions();
	void InitializeTransactions();

	//Response
	Wt::WContainerWidget *responseContainer;
	Wt::WTextArea *responseTextArea;
	void InitializeResponse();
	void WriteToTerminal(std::wstring message);

public:
	WtApp(const Wt::WEnvironment& env);
	~WtApp();
};
