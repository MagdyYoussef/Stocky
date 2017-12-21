

#include "RestClient.h"
#include "WtApp.h"

WtApp::WtApp(const Wt::WEnvironment& env) :Wt::WApplication(env)
{
	bs_.setVersion(Wt::WBootstrapTheme::Version3);
	bs_.setResponsive(true);
	setTheme(&bs_);
	setTitle("App");
	
	this->client = new RestClient();
	this->animation = new Wt::WAnimation(Wt::WAnimation::SlideInFromTop, Wt::WAnimation::EaseOut, 100);
	this->dashboardGrid = new Wt::WGridLayout();

	this->InitializeRegister();
	this->InitializeLogin();
	this->InitializeQuote();
	this->InitializeBuy();
	this->InitializeSell();
	this->InitializePortofolio();
	this->InitializeTransactions();
	this->InitializeResponse();
	this->InitializeGroups();
	this->InitializeNavigationBar();
}
WtApp::~WtApp()
{
	delete this->client;
}

void WtApp::WriteToTerminal(std::wstring message)
{
	std::wstring content = responseTextArea->valueText();
	responseTextArea->setText(content + L"\n" + message);
	this->GetPortfolio();
	this->GetTransactions();
}

//--------------------------------------------login-----------------------------------------------
bool WtApp::ValidateLogin()
{
	int valid = 0;
	if (!(loginUsername->validate() == Wt::WValidator::Valid)) {
		loginUsernameValMsg->setStyleClass("alert alert-danger");
		loginUsernameValMsg->show();
		valid++;
	}
	if (!(loginPassword->validate() == Wt::WValidator::Valid))
	{
		loginPasswordValMsg->setStyleClass("alert alert-danger");
		loginPasswordValMsg->show();
		valid++;
	}
	return valid > 0 ? false : true;
}
void WtApp::LoginBtnClickHandler()
{
	loginUsernameValMsg->hide();
	loginPasswordValMsg->hide();
	bool valid = false;
	valid = ValidateLogin();

	if (valid)
	{
		this->username = loginUsername->text();
		this->password = loginPassword->text();
		std::wstring result = client->Login(this->username, this->password);

		if (result == L"logged in successfully")
		{
			leftMenu->setItemHidden(1, false);
			navContents->setCurrentWidget(exchangePage);
			leftMenu->setItemHidden(0, true);
			rightMenu->setItemHidden(0, false);
			this->GetPortfolio();
			this->GetTransactions();
		}
		else
		{
			loginResponse->setText("Username and password don't match");
			loginResponse->setStyleClass("alert alert-danger");
			loginResponse->show();
		}
	}
}
void WtApp::InitializeLogin()
{
	this->loginWidget = new Wt::WContainerWidget();
	Wt::WVBoxLayout *loginbox = new Wt::WVBoxLayout();
	loginbox->addWidget(new Wt::WText("<legend>Login</legend>"));
	loginUsername = new Wt::WLineEdit();
	loginPassword = new Wt::WLineEdit();
	loginPassword->setEchoMode(Wt::WLineEdit::Password);
	Wt::WLengthValidator *loginUsernameVal = new Wt::WLengthValidator();
	loginUsernameVal->setMandatory(true);
	loginUsernameVal->setMinimumLength(MIN_LENGTH);
	loginUsernameVal->setMaximumLength(MAX_LENGTH);
	loginUsername->setValidator(loginUsernameVal);
	this->loginUsernameValMsg = new Wt::WText("Username must be between 4 and 20 characters");
	loginUsernameValMsg->setInline(false);
	loginUsernameValMsg->hide();
	Wt::WLengthValidator *loginPasswordVal = new Wt::WLengthValidator();
	loginPasswordVal->setMandatory(true);
	loginPasswordVal->setMinimumLength(MIN_LENGTH);
	loginPasswordVal->setMaximumLength(MAX_LENGTH);
	loginPassword->setValidator(loginPasswordVal);
	this->loginPasswordValMsg = new Wt::WText("Password must be between 4 and 20 characters");
	loginPasswordValMsg->setInline(false);
	loginPasswordValMsg->hide();
	loginbox->addWidget(new Wt::WText("Username:"));
	loginbox->addWidget(loginUsername);
	loginbox->addWidget(loginUsernameValMsg);
	loginbox->addWidget(new Wt::WText("Password:"));
	loginbox->addWidget(loginPassword);
	loginbox->addWidget(loginPasswordValMsg);
	loginBtn = new Wt::WPushButton("Login", root());
	loginBtn->clicked().connect(this, &WtApp::LoginBtnClickHandler);
	loginBtn->setStyleClass("btn btn-primary");
	loginbox->addWidget(loginBtn);
	loginResponse = new Wt::WText("");
	loginbox->addWidget(loginResponse);
	loginWidget->setLayout(loginbox);


}

