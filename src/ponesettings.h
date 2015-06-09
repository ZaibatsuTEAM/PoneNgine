#ifndef PONESETTINGS
#define PONESETTINGS

/*
 *  PoneNgine settings header
 *
 * We put global things in here. 'Nuff said.
 */

// Defined all possible platforms
#define PLAT_WIN32 0
#define PLAT_MACOSX 1
#define PLAT_UNIX 2
#define PLAT_WINRT 3
#define PLAT_ANDROID 4
#define PLAT_IOS 5

// Yes we are calling almost everything with Pony at the beginning
// The engine is named after pony, so, why not?

#if defined(WIN32) || defined(_WIN32)
#define PONY_PLATFORM PLAT_WIN32
#elif defined(__APPLE__)
#define PONY_PLATFORM PLAT_MACOSX
#elif defined(__WINRT__)
#define PONY_PLATFORM PLAT_WINRT
#elif defined(__ANDROID__)
#define PONY_PLATFORM PLAT_ANDROID
#elif defined(__IPHONEOS__)
#define PONY_PLATFORM PLAT_IOS
#else
#define PONY_PLATFORM PLAT_UNIX
#endif

// I'm using lots of preprocesors, am I c++ mlg pro yet guise? Guise? :c

/*
 * PoneNgine at the moment only supports Windows, but we have defined all these other
 * platforms for the possibility that we will support those, after windows, we are going
 * after Linux, then Mac (when I get someone to compile this for mac, because I don't own
 * a mac PC) and then after Android and IOS. WINRT is just there incase we want PoneNgine
 * to support Windows 8/10's app system, it might be experimented, but who knows.
 *
 * Another Note: as long as all of the libraries we use support these platforms, PoneNgine
 * will be able to support those platforms. Which is also how I am researching a lot on
 * many libraries to find one that supports AT LEAST all desktop platforms.
 *
 * ~WapaMario63 (Wapa Viddter)
 */

// Define prefixes for logs

#define PREF_LOG_INFO       "[INFO] "
#define PREF_LOG_ERROR      "[ERROR] "
#define PREF_LOG_SEVERE     "[SEVERE] "
#define PREF_LOG_NETINFO    "[NETWORK] "
#define PREF_LOG_MEDIAINFO  "[MEDIA] "

#endif // PONESETTINGS

