#include <string>
#include <Wininet.h>

extern void  ConsoleBypass(const char* Name);
extern void auto_update();
std::string DownloadURL(const std::string& url);