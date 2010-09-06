# make file of uuiced

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0
OBJS     =	uuiced.o uuice.o
LIBS     =  -L /usr/lib -lIce -lIceUtil -L /usr/lib/mysql/ -lmysqlclient -lboost_filesystem -L /usr/local/Geoip145/lib/ -lGeoIP -lpthread
TARGET   =	uuiced
HEADER   =  constdef.h serverimpl.h uuice.h


$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

uuiced.o uuice.o: $(HEADER)
	slice2cpp uuice.ice
	$(CXX) -c -g -I /usr/local/include -I . -I /usr/include/mysql/ -I /usr/include/ -I /usr/local/Geoip145/include/ uuiced.cpp
	$(CXX) -c -g -I /usr/local/include -I . -I /usr/include/ uuice.cpp

clean:
	rm -f $(OBJS) $(TARGET)
