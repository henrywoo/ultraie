#ifndef __GETADDRESS__
#define __GETADDRESS__

#include <string>
using namespace std;

string GetUUGardenURL();
string GetFromOtherWebsite(const string& url);
string findaddress(const string& html);

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif
