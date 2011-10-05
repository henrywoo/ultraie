// UltraIE - Yet Another IE Add-on
// Copyright (C) 2006-2010
// Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
// Homepage: http://www.linkedin.com/in/simonwoo
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
