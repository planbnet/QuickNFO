#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>
#include <WebKit/WebKit.h>

#include "quicklooknfo.h"

#define minSize 32
#define thumbnailWidth 600.0
#define thumbnailHeight 800.0

OSStatus 
GenerateThumbnailForURL(void *thisInterface, 
						QLThumbnailRequestRef thumbnail, 
						CFURLRef url, 
						CFStringRef contentTypeUTI, 
						CFDictionaryRef options, 
						CGSize maxSize)
{
	
    if (maxSize.width < minSize || maxSize.height < minSize)
        return noErr;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSRect renderRect = NSMakeRect(0.0, 0.0, thumbnailWidth, thumbnailHeight);
    float scale = maxSize.height/thumbnailHeight;
    NSSize scaleSize = NSMakeSize(scale, scale);
    CGSize thumbSize = NSSizeToCGSize(
									  NSMakeSize((maxSize.width * (thumbnailWidth/thumbnailHeight)), 
												 maxSize.height));
	
  	CFDataRef dataRef = createHTMLPreview( url );
    NSData *data = (id)dataRef;
	
    WebView* webView = [[WebView alloc] initWithFrame:renderRect];
    [webView scaleUnitSquareToSize:scaleSize];
    [[[webView mainFrame] frameView] setAllowsScrolling:NO];
    
    [[webView mainFrame] loadData:data MIMEType:@"text/html"
                 textEncodingName:@"UTF-8" baseURL:nil];
    
    while([webView isLoading]) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
    }
    
    // Get a context to render into
    CGContextRef context = 
	QLThumbnailRequestCreateContext(thumbnail, thumbSize, false, NULL);
    
    if(context != NULL) {
        NSGraphicsContext* nsContext = 
		[NSGraphicsContext
		 graphicsContextWithGraphicsPort:(void *)context 
		 flipped:[webView isFlipped]];
        
        [webView displayRectIgnoringOpacity:[webView bounds]
                                  inContext:nsContext];
        
        QLThumbnailRequestFlushContext(thumbnail, context);
        
        CFRelease(context);
    }
	
	CFRelease(data);
    [webView release];
    
	[pool release];
	
    return noErr;
}

void CancelThumbnailGeneration(void* thisInterface, QLThumbnailRequestRef thumbnail)
{

}
