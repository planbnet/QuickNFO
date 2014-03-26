#include "quicklooknfo.h"
#include <iconv.h>
#include <unistd.h>

#define BUFFERSIZE 1024 * 16

#define PRE_NFO_HTML "<html style='margin: 0; padding: 0; background-color: white;'><style type='text/css'>.block { -webkit-font-smoothing: none; }</style><body style='margin: 0; padding: 0;'><pre style='font-family: Andale Mono, Menlo, monospace; font-size: 10px; line-height: 1; color: black; -webkit-font-smoothing: subpixel-antialiased;'>"
#define POST_NFO_HTML "</pre></body></html>"

#define PRE_BLOCK_HTML "<span class='block'>"
#define POST_BLOCK_HTML "</span>"

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
	CFDataRef result = createConvertedString(text, "437", "UTF8");
	CFRelease(text);
	printf("%s", CFDataGetBytePtr(result));
	CFRelease(result);
	return 0;
}

CFDataRef createHTMLPreview( CFURLRef url )
{
	CFDataRef text = createDataFromURL(url);
	if (!text) return NULL;
    
	CFDataRef ucs2 = createConvertedString(text, "437", "UCS-2-INTERNAL");
	CFRelease(text);
	if (!ucs2) return NULL;
    
    CFDataRef page2 = createHTMLData(ucs2);
	CFRelease(ucs2);
	if (!page2) return NULL;
    
    CFDataRef result = createConvertedString(page2, "UCS-2-INTERNAL", "UTF8");
    CFRelease(page2);
    
	return result;
}

CFDataRef createDataFromURL( CFURLRef url )
{
	CFMutableDataRef fileContent = CFDataCreateMutable(kCFAllocatorDefault, 0);
    CFReadStreamRef stream = CFReadStreamCreateWithFile(kCFAllocatorDefault, url);

    if (stream) {
        if (CFReadStreamOpen(stream)) {
            UInt8 buffer[BUFFERSIZE];
            CFIndex bytesRead;
            do {
                bytesRead = CFReadStreamRead(stream, buffer, sizeof(buffer));
                if (bytesRead > 0) {
                    CFDataAppendBytes(fileContent, buffer, bytesRead);
                }
            } while (bytesRead > 0);
            CFReadStreamClose(stream);
        }
        CFRelease(stream);
    }
    
	return fileContent;
}

CFDataRef createConvertedString( CFDataRef text, char* srcEncoding, char* dstEncoding )
{
	iconv_t cd;
	cd = iconv_open (dstEncoding, srcEncoding);
	if (cd == (iconv_t) -1)
		return NULL;

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

CFDataRef createUCS2FromConst(char * ptr, size_t length) {
    CFDataRef tmp = CFDataCreate(NULL, (UInt8*)ptr, length);
    CFDataRef result = createConvertedString(tmp, "C99", "UCS-2-INTERNAL");
    CFRelease(tmp);
    
    return result;
}

void appendCFData(CFMutableDataRef dst, CFDataRef src) {
    CFDataAppendBytes(dst, CFDataGetBytePtr(src), CFDataGetLength(src));
}

#define ISBLOCKORBOX(wchar) ((wchar >= 0x2500) && (wchar <= 0x25A9))
#define ISWHITESPACE(wchar) ((wchar <= 0x0020))

CFDataRef createHTMLData(CFDataRef nfo) {
    // Load HTML constants (UCS2)
    CFDataRef preNfo = createUCS2FromConst(PRE_NFO_HTML, sizeof(PRE_NFO_HTML));
    CFDataRef postNfo = createUCS2FromConst(POST_NFO_HTML, sizeof(POST_NFO_HTML));
    CFDataRef preBlock = createUCS2FromConst(PRE_BLOCK_HTML, sizeof(PRE_BLOCK_HTML));
    CFDataRef postBlock = createUCS2FromConst(POST_BLOCK_HTML, sizeof(POST_BLOCK_HTML));
    
    
    CFMutableDataRef result = CFDataCreateMutable(NULL, 0);
    
    appendCFData(result, preNfo);
    CFRelease(preNfo);
    
    const UInt8* inPtr = CFDataGetBytePtr(nfo);
	size_t inCharsLeft = CFDataGetLength(nfo) / sizeof(UInt16);
    
    const UInt8* bsPtr = inPtr;
    bool inRun = false;
    
    while (inCharsLeft-- > 0) {
        UInt16 chr = *((const UInt16*)inPtr);
        
        // Look ahead for new state
        bool newState = inRun;
        
        if(!inRun) {
            if(ISBLOCKORBOX(chr)) {
                newState = true;
            }
        } else {
            if(!ISBLOCKORBOX(chr) && !ISWHITESPACE(chr)) {
                newState = false;
            }
        }
        
        // Process change of state, append data
        if(inRun != newState) {
            if(inPtr != bsPtr) {
                if(inRun) {
                    appendCFData(result, preBlock);
                    CFDataAppendBytes(result, (const UInt8*)bsPtr, (inPtr - bsPtr));
                    appendCFData(result, postBlock);
                } else {
                    CFDataAppendBytes(result, (const UInt8*)bsPtr, (inPtr - bsPtr));
                }
                
                bsPtr = inPtr;
            }
            
            inRun = newState;
        }
        
        inPtr += sizeof(UInt16);
    }
    
    // Append trailing data
    if(inPtr > bsPtr) {
        if(inRun) {
            appendCFData(result, preBlock);
            CFDataAppendBytes(result, (const UInt8*)bsPtr, (inPtr - bsPtr));
            appendCFData(result, postBlock);
        } else {
            CFDataAppendBytes(result, (const UInt8*)bsPtr, (inPtr - bsPtr));
        }
    }
    
    CFRelease(preBlock);
    CFRelease(postBlock);
    
    appendCFData(result, postNfo);
    CFRelease(postNfo);
    
    return result;
}