//--------------------------------------------register--------------------------------------------
bool WtApp::ValidateRegistration()
{
	int valid = 0;
	if (!(registerUsername->validate() == Wt::WValidator::Valid)) {
		registerUsernameValMsg->setStyleClass("alert alert-danger");
		registerUsernameValMsg->show();
		valid++;
	}
	if (!(registerPassword->validate() == Wt::WValidator::Valid))
	{
		registerPasswordValMsg->setStyleClass("alert alert-danger");
		registerPasswordValMsg->show();
		valid++;
	}
	if (!(registerbalance->validate() == Wt::WValidator::Valid))
	{
		registerBalanceValMsg->setStyleClass("alert alert-danger");
		registerBalanceValMsg->show();
		valid++;
	}
	return valid > 0 ? false : true;
}
void WtApp::RegisterButtonClickHandler()
{
	registerUsernameValMsg->hide();
	registerPasswordValMsg->hide();
	registerBalanceValMsg->hide();
	bool valid = false;
	valid = ValidateRegistration();

	if (valid) 
	{
		registerResponse->hide();
		std::wstring response = this->client->Register(registerUsername->text(), registerPassword->text(), registerbalance->text());

		registerResponse->setText(response);
		
		if (response == L"Registered successfully")
			registerResponse->setStyleClass("alert alert-success");
		else
			registerResponse->setStyleClass("alert alert-danger");

		registerResponse->show();
	}
}
void WtApp::InitializeRegister()
{
	this->regWidget = new Wt::WContainerWidget();
	Wt::WVBoxLayout *registerbox = new Wt::WVBoxLayout();
	registerbox->addWidget(new Wt::WText("<legend>Register</legend>"));
	registerUsername = new Wt::WLineEdit();
	Wt::WLengthValidator *registerUsernameVal = new Wt::WLengthValidator();
	registerUsernameVal->setMandatory(true);
	registerUsernameVal->setMinimumLength(MIN_LENGTH);
	registerUsernameVal->setMaximumLength(MAX_LENGTH);
	registerUsername->setValidator(registerUsernameVal);
	this->registerUsernameValMsg = new Wt::WText("Username must be between 4 and 20 characters");
	registerUsernameValMsg->setInline(false);
	registerUsernameValMsg->hide();
	registerPassword = new Wt::WLineEdit();
	registerPassword->setEchoMode(Wt::WLineEdit::Password);
	Wt::WLengthValidator *registerPasswordVal = new Wt::WLengthValidator();
	registerPasswordVal->setMandatory(true);
	registerPasswordVal->setMinimumLength(MIN_LENGTH);
	registerPasswordVal->setMaximumLength(MAX_LENGTH);
	registerPassword->setValidator(registerPasswordVal);
	this->registerPasswordValMsg = new Wt::WText("Password must be between 4 and 20 characters");
	registerPasswordValMsg->setInline(false);
	registerPasswordValMsg->hide();
	registerbalance = new Wt::WLineEdit();
	Wt::WPushButton *registerBtn = new Wt::WPushButton("register", root());
	Wt::WTemplate *registrationTemplate = new Wt::WTemplate(Wt::WString::tr("validation-template"));
	registrationTemplate->addFunction("id", &Wt::WTemplate::Functions::id);
	registrationTemplate->bindWidget("balance", registerbalance);
	registrationTemplate->bindWidget("registerBtn", registerBtn);
	Wt::WIntValidator *registerBalanceVal = new Wt::WIntValidator(1000, 100000);
	registerBalanceVal->setMandatory(true);
	registerbalance->setValidator(registerBalanceVal);
	this->registerBalanceValMsg = new Wt::WText("Balance must be between 1000 and 100000");
	registerBalanceValMsg->setInline(false);
	registerBalanceValMsg->hide();
	registrationTemplate->bindWidget("registerBalanceValMsg", registerBalanceValMsg);
	registerResponse = new Wt::WText();
	registrationTemplate->bindWidget("registerResponse", registerResponse);
	registerbox->addWidget(new Wt::WText("Username:"));
	registerbox->addWidget(registerUsername);
	registerbox->addWidget(registerUsernameValMsg);
	registerbox->addWidget(new Wt::WText("Password:"));
	registerbox->addWidget(registerPassword);
	registerbox->addWidget(registerPasswordValMsg);
	registerbox->addWidget(new Wt::WText("Balance:"));
	registerbox->addWidget(registerbalance);
	registerbox->addWidget(registerBalanceValMsg);
	registerBtn->setStyleClass("btn btn-primary");
	regWidget->setLayout(registerbox);
	registerBtn->clicked().connect(this, &WtApp::RegisterButtonClickHandler);
	registerbox->addWidget(registerBtn);
	registerbox->addWidget(registerResponse);
}

