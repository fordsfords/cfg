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

#ifndef CFG_H
#define CFG_H

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

#define CFG_ERR_INTERNAL 1
#define CFG_ERR_PARAM 2
#define CFG_ERR_NOMEM 3
#define CFG_ERR_BADFILE 4
#define CFG_ERR_LINETOOLONG 5
#define CFG_ERR_NOEQUALS 6
#define CFG_ERR_NOKEY 7

ERR_F cfg_create(cfg_t **rtn_cfg);
ERR_F cfg_delete(cfg_t *cfg);
ERR_F cfg_parse_line(cfg_t *cfg, const char *iline, const char *filename, int line_num);
ERR_F cfg_parse_file(cfg_t *cfg, const char *filename);

#ifdef __cplusplus
}
#endif

#endif  /* CFG_H */
