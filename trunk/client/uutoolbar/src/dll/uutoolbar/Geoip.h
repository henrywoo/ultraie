#pragma once
#include "CHttpSocket.h"

//不知道为什么好像已经包含了<<winsock2.h>>

#ifndef _WINSOCK2API_ // 没有包含winsock2.h
#define _WINSOCK2API_ // 避免再包含winsock2.h
#include <winsock2.h>
#endif

#pragma comment(lib, "ws2_32.lib")

// Copied from GeoIP.h
typedef struct GeoIPTag {
  FILE *GeoIPDatabase;
  char *file_path;
	unsigned char *cache;
	unsigned char *index_cache;
	unsigned int *databaseSegments;
	char databaseType;
	time_t mtime;
	int flags;
	char record_length;
	int record_iter; /* used in GeoIP_next_record */
} GeoIP;

typedef enum {
	GEOIP_STANDARD = 0,
	GEOIP_MEMORY_CACHE = 1,
	GEOIP_CHECK_CACHE = 2,
	GEOIP_INDEX_CACHE = 4,
} GeoIPOptions;

typedef GeoIP* (*GeoIP_newFunc)(int);
typedef const char * (*GeoIP_country_code_by_addrFunc) (GeoIP*, const char *);
typedef const char * (*GeoIP_country_name_by_addrFunc) (GeoIP*, const char *);

///////////////////////////////////////////////////////////////////////////////////////////////
class C6BeeGeoip{
private:
	CHttpSocket m_HttpSocket;
public:
	C6BeeGeoip();

public:
	HINSTANCE m_hGeoIP;
	GeoIP* m_pGeoIP;
	GeoIP_country_code_by_addrFunc m_pfnGeoIP_country_code_by_addr;
	GeoIP_country_name_by_addrFunc m_pfnGeoIP_country_name_by_addr;	

	ATL::CString GetCountryCode(IN_ADDR pAddress);
	ATL::CString GetCountryName(IN_ADDR pAddress);
};