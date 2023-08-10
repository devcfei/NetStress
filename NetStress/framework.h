#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <strsafe.h>
#include <shlobj.h>
#include <shlwapi.h>

// Socket
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdarg.h>

// STL
#include <vector>

// 
#include "resource.h"


#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


// Private modules
#include "maindlg.h"
#include "app.h"


// Macro
#define LOGF(...) AppLogPrintf(APPLOG_FATAL,__VA_ARGS__)
#define LOGE(...) AppLogPrintf(APPLOG_ERROR,__VA_ARGS__)
#define LOGW(...) AppLogPrintf(APPLOG_WARNING,__VA_ARGS__)
#define LOGI(...) AppLogPrintf(APPLOG_INFO,__VA_ARGS__)
#define LOGV(...) AppLogPrintf(APPLOG_VERBOSE,__VA_ARGS__)

// 
extern App* GetApp();


// Library
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Ws2_32.lib")
