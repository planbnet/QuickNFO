QuickNFO
==================

Max OSX QuickLook Plugin for previewing NFO files

Uses libiconv to convert the nfo text to utf8 and then lets WebKit render the output. Seems to work well (at least for the few cases I have tested).

The current version supports preview and thumbnail generation.

Installation
==================

Build the xcode project or download the [compiled plugin][download]. Then copy the file `QuickNFO.qlgenerator` to the path `Library/QuickLook` inside your home directory.

[download]:http://github.com/downloads/planbnet/QuickNFO/QuickNFO.zip

Examples
==================
###Thumbnails
![QuickLook Thumbnails](http://github.com/downloads/planbnet/QuickNFO/thumbnails.png "QuickLook Thumbnails")

###Coverflow
![QuickLook Coverflow](http://github.com/downloads/planbnet/QuickNFO/coverflow.png "QuickLook Coverflow")

###Preview
![QuickLook Preview](http://github.com/downloads/planbnet/QuickNFO/preview.png "QuickLook Preview")
