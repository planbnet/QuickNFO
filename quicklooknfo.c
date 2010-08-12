#include "quicklooknfo.h"
#include "common.h"
#include <iconv.h>
#include <unistd.h>

#define BUFFERSIZE 1024 * 16

#define PRE_NFO_HTML "<html style='margin: 0; padding: 0; background-color: black;'><body style='margin: 0; padding: 0;'><pre style='font-family: Andale Mono, Menlo, monospace; font-size: 10px; line-height: 1; color: white;'>"
#define POST_NFO_HTML "</pre></body></html>"

CFDataRef createNFOString( CFDataRef text);
CFDataRef createHTMLDataFromNFO( CFDataRef nfoString);

CFDataRef NFO2HTML( CFURLRef url )
{
	CFDataRef text = createDataFromURL(url);
	if (!text) return NULL;
	CFDataRef page = createNFOString(text);
	CFRelease(text);
	if (!page) return NULL;
	CFDataRef result = createHTMLDataFromNFO(page);
	CFRelease(page);
	return result;
}


CFDataRef createNFOString( CFDataRef text )
{
	iconv_t cd;
	cd = iconv_open ("UTF8", "437");
	if (cd == (iconv_t) -1)
	{
		return NULL;
	}

	CFMutableDataRef result = CFDataCreateMutable(NULL, 0);
	
	char *inPtr = (char*) CFDataGetBytePtr(text);
	size_t inBytesLeft = CFDataGetLength(text);
	
	char buffer[BUFFERSIZE];
	do {
		size_t outBytesLeft = BUFFERSIZE;
		char *outPtr = (char*)buffer;
		iconv(cd, &inPtr, &inBytesLeft, &outPtr , &outBytesLeft);
		CFIndex converted = BUFFERSIZE - outBytesLeft;
		CFDataAppendBytes(result, (UInt8*) &buffer, converted);
	} while (inBytesLeft > 0);
	
	iconv_close( cd );
	
	return result;
}


CFDataRef createHTMLDataFromNFO(CFDataRef nfo )
{
	CFMutableDataRef page = CFDataCreateMutable(NULL, 0);

	CFDataAppendBytes(page, (const UInt8*) PRE_NFO_HTML, sizeof(PRE_NFO_HTML));
	CFDataAppendBytes(page, CFDataGetBytePtr(nfo), CFDataGetLength(nfo));
	CFDataAppendBytes(page, (const UInt8*) POST_NFO_HTML, sizeof(POST_NFO_HTML));
	
	return page;
}