//--------------------------------------------quote-----------------------------------------------
void WtApp::QouteButtonClickHandler()
{
	qouteCodeValMsg->hide();
	if (!(qouteCode->validate() == Wt::WValidator::Valid)) {
		qouteCodeValMsg->setStyleClass("alert alert-danger");
		qouteCodeValMsg->show();
	}
	else
	{
		qouteBtn->disable();
		std::wstring v = this->client->Quote(this->username, this->password, qouteCode->text());
		qouteBtn->enable();
		this->WriteToTerminal(L"current price is: " + v);
	}
}
void WtApp::InitializeQuote()
{
	Wt::WContainerWidget *qouteWidget = new Wt::WContainerWidget();
	qouteWidget->addWidget(new Wt::WText("<b>Dashboard</b>"));
	Wt::WVBoxLayout  *qoutehbox = new Wt::WVBoxLayout();
	qouteCode = new Wt::WLineEdit();
	Wt::WLengthValidator *qouteUsernameVal = new Wt::WLengthValidator();
	qouteUsernameVal->setMandatory(true);
	qouteUsernameVal->setMinimumLength(1);
	qouteUsernameVal->setMaximumLength(MAX_LENGTH);
	qouteCode->setValidator(qouteUsernameVal);
	this->qouteCodeValMsg = new Wt::WText("Code is invalid");
	qouteCodeValMsg->setInline(false);
	qouteCodeValMsg->hide();
	qouteBtn = new Wt::WPushButton("Qoute", root());
	qouteBtn->clicked().connect(this, &WtApp::QouteButtonClickHandler);
	qouteBtn->setStyleClass("btn btn-primary");
	qoutehbox->addWidget(new Wt::WText("Code:"));
	qoutehbox->addWidget(qouteCode);
	qoutehbox->addWidget(qouteCodeValMsg);
	qoutehbox->addWidget(qouteBtn);
	qouteWidget->setLayout(qoutehbox);
	dashboardGrid->addWidget(qouteWidget, 1, 0);
}

