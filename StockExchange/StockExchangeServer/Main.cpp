
#include "Utility.h"
#include "CasablacaServer.h"

CasablancaServer serverInstance(DATABASE_SERVER, DATABASE_PRODUCTION, DATABASE_USERNAME, DATABASE_PASSWORD);

void HandleRequest(http_request request)
{
	serverInstance.HandleRequest(request);
}

int main()
{
	TRACE("Calling Main", 2);

	try
	{
		http_listener listener(L"http://localhost/terminal");
		listener.support(HandleRequest);
		listener.open();
		TRACE("*** Server Started ***", 1);
		TRACE("Waiting for requests...", 1);
		while (true);
	}
	catch (std::exception& e)
	{
		TRACE(e.what(), 1);
		return -1;
	}
	return 0;
}