# cfg
Simple configuration loader in C.


## Table of contents

<!-- mdtoc-start -->
&bull; [cfg](#cfg)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Table of contents](#table-of-contents)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [API](#api)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Possible enhancements:](#possible-enhancements)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Integration with "err" and "hmap".](#integration-with-err-and-hmap)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Development](#development)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [License](#license)  
<!-- TOC created by '../mdtoc/mdtoc.pl README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->


## Introduction

Allows you load a file with keyword=value pairs and access
the values via a hashmap.
Caller can define default values.


## API

Should be pretty obvious just looking at the "cfg.h" file. 
Maybe some day I'll put more here.

Uses "err" subsystem; see https://github.com/fordsfords/err

keys and values may not contain whitespace. Special characters are OK and are not interpreted in any special way
except for equals sign (=), which separates keyword from value.


## Possible enhancements:
* Have flags that indicate if a keyword is required.
* %include file
* abc %get_env MY_ENV_VAR - allows
* Quotes strings to get whitespace in vals.


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
