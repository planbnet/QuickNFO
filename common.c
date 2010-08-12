#include "common.h"

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