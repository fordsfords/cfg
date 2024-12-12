# cfg
Simple configuration loader in C.


## Table of contents

<!-- mdtoc-start -->
&bull; [cfg](#cfg)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Table of contents](#table-of-contents)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Configuration File Format](#configuration-file-format)  
&bull; [Comments start with #](#comments-start-with-)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [API](#api)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Value Retrieval](#value-retrieval)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Operation Modes](#operation-modes)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Example Usage](#example-usage)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Possible enhancements:](#possible-enhancements)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Development Tips](#development-tips)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [License](#license)  
<!-- TOC created by '../mdtoc/mdtoc.pl README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->


## Introduction

A lightweight C library for parsing and managing configuration data in key-value pair format. The library provides robust error handling, type conversion, and flexible configuration loading from both files and string arrays.

- Simple key-value pair configuration format.
- Caller can set default values.
- Caller uses a [hash map](https://github.com/fordsfords/hmap) to retrieve values.
- Error handling with the [err module](https://github.com/fordsfords/err).
- Zero external dependencies except for standard C library


## Configuration File Format

The configuration file uses a simple key-value format:
```
# Comments start with #
key1=value1

key2=value2   # Inline comments are supported
   key3 = value3   # Whitespace is automatically trimmed
```

Rules:
- One key-value pair per line
- Keys must start with "_", "-" or alphabetic character,
followed by zero or more "_", "-", or alphanumeric characters.
- Keys and values are separated by '=' character
- Lines starting with '#' are treated as comments
- Inline comments after '#' are ignored
- Leading and trailing whitespace is automatically trimmed
- Empty lines are ignored
- Keys cannot contain whitespace
- Values cannot contain '#'


## API

Caller should include "err.h", "hmap.h", and "cfg.h".
- Caller uses a [hash map](https://github.com/fordsfords/hmap) to retrieve values.
- Error handling with the [err module](https://github.com/fordsfords/err).

```c
ERR_F cfg_create(cfg_t **rtn_cfg);
```
Creates a new configuration object. Must be called before any other operations.

```c
ERR_F cfg_delete(cfg_t *cfg);
```
Frees all resources associated with a configuration object.

```c
ERR_F cfg_parse_file(cfg_t *cfg, int mode, const char *filename);
```
Loads configuration from a file. Use "-" for stdin.
- `mode`: CFG_MODE_ADD or CFG_MODE_UPDATE; see [Operation Modes](#operational-modes).
- Returns error if file cannot be opened or contains invalid format

```c
ERR_F cfg_parse_string_list(cfg_t *cfg, int mode, char **string_list);
```
Loads configuration from a NULL-terminated array of strings.
- Each string should follow the same format as file lines
- `mode`: CFG_MODE_ADD or CFG_MODE_UPDATE; see [Operation Modes](#operational-modes).

The string list is an array of pointers to strings, with the last element NULL.
For example:
```c
  char *opt_list[] = {
    "abc = 123",
    "xyz=",  /* xyz defaults to zero-length string. */
    NULL};
```

### Value Retrieval

```c
ERR_F cfg_get_str_val(cfg_t *cfg, const char *key, char **rtn_value);
```
Retrieves string value for a given key.
- Returns error if key doesn't exist
- Returned string should not be modified or freed

```c
ERR_F cfg_get_long_val(cfg_t *cfg, const char *key, long *rtn_value);
```
Retrieves and converts value to long integer.
- Supports decimal and hexadecimal (0x prefix) formats
- Returns error if value cannot be converted

### Operation Modes

Reading configuration with `cfg_parse_string_list()` or `cfg_parse_string_list()`
can be done in two modes:
- `CFG_MODE_ADD`: Keys must not already exist
- `CFG_MODE_UPDATE`: Keys must already exist

The "add" mode is used by the application to set define the legal options
and set default values.
This is frequently used with `cfg_parse_string_list()` so that the program
has a compiled-in set of defaults.

The "update" mode is usually used with `cfg_parse_string_list()` to read in
the end-user's configuration file.


## Example Usage

See [example.c](example.c).


## Possible enhancements:

* Have flags that indicate if a keyword is required.
* %include file
* abc %get_env MY_ENV_VAR - allows
* Quoted strings to get whitespace in vals.


## Development Tips

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
