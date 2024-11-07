# cfg
Simple configurator in C.


## Table of contents

<!-- mdtoc-start -->
&bull; [cfg](#cfg)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Table of contents](#table-of-contents)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [API](#api)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Integration with "err" and "hmap".](#integration-with-err-and-hmap)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Development](#development)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [License](#license)  
<!-- TOC created by '../mdtoc/mdtoc.pl README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->


## Introduction

tbd

* Values may not contain whitespace. Special characters are OK and are not interpreted in any special way.


## API

Should be pretty obvious. 
APIs return 0 for success, -1 for error.
A key is an arbitrary byte array, not a necessarily a valid C string (although a C string works fine).


## Integration with "err" and "hmap".

tbd


## Development

* bld.sh - builds the test program.
* tst.sh - calls "bld.sh" and runs the test programs.


## License

I want there to be NO barriers to using this code, so I am releasing it to the public domain.  But "public domain" does not have an internationally agreed upon definition, so I use CC0:

Copyright 2024-2024 Steven Ford http://geeky-boy.com and licensed
"public domain" style under
[CC0](http://creativecommons.org/publicdomain/zero/1.0/):
![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png "CC0")

To the extent possible under law, the contributors to this project have
waived all copyright and related or neighboring rights to this work.
In other words, you can use this code for any purpose without any
restrictions.  This work is published from: United States.  The project home
is https://github.com/fordsfords/cfg

To contact me, Steve Ford, project owner, you can find my email address
at http://geeky-boy.com.  Can't see it?  Keep looking.