//--------------------------------------------buy-------------------------------------------------
bool WtApp::ValidateBuy()
{
	int valid = 0;
	if (!(buyCode->validate() == Wt::WValidator::Valid)) {
		buyCodeValMsg->setStyleClass("alert alert-danger");
		buyCodeValMsg->show();
		valid++;
	}
	if (!(buyQty->validate() == Wt::WValidator::Valid))
	{
		buyQtyValMsg->setStyleClass("alert alert-danger");
		buyQtyValMsg->show();
		valid++;
	}
	if (!(buyPrice->validate() == Wt::WValidator::Valid))
	{
		buyPriceValMsg->setStyleClass("alert alert-danger");
		buyPriceValMsg->show();
		valid++;
	}
	return valid > 0 ? false : true;
}
void WtApp::InitializeBuy()
{
	Wt::WContainerWidget *buyWidget = new Wt::WContainerWidget();
	Wt::WVBoxLayout *buyhbox = new Wt::WVBoxLayout();
	buyCode = new Wt::WLineEdit();
	buyQty = new Wt::WLineEdit();
	buyPrice = new Wt::WLineEdit();
	buyBtn = new Wt::WPushButton("Buy", root());
	buyBtn->setStyleClass("btn btn-primary");

	Wt::WTemplate *buyTemplate = new Wt::WTemplate(Wt::WString::tr("validation-template"));
	buyTemplate->addFunction("buyid", &Wt::WTemplate::Functions::id);
	buyTemplate->bindWidget("buyCode", buyCode);
	buyTemplate->bindWidget("buyQty", buyQty);
	buyTemplate->bindWidget("buyPrice", buyPrice);
	buyTemplate->bindWidget("buyBtn", buyBtn);

	Wt::WLengthValidator *buyCodeVal = new Wt::WLengthValidator();
	buyCodeVal->setMandatory(true);
	buyCodeVal->setMinimumLength(1);
	buyCodeVal->setMaximumLength(MAX_LENGTH);
	buyCode->setValidator(buyCodeVal);
	this->buyCodeValMsg = new Wt::WText("Code is invalid");
	buyCodeValMsg->setInline(false);
	buyCodeValMsg->hide();

	Wt::WIntValidator *buyQtyVal = new Wt::WIntValidator(1, 100000);
	buyQtyVal->setMandatory(true);
	buyQty->setValidator(buyQtyVal);
	this->buyQtyValMsg = new Wt::WText("Quantity must be between 1 and 100000");
	buyQtyValMsg->setInline(false);
	buyQtyValMsg->hide();
	buyTemplate->bindWidget("buyValMsg", buyQtyValMsg);

	Wt::WIntValidator *buyPriceVal = new Wt::WIntValidator(100, 100000);
	buyPriceVal->setMandatory(true);
	buyPrice->setValidator(buyPriceVal);
	this->buyPriceValMsg = new Wt::WText("Price must be between 100 and 100000");
	buyPriceValMsg->setInline(false);
	buyPriceValMsg->hide();
	buyTemplate->bindWidget("buyPriceValMsg", buyPriceValMsg);

	buyhbox->addWidget(new Wt::WText("Code:"));
	buyhbox->addWidget(buyCode);
	buyhbox->addWidget(buyCodeValMsg);
	buyhbox->addWidget(new Wt::WText("Quantity:"));
	buyhbox->addWidget(buyQty);
	buyhbox->addWidget(buyQtyValMsg);
	buyhbox->addWidget(new Wt::WText("Price:"));
	buyhbox->addWidget(buyPrice);
	buyhbox->addWidget(buyPriceValMsg);
	buyhbox->addWidget(buyBtn);
	buyBtn->clicked().connect(this, &WtApp::BuyButtonClickHandler);
	buyWidget->setLayout(buyhbox);
	dashboardGrid->addWidget(buyWidget, 2, 0);
}
void WtApp::BuyButtonClickHandler()
{
	buyCodeValMsg->hide();
	buyPriceValMsg->hide();
	buyQtyValMsg->hide();
	bool valid = false;
	valid = ValidateBuy();
	if (valid)
	{
		buyBtn->disable();
		std::wstring v = this->client->Buy(this->username, this->password, buyCode->text(), buyQty->text(), buyPrice->text());
		buyBtn->enable();
		this->WriteToTerminal(v);
	}
}

