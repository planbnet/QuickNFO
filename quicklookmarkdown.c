#include "quicklookmarkdown.h"
#include "common.h"
#include "discount/mkdio.h"

#define PRE_MD_HTML "<html style='margin: 0; padding: 0; background-color: black;'><head><style>a { color: inherit; }</style></head><body style='margin: 0; padding: 2em;'><div style='color: white;'>"
#define POST_MD_HTML "</div></body></html>"

CFDataRef createMarkdownData(CFDataRef doc );
CFDataRef createHTMLDataFromMarkdown(CFDataRef markdown );

CFDataRef markdown2HTML( CFURLRef url)
{
	CFDataRef text = createDataFromURL(url);
	if (!text) return NULL;
	CFDataRef page = createMarkdownData(text);
	CFRelease(text);
	if (!page) return NULL;
	CFDataRef result = createHTMLDataFromMarkdown(page);
	CFRelease(page);
	return result;
}

CFDataRef createMarkdownData( CFDataRef doc )
{
	MMIOT* markdown = mkd_string((char*)CFDataGetBytePtr(doc), CFDataGetLength(doc), MKD_AUTOLINK); 
	char *buffer;
	if ( mkd_compile(markdown, 0) ) {
		int generated = mkd_document(markdown, &buffer);
		CFDataRef page = CFDataCreate(NULL, (UInt8*)buffer, generated);
		mkd_cleanup(markdown); 
		return page;
	}
	
	mkd_cleanup(markdown); 
	return NULL;
}

CFDataRef createHTMLDataFromMarkdown( CFDataRef markdown )
{
	CFMutableDataRef page = CFDataCreateMutable(NULL, 0);
	
	CFDataAppendBytes(page, (const UInt8*) PRE_MD_HTML, sizeof(PRE_MD_HTML));
	CFDataAppendBytes(page, CFDataGetBytePtr(markdown), CFDataGetLength(markdown));
	CFDataAppendBytes(page, (const UInt8*) POST_MD_HTML, sizeof(POST_MD_HTML));
	
	return page;
}

