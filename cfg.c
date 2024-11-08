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

ERR_F cfg_create(cfg_t **rtn_cfg) {
  err_t *err;

  cfg_t *cfg;
  ERR_ASSRT(cfg = calloc(1, sizeof(cfg_t)), CFG_ERR_NOMEM);

  ERR(hmap_create(&(cfg->option_vals), CFG_OPTION_MAP_SIZE));

  err = hmap_create(&(cfg->option_locations), CFG_OPTION_MAP_SIZE);
  if (err) {
    ERR(hmap_delete(cfg->option_vals));
    ERR_RETHROW(err, err->code);
  }

  *rtn_cfg = cfg;
  return ERR_OK;
}  /* cfg_create */


ERR_F cfg_delete(cfg_t *cfg) {
  hmap_node_t *node;

  node = NULL;  /* Start at beginning. */
  do {
    ERR(hmap_next(cfg->option_vals, &node));
    if (node) {
        ERR_ASSRT(node->value != NULL, CFG_ERR_INTERNAL);
        free(node->value);
    }
  } while (node);
  ERR(hmap_delete(cfg->option_vals));

  node = NULL;  /* Start at beginning. */
  do {
    ERR(hmap_next(cfg->option_locations, &node));
    if (node) {
        ERR_ASSRT(node->value != NULL, CFG_ERR_INTERNAL);
        free(node->value);
    }
  } while (node);
  ERR(hmap_delete(cfg->option_locations));

  return ERR_OK;
}  /* cfg_delete */


char *trim(char *in_str) {
  /* Skip over initial whitespace. */
  char *first_nonspace = in_str;
  while(isspace((unsigned char)*first_nonspace)) {
    str++;
  }
  int len = strlen(first_nonspace);
  if (len == 0) {  /* empty line. */
    return first_nonspace;
  }

  /* Trim final whitespace. */
  while (len > 0 && isspace((unsigned char)first_nonspace[len - 1])) {
    len --;
  }
  first_nonspace[len] = '\0';

  return first_non_space;
}  /* trim */


ERR_F cfg_parse_line(cfg_t *cfg, const char *iline, const char *filename, int line_num) {
  char *local_iline;
  char *strtok_state;

  ERR_ASSRT(local_iline = strdup(iline), CFG_ERR_NOMEM);  /* Need local copy because strtoc modifies string. */

  /* Strip (optional) comment from iline. */
  char *hash = strchr(local_iline, '#');
  if (hash) {
    *hash = '\0';  /* Force-end string at the "#" to strip comment. */
  }

  /* Skip blank lines. */
  char *key = trim(local_line);  /* Trim whitespace. */
  if (*key == '\0') {
    return ERR_OK;
  }

  /* Use equals sign to split key and value. */
  char *equals = strchr(trimmed_line, '=');
  ERR_ASSRT(equals, CFG_ERR_NOEQUALS);
  *equals = '\0';
  char *value = equals + 1;

  key = trim(key);
  ERR_ASSRT(strlen(key) > 0, CFG_ERR_NOKEY);
  value = trim(value);

  /* Make value its own mem segment to store in hash. */
  ERR_ASSRT(val = strdup(val), CFG_ERR_NOMEM);
  ERR(hmap_write(cfg->option_vals, key, strlen(key), val));

  /* Remember location for this option. */
  char *location;
  ERR_ASSRT(location = err_asprintf("%s:%d", filename, line_num), CFG_ERR_NOMEM);
  ERR(hmap_write(cfg->option_locations, key, strlen(key), location));

  free(local_iline);
  return ERR_OK;
}  /* cfg_parse_line */


ERR_F cfg_parse_file(cfg_t *cfg, const char *filename) {
  char iline[1024];
  FILE *file_fp;
  err_t *err = ERR_OK;

  ERR_ASSRT(cfg, CFG_ERR_PARAM);
  ERR_ASSRT(filename, CFG_ERR_PARAM);
  if (strcmp(filename, "-") == 0) {
    file_fp = stdin;
  } else {
    file_fp = fopen(filename, "r");
  }
  ERR_ASSRT(file_fp, CFG_ERR_BADFILE);

  int line_num = 0;
  while (fgets(iline, sizeof(iline), file_fp)) {
    line_num++;
    size_t len = strlen(iline);
    ERR_ASSRT(len < sizeof(iline) - 1, CFG_ERR_LINETOOLONG);  /* Line too long. */

    err = cfg_parse_line(cfg, iline, filename, line_num);
    if (err) { break; }
  }  /* while */

  if (strcmp(filename, "-") == 0) {
    /* Don't close stdin. */
  } else {
    fclose(file_fp);
  }

  if (err) {
    ERR_RETHROW(err, err->code);
  }
  return ERR_OK;
}  /* cfg_parse_file */


ERR_F cfg_get_str_val(cfg_t *cfg, const char *key) {
}  /* cfg_get_str_val */


ERR_F cfg_get_int_val(cfg_t *cfg, const char *key) {
}  /* cfg_get_int_val */
