QuickNFO 1.2
==================

Mac OSX QuickLook Plugin for previewing NFO files

Uses libiconv to convert the nfo text to utf8 and then lets WebKit render the output. Seems to work well (at least for the few cases I have tested).

The current version supports preview and thumbnail generation. Rendering with HiDPI, like on retina displays, is also supported.

Installation
==================

Build the xcode project or download the [compiled plugin][download]. Then copy the file `QuickNFO.qlgenerator` to the path `Library/QuickLook` inside your home directory.

[download]:https://github.com/The-Master777/QuickNFO/releases/download/v1.2/QuickNFO.qlgenerator.zip

Examples
==================
###Thumbnails
![QuickLook Thumbnails](https://github.com/downloads/planbnet/QuickNFO/thumbnails.png "QuickLook Thumbnails")

###Coverflow
![QuickLook Coverflow](https://github.com/downloads/planbnet/QuickNFO/coverflow.png "QuickLook Coverflow")

###Preview
![QuickLook Preview](https://github.com/downloads/planbnet/QuickNFO/preview.png "QuickLook Preview")
