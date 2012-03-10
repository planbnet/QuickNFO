#include "quicklooknfo.h"
#include <iconv.h>
#include <unistd.h>

#define BUFFERSIZE 1024 * 16

#define PRE_NFO_HTML "<html style='background-color: #000064;'><body><pre style='font-family: Andale Mono, Menlo, monospace; font-size: 12px; line-height: 1; color: #f6f6f6;'>"
#define POST_NFO_HTML "</pre></body></html>"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Specify nfo file");
		return 1;
	}
	CFStringRef path = CFStringCreateWithCString(NULL, argv[1], kCFStringEncodingUTF8);
	CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, false);
	CFRelease(path);
	CFDataRef text = createDataFromURL(url);
	CFRelease(url);
	if (!text) {
		printf("File does not exist");
		return 2;
	}
	CFDataRef result = createNFOString(text);
	CFRelease(text);
	printf("%s", CFDataGetBytePtr(result));
	CFRelease(result);
	return 0;
}

CFDataRef createHTMLPreview( CFURLRef url )
{
	CFDataRef text = createDataFromURL(url);
	if (!text) return NULL;
	CFDataRef page = createNFOString(text);
	CFRelease(text);
	if (!page) return NULL;
	CFDataRef result = createHTMLData(page);
	CFRelease(page);
	return result;
}

CFDataRef createDataFromURL( CFURLRef url )
{
	SInt32 errorCode = 0;

	CFDataRef fileContent;
	CFDictionaryRef dict;
	CFArrayRef arr = CFArrayCreate(NULL, NULL, 0, NULL);

	Boolean success = CFURLCreateDataAndPropertiesFromResource (NULL,
																url,
																&fileContent,
																&dict,
																arr,
																&errorCode);
	CFRelease(arr);
	CFRelease(dict);
	
	if (!success) {
		return NULL;
	}
	
	return fileContent;
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


CFDataRef createHTMLData(CFDataRef nfo )
{
	CFMutableDataRef page = CFDataCreateMutable(NULL, 0);

	CFDataAppendBytes(page, (const UInt8*) PRE_NFO_HTML, sizeof(PRE_NFO_HTML));
	CFDataAppendBytes(page, CFDataGetBytePtr(nfo), CFDataGetLength(nfo));
	CFDataAppendBytes(page, (const UInt8*) POST_NFO_HTML, sizeof(POST_NFO_HTML));
	
	return page;
}