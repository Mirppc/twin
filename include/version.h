
#ifndef TWIN_VERSION

#define STR(v)   #v
#define toSTR(v) STR(v)

#define TWIN_VERSION_MAJOR 0
#define TWIN_VERSION_MINOR 3
#define TWIN_VERSION_PATCH 4
#define TWIN_VERSION       ((TWIN_VERSION_MAJOR<<16)|(TWIN_VERSION_MINOR<8)|TWIN_VERSION_PATCH)
#define TWIN_VERSION_STR   toSTR(TWIN_VERSION_MAJOR) "." toSTR(TWIN_VERSION_MINOR) "." toSTR(TWIN_VERSION_PATCH)

#define TW_PROTOCOL_VERSION_MAJOR 2
#define TW_PROTOCOL_VERSION_MINOR 0
#define TW_PROTOCOL_VERSION_PATCH 0
#define TW_PROTOCOL_VERSION       ((TW_PROTOCOL_VERSION_MAJOR<<16)|(TW_PROTOCOL_VERSION_MINOR<8)|TW_PROTOCOL_VERSION_PATCH)
#define TW_PROTOCOL_VERSION_STR   toSTR(TW_PROTOCOL_VERSION_MAJOR) "." toSTR(TW_PROTOCOL_VERSION_MINOR) "." toSTR(TW_PROTOCOL_VERSION_PATCH)

#endif /* TWIN_VERSION */

