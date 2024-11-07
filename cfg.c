/* cfg.c - support functions for simple error handler. */
/*
# This code and its documentation is Copyright 2024-2024 Steven Ford, http://geeky-boy.com
# and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/cfg
*/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include "err.h"
#include "hmap.h"
#include "cfg.h"


/* This prime number tells hmap how big of a table to allocate.
 * I wouldn't expect more than a few hundred options at the most. */
#define CFG_OPTION_MAP_SIZE 1009

ERR_F cfg_create(cfg_t **rtn_cfg, char *filename, err_t *err) {
  err_t err;
  FILE *file_fp;

  cfg_t cfg = malloc(sizeof(cfg_t));  ERR_ASSRT(cfg, CFG_ERR_NOMEM, err);
  *cfg = (cfg_t )0;

  ERR(hmap_create(&(cfg->option_vals), CFG_OPTION_MAP_SIZE));

  err = hmap_create(&(cfg->option_locations), CFG_OPTION_MAP_SIZE);
  if (err) {
    hmap_delete(cfg->option_vals);
    ERR_RETHROW(err, err->code);
  }

  file_fp = fopen(

  *rtn_cfg = cfg;
  ERR_RTN_OK(err);
}  /* cfg_create */


ERR_F cfg_parse_line(cfg_t *cfg, char *iline, char *location) {
  /* Strip optional comment from line. */
  no_comment = strtok(line, "#");

  char *key = strtok(no_comment, "\t ");
  if (key && strlen(key) > 0) {
    val = strtok(no_comment, "\n\r\t ");
    if (val) {
      val = strdup(val);  ERR_ASSRT(val, CFG_ERR_NOMEM, err);
      hmap_write(cfg->option_map, key, strlen(key), val, err);
    } else {
      val = strdup("");  ERR_ASSRT(val, CFG_ERR_NOMEM, err);
      hmap_write(cfg->option_map, key, strlen(key), val);
    }
  }

  return ERR_OK;
}


ERR_F cfg_delete(cfg_t *cfg) {
  hmap_delete(cfg->option_vals, NULL);

  return ERR_OK;
}  /* cfg_delete */

/* Parse space-delimited keyword/value pairs. */
ERR_F cfg_parse_file(FILE *fp, hmap_t *hmap) {
  char line[1024];

  while (fgets(iline, sizeof(iline), fp)) {
    char *no_comment, *key, *val;
    size_t len = strlen(line);
    ERR_ASSRT(len < sizeof(line) - 1) {  /* Line too long. */
  }

  return ERR_OK;
}
