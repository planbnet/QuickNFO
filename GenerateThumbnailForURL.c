#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>

#define minSize 32

OSStatus 
GenerateThumbnailForURL(void *thisInterface, 
						QLThumbnailRequestRef thumbnail, 
						CFURLRef url, 
						CFStringRef contentTypeUTI, 
						CFDictionaryRef options, 
						CGSize maxSize)
{
        return noErr;
}

void CancelThumbnailGeneration(void* thisInterface, QLThumbnailRequestRef thumbnail)
{
    // implement only if supported
}
