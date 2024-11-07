/* err.h - simple error handler. */
/*
# This code and its documentation is Copyright 2024-2024 Steven Ford, http://geeky-boy.com
# and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/lsim
*/

#ifndef ERR_H
#define ERR_H

#include "err.h"
#include "hmap.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cfg_s {
  hmap_t *option_vals;
  hmap_t *option_locations;
};
typedef struct cfg_s cfg_t;

#define ERR(err_code) extern char *err_code
ERR(CFG_ERR_LINETOOLONG);
ERR(CFG_ERR_NOMEM);
ERR(CFG_ERR_HMAP);
#undef ERR

#define CFG_CODE_BADFILE -1

#ifdef __cplusplus
}
#endif

#endif // ERR_H
