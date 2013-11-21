#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>

CFDataRef createHTMLPreview( CFURLRef url);
CFDataRef createDataFromURL( CFURLRef url);
CFDataRef createConvertedString( CFDataRef text, char* srcEncoding, char* dstEncoding );
CFDataRef createHTMLData( CFDataRef nfoString);