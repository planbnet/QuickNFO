#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>

CFDataRef createHTMLPreview( CFURLRef url) CF_RETURNS_RETAINED;
CFDataRef createDataFromURL( CFURLRef url) CF_RETURNS_RETAINED;
CFDataRef createConvertedString( CFDataRef text, char* srcEncoding, char* dstEncoding ) CF_RETURNS_RETAINED;
CFDataRef createHTMLData( CFDataRef nfoString) CF_RETURNS_RETAINED;