#include <Ice/Ice.h>
#include <Ice/Service.h>
#include <uuice.h>
#include <serverimpl.h>
#include <constdef.h>

using namespace std;
using namespace UUTOOLBAR;

Ice::LoggerPtr plogger;

class MyService : public Ice::Service {
protected:
	virtual bool start(int, char*[]);
private:
	Ice::ObjectAdapterPtr _adapter;
};

bool MyService::start(int argc, char* argv[]) {
	_adapter = communicator()->createObjectAdapterWithEndpoints(SVRADAPTER,URL);
	Ice::ObjectPtr object = new UserI;
	plogger = communicator()->getLogger();
	_adapter->add(object, communicator()->stringToIdentity(SVRID));
	_adapter->activate();
	return true;
}

int main(int argc, char* argv[])
{
	MyService svc;
	return svc.main(argc, argv);
}
