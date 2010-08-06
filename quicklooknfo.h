#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>

CFDataRef createHTMLPreview( CFURLRef url);
CFDataRef createDataFromURL( CFURLRef url);
CFDataRef createNFOString( CFDataRef text);
CFDataRef createHTMLData( CFDataRef nfoString);