//--------------------------------------------sell-----------------------------------------------
void WtApp::InitializeSell()
{
	Wt::WContainerWidget *sellWidget = new Wt::WContainerWidget();
	Wt::WVBoxLayout *sellhbox = new Wt::WVBoxLayout();
	sellCode = new Wt::WLineEdit();
	sellQty = new Wt::WLineEdit();
	sellPrice = new Wt::WLineEdit();
	sellBtn = new Wt::WPushButton("sell", root());

	Wt::WTemplate *sellTemplate = new Wt::WTemplate(Wt::WString::tr("validation-template"));
	sellTemplate->addFunction("sellid", &Wt::WTemplate::Functions::id);
	sellTemplate->bindWidget("sellCode", sellCode);
	sellTemplate->bindWidget("sellQty", sellQty);
	sellTemplate->bindWidget("sellPrice", sellPrice);
	sellTemplate->bindWidget("sellBtn", sellBtn);

	Wt::WLengthValidator *sellCodeVal = new Wt::WLengthValidator();
	sellCodeVal->setMandatory(true);
	sellCodeVal->setMinimumLength(1);
	sellCodeVal->setMaximumLength(MAX_LENGTH);
	sellCode->setValidator(sellCodeVal);
	this->sellCodeValMsg = new Wt::WText("Code is invalid");
	sellCodeValMsg->setInline(false);
	sellCodeValMsg->hide();

	Wt::WIntValidator *sellQtyVal = new Wt::WIntValidator(1, 100000);
	sellQtyVal->setMandatory(true);
	sellQty->setValidator(sellQtyVal);
	this->sellQtyValMsg = new Wt::WText("Quantity must be between 1 and 100000");
	sellQtyValMsg->setInline(false);
	sellQtyValMsg->hide();
	sellTemplate->bindWidget("sellQtyValMsg", sellQtyValMsg);

	Wt::WIntValidator *sellPriceVal = new Wt::WIntValidator(100, 100000);
	sellPriceVal->setMandatory(true);
	sellPrice->setValidator(sellPriceVal);
	this->sellPriceValMsg = new Wt::WText("Price must be between 100 and 100000");
	sellPriceValMsg->setInline(false);
	sellPriceValMsg->hide();
	sellTemplate->bindWidget("sellPriceValMsg", sellPriceValMsg);

	sellhbox->addWidget(new Wt::WText("Code:"));
	sellhbox->addWidget(sellCode);
	sellhbox->addWidget(sellCodeValMsg);
	sellhbox->addWidget(new Wt::WText("Quantity:"));
	sellhbox->addWidget(sellQty);
	sellhbox->addWidget(sellQtyValMsg);
	sellhbox->addWidget(new Wt::WText("Price:"));
	sellhbox->addWidget(sellPrice);
	sellhbox->addWidget(sellPriceValMsg);
	sellhbox->addWidget(sellBtn);
	sellBtn->clicked().connect(this, &WtApp::SellButtonClickHandler);
	sellBtn->setStyleClass("btn btn-primary");
	sellWidget->setLayout(sellhbox);
	dashboardGrid->addWidget(sellWidget, 3, 0);
}
bool WtApp::ValidateSell()
{
	int valid = 0;
	if (!(sellCode->validate() == Wt::WValidator::Valid)) {
		sellCodeValMsg->setStyleClass("alert alert-danger");
		sellCodeValMsg->show();
		valid++;
	}
	if (!(sellQty->validate() == Wt::WValidator::Valid))
	{
		sellQtyValMsg->setStyleClass("alert alert-danger");
		sellQtyValMsg->show();
		valid++;
	}
	if (!(sellPrice->validate() == Wt::WValidator::Valid))
	{
		sellPriceValMsg->setStyleClass("alert alert-danger");
		sellPriceValMsg->show();
		valid++;
	}
	return valid > 0 ? false : true;
}
void WtApp::SellButtonClickHandler()
{
	sellCodeValMsg->hide();
	sellPriceValMsg->hide();
	sellQtyValMsg->hide();
	bool valid = false;
	valid = ValidateSell();
	if (valid)
	{
		sellBtn->disable();
		std::wstring v = this->client->Sell(this->username, this->password, sellCode->text(), sellQty->text(), sellPrice->text());
		sellBtn->enable();
		this->WriteToTerminal(v);
	}
}

