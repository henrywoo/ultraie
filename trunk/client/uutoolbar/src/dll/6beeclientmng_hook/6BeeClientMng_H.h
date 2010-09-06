#ifdef MY6BEECLIENTMNG_H_EXPORTS
#define MY6BEECLIENTMNG_H_API __declspec(dllexport)
#else
#define MY6BEECLIENTMNG_H_API __declspec(dllimport)
#endif

extern "C"{
  MY6BEECLIENTMNG_H_API BOOL WINAPI InstallIECreatProcessHook();
  MY6BEECLIENTMNG_H_API BOOL WINAPI UnInstallIECreatProcessHook();
}