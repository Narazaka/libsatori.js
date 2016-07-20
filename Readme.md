libsatori.js
=======================================

Yet Another SATORI on JavaScript

Original SHIORI module
---------------------------------------

[SATORI](https://github.com/ponapalt/satoriya-shiori) by [The Maintenance Shop](http://ms.shillest.net/)

Installation
---------------------------------------

    npm install libsatori.js

or

    bower install libsatori.js

or download zip archive and get libsatori.js

Usage
---------------------------------------

This module has only low level API, that is provided by emscripten.

So use the wrapper module, [nativeshiori](https://github.com/Narazaka/nativeshiori), and do like below.

    <script src="encoding.min.js"></script>
    <script src="nativeshiori.js"></script>
    <script src="libsatori.js"></script>
    <script>
    
    var dic_test_txt_str = '＊OnBoot\r\n：Hello World\r\n';
    var dic_test_txt = Encoding.convert(Encoding.stringToCode(dic_test_txt_str), 'SJIS', 'UNICODE');
    var storage = {
      'dic_test.txt': new Uint8Array(dic_test_txt) // filename: ArrayBufferView
    };
    
    // Shiori instance and optional storage (/path/to/ghost/master/*)
    var nativeshiori = new NativeShiori(new Satori(), storage);
    
    // write files in storage to FS then load() if storage exists, else load()
    var load_code = nativeshiori.load('/path/to/ghost/master'); 
    
    // request()
    var response = nativeshiori.request('GET SHIORI/3.0\r\nCharset: Shift_JIS\r\ID: OnBoot\r\n\r\n');
    
    // unload()
    var unload_code = nativeshiori.unload();
    
    </script>

LICENSE
--------------------------------

(C) 2016 Narazaka : Licensed under [The BSD 3-Clause License](http://narazaka.net/license/BSD3?2016)