//--------------------------------------------transactions----------------------------------------
void WtApp::GetTransactions()
{
	std::wstring transactions = this->client->Transactions(this->username, this->password);
	this->transactionsTextArea->setText(transactions);
}
void WtApp::InitializeTransactions()
{
	this->transactionsPanel = new Wt::WPanel();
	transactionsPanel->setTitle("Transactions");
	transactionsPanel->addStyleClass("centered-example");
	transactionsPanel->setCollapsible(true);
	transactionsPanel->setAnimation(*this->animation);
	Wt::WGroupBox* transactionsBox = new Wt::WGroupBox();
	transactionsPanel->setCentralWidget(transactionsBox);
	transactionsBox->addStyleClass("centered-example");
	this->transactionsTextArea = new Wt::WTextArea("", transactionsBox);
	this->transactionsTextArea->disable();

}

//--------------------------------------------portofolio------------------------------------------
void WtApp::InitializePortofolio()
{
	this->portofolioPanel = new Wt::WPanel();
	portofolioPanel->setTitle("Portofolio");
	portofolioPanel->addStyleClass("centered-example");
	portofolioPanel->setCollapsible(true);
	portofolioPanel->setAnimation(*this->animation);
	Wt::WGroupBox* portofolioBox = new Wt::WGroupBox("");
	portofolioPanel->setCentralWidget(portofolioBox);
	portofolioBox->addStyleClass("centered-example");
	root()->addWidget(portofolioPanel);
	this->portfolioTextArea = new Wt::WTextArea("", portofolioBox);
	this->portfolioTextArea->disable();
}
void WtApp::GetPortfolio()
{
	std::wstring portfolio = this->client->Portofolio(this->username, this->password);
	this->portfolioTextArea->setText(portfolio);
}


void WtApp::InitializeResponse()
{
	this->responseContainer = new Wt::WContainerWidget();
	responseContainer->setOverflow(Wt::WContainerWidget::Overflow::OverflowAuto);
	responseTextArea = new Wt::WTextArea(responseContainer);
	responseTextArea->setColumns(185);
	responseTextArea->disable();
	responseTextArea->setRows(30);
	responseTextArea->setMinimumSize(280, 600);
	responseTextArea->setText("Ready ... ");
	Wt::WText *response = new Wt::WText("<p></p>", responseContainer);
	response->addStyleClass("help-block");
}

void WtApp::InitializeNavigationBar()
{
	Wt::WNavigationBar *navigation = new Wt::WNavigationBar(root());
	navigation->setTitle("Fast Exchange");
	navigation->setResponsive(true);
	navContents = new Wt::WStackedWidget(root());
	navContents->addWidget(loginPage);
	navContents->addWidget(exchangePage);
	navContents->addStyleClass("contents");
	leftMenu = new Wt::WMenu(navContents, root());
	navigation->addMenu(leftMenu);
	leftMenu->addItem("Login", loginPage);
	leftMenu->addItem("Exchange", exchangePage);
	rightMenu = new Wt::WMenu();
	navigation->addMenu(rightMenu, Wt::AlignRight);
	Wt::WPopupMenu *popup = new Wt::WPopupMenu();
	popup->addItem("Logout")->triggered().connect(this, &WtApp::LogoutBtnClickHandler);
	Wt::WMenuItem *item = new Wt::WMenuItem("Options");
	item->setMenu(popup);
	rightMenu->addItem(item);
	navigation->addMenu(rightMenu);
	navContents->setCurrentWidget(loginPage);
	leftMenu->select(0);
	leftMenu->setItemHidden(1, true);
	rightMenu->setItemHidden(0, true);
	root()->addWidget(navContents);
}

