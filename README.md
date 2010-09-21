QuickNFO
==================

Max OSX QuickLook Plugin for previewing NFO files

Uses libiconv to convert the nfo text to utf8 and then lets WebKit render the output. Seems to work well (at least for the few cases I have tested).

Currently, the plugin can only generate a preview. Maybe I'll add thumbnail support later.

Installation
==================
Build the xcode project or download the [compiled plugin][download]. Then copy the file `QuickNFO.qlgenerator` to the path `Library/QuickLook` inside your home directory.

[download]:http://github.com/downloads/planbnet/QuickNFO/quicknfo.zip