void WtApp::LogoutBtnClickHandler()
{
	this->ResetFields();
	leftMenu->setItemHidden(0, false);
	navContents->setCurrentWidget(loginPage);
	leftMenu->setItemHidden(1, true);
	rightMenu->setItemHidden(0, true);
}

void WtApp::InitializeGroups()
{
	loginPage = new Wt::WGroupBox();
	Wt::WVBoxLayout *loginPageLayout = new 	Wt::WVBoxLayout();
	this->loginWidget->setWidth(500);
	this->loginWidget->setMargin(440, Wt::Side::Left);
	this->loginWidget->setMargin(440, Wt::Side::Right);
	regWidget->setWidth(500);
	regWidget->setMargin(440, Wt::Side::Left);
	regWidget->setMargin(440, Wt::Side::Right);
	loginPageLayout->addWidget(loginWidget);
	loginPageLayout->addWidget(regWidget);
	loginPage->setLayout(loginPageLayout);

	exchangePage = new Wt::WGroupBox();
	Wt::WContainerWidget *exchangeContainer = new Wt::WContainerWidget();
	dashboardGrid->addWidget(this->responseContainer, 1, 2, 5, 2);
	exchangeContainer->setStyleClass("centered");
	exchangeContainer->setLayout(dashboardGrid);
	Wt::WContainerWidget *historyContainer = new Wt::WContainerWidget();
	historyContainer->setStyleClass("centered");
	Wt::WVBoxLayout *historyvbox = new Wt::WVBoxLayout();
	historyvbox->addWidget(this->portofolioPanel);
	historyvbox->addWidget(this->transactionsPanel);
	historyContainer->setLayout(historyvbox);
	exchangeContainer->setMargin(200, Wt::Side::Left);
	exchangeContainer->setMargin(200, Wt::Side::Right);
	historyContainer->setMargin(200, Wt::Side::Left);
	historyContainer->setMargin(200, Wt::Side::Right);
	exchangePage->addWidget(exchangeContainer);
	exchangePage->addWidget(historyContainer);
}

void WtApp::ResetFields()
{
	username.empty();
	password.empty();

	//Login 
	loginUsername->setText("");
	loginPassword->setText("");
	loginUsernameValMsg->hide();
	loginPasswordValMsg->hide();
	loginResponse->hide();

	//Register  
	registerUsername->setText("");
	registerPassword->setText("");
	registerbalance->setText("");
	registerPasswordValMsg->hide();
	registerUsernameValMsg->hide();
	registerBalanceValMsg->hide();
	registerResponse->hide();

	//Quote 
	qouteCode->setText("");
	qouteCodeValMsg->hide();

	//Buy 
	buyCode->setText("");
	buyCodeValMsg->hide();
	buyQty->setText("");
	buyQtyValMsg->hide();
	buyPrice->setText("");
	buyPriceValMsg->hide();

	//Sell 
	sellCode->setText("");
	sellCodeValMsg->hide();
	sellQty->setText("");
	sellQtyValMsg->hide();
	sellPrice->setText("");
	sellPriceValMsg->hide();

	//Portofolio  
	portfolioTextArea->setText("");

	//Transactions
	transactionsTextArea->setText("");

	//Response 
	responseTextArea->setText("Ready ... ");

}

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	return new WtApp(env);
}
int main(int argc, char **argv)
{
	return Wt::WRun(argc, argv, &createApplication);
}