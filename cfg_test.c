/* cfg_tst.c - self-test.
 *
 * This code and its documentation is Copyright 2019, 2019 Steven Ford,
 * http://geeky-boy.com and licensed "public domain" style under Creative
 * Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
 * To the extent possible under law, the contributors to this project have
 * waived all copyright and related or neighboring rights to this work.
 * In other words, you can use this code for any purpose without any
 * restrictions. This work is published from: United States. The project home
 * is https://github.com/fordsfords/cfg
 */

#include <stdio.h>
#include <string.h>
#if ! defined(_WIN32)
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#endif
#include "err.h"
#include "hmap.h"
#include "cfg.h"

#if defined(_WIN32)
#define MY_SLEEP_MS(msleep_msecs) Sleep(msleep_msecs)
#else
#define MY_SLEEP_MS(msleep_msecs) usleep((msleep_msecs)/1000)
#endif

#define E(e__test) do { \
  err_t *e__err = (e__test); \
  if (e__err != ERR_OK) { \
    printf("ERROR [%s:%d]: '%s' returned error\n", __FILE__, __LINE__, #e__test); \
    ERR_ABRT_ON_ERR(e__err, stdout); \
    exit(1); \
  } \
} while (0)

#define ASSRT(assrt__cond) do { \
  if (! (assrt__cond)) { \
    printf("ERROR [%s:%d]: assert '%s' failed\n", __FILE__, __LINE__, #assrt__cond); \
    exit(1); \
  } \
} while (0)


/* Options */
int o_testnum;


char usage_str[] = "Usage: err_test [-h] [-t testnum]";
void usage(char *msg) {
  if (msg) fprintf(stderr, "\n%s\n\n", msg);
  fprintf(stderr, "%s\n", usage_str);
  exit(1);
}  /* usage */

void help() {
  printf("%s\n"
    "where:\n"
    "  -h - print help\n"
    "  -t testnum - Specify which test to run [1].\n"
    "For details, see https://github.com/fordsfords/cfg\n",
    usage_str);
  exit(0);
}  /* help */


void parse_cmdline(int argc, char **argv) {
  int i;

  /* Since this is Unix and Windows, don't use getopts(). */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0) {
      help();  exit(0);

    } else if (strcmp(argv[i], "-t") == 0) {
      if ((i + 1) < argc) {
        i++;
        o_testnum = atoi(argv[i]);
      } else { fprintf(stderr, "Error, -t requires test number\n");  exit(1); }

    } else { fprintf(stderr, "Error, unknown option '%s'\n", argv[i]);  exit(1); }
  }  /* for i */
}  /* parse_cmdline */


void test1() {
  cfg_t *cfg;
  char *val;
  err_t *err;

  E(cfg_create(&cfg));

  E(cfg_parse_line(cfg, CFG_MODE_ADD, "", "test1a", 1));
  ASSRT(cfg->option_vals->num_entries == 0);
  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, "   ", "test1a", 2));
  ASSRT(cfg->option_vals->num_entries == 0);
  E(cfg_parse_line(cfg, CFG_MODE_ADD, "#  ", "test1a", 3));
  ASSRT(cfg->option_vals->num_entries == 0);
  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, " # ", "test1a", 4));
  ASSRT(cfg->option_vals->num_entries == 0);

  err = cfg_parse_line(cfg, CFG_MODE_ADD, "aaa", "test1a", 5);
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_NOEQUALS);
  err_dispose(err);

  err = cfg_parse_line(cfg, CFG_MODE_UPDATE, "aaa=", "test1a", 5);
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_UPDATE_KEY_NOT_FOUND);
  err_dispose(err);

  E(cfg_parse_line(cfg, CFG_MODE_ADD, "aaa=", "test1a", 6));
  ASSRT(cfg->option_vals->num_entries == 1);
  E(hmap_lookup(cfg->option_vals, "aaa", 3, (void **)&val));
  ASSRT(strlen(val) == 0);
  ASSRT(cfg->option_locations->num_entries == 1);
  E(hmap_lookup(cfg->option_locations, "aaa", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:6") == 0);

  E(cfg_parse_line(cfg, CFG_MODE_ADD, " aab = # ", "test1a", 7));
  ASSRT(cfg->option_vals->num_entries == 2);
  E(hmap_lookup(cfg->option_vals, "aab", 3, (void **)&val));
  ASSRT(strlen(val) == 0);
  ASSRT(cfg->option_locations->num_entries == 2);
  E(hmap_lookup(cfg->option_locations, "aab", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:7") == 0);

  E(cfg_parse_line(cfg, CFG_MODE_ADD, "aac=113", "test1a", 8));
  ASSRT(cfg->option_vals->num_entries == 3);
  E(hmap_lookup(cfg->option_vals, "aac", 3, (void **)&val));
  ASSRT(strcmp(val, "113") == 0);
  ASSRT(cfg->option_locations->num_entries == 3);
  E(hmap_lookup(cfg->option_locations, "aac", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:8") == 0);

  E(cfg_parse_line(cfg, CFG_MODE_ADD, "  aad = 1 1    4  #  xyz", "test1a", 9));
  ASSRT(cfg->option_vals->num_entries == 4);
  E(hmap_lookup(cfg->option_vals, "aad", 3, (void **)&val));
  ASSRT(strcmp(val, "1 1    4") == 0);
  ASSRT(cfg->option_locations->num_entries == 4);
  E(hmap_lookup(cfg->option_locations, "aad", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:9") == 0);

  /* Overwrite previous one. */
  err = cfg_parse_line(cfg, CFG_MODE_ADD, " aab= 1=12#", "test1a", 9);
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_ADD_KEY_ALREADY_EXIST);
  err_dispose(err);

  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, " aab= 1=12#", "test1a", 10));
  ASSRT(cfg->option_vals->num_entries == 4);
  E(hmap_lookup(cfg->option_vals, "aab", 3, (void **)&val));
  ASSRT(strcmp(val, "1=12") == 0);
  ASSRT(cfg->option_locations->num_entries == 4);
  E(hmap_lookup(cfg->option_locations, "aab", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:10") == 0);

  E(cfg_delete(cfg));
}  /* test1 */


void test2() {
  cfg_t *cfg;
  char *val;
  err_t *err;
  char *opt_list[] = {"abc = 123", "xyz=", "1 2 3 = x y z", NULL};

  E(cfg_create(&cfg));

  err = cfg_parse_string_list(cfg, CFG_MODE_UPDATE, opt_list);
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_UPDATE_KEY_NOT_FOUND);

  E(cfg_parse_string_list(cfg, CFG_MODE_ADD, opt_list));
  E(hmap_lookup(cfg->option_vals, "1 2 3", 5, (void **)&val));
  ASSRT(strcmp(val, "x y z") == 0);

  E(cfg_parse_file(cfg, CFG_MODE_ADD, "tst2.cfg"));
  E(hmap_lookup(cfg->option_vals, "opt1", 4, (void **)&val));
  ASSRT(strcmp(val, "xyz") == 0);
  E(hmap_lookup(cfg->option_vals, "opt2", 4, (void **)&val));
  ASSRT(strcmp(val, "") == 0);
  E(hmap_lookup(cfg->option_vals, "opt3", 4, (void **)&val));
  ASSRT(strcmp(val, "3") == 0);

  err = cfg_parse_file(cfg, CFG_MODE_ADD, "tst2.cfg");
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_ADD_KEY_ALREADY_EXIST);

  E(cfg_delete(cfg));
}  /* test2 */


void test3() {
  cfg_t *cfg;
  char *val;
  err_t *err;

  E(cfg_create(&cfg));

  err = cfg_parse_file(cfg, CFG_MODE_ADD, "-");
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_ADD_KEY_ALREADY_EXIST);

  E(hmap_lookup(cfg->option_vals, "opt10", 5, (void **)&val));
  ASSRT(strcmp(val, "xyz") == 0);
  E(hmap_lookup(cfg->option_vals, "opt20", 5, (void **)&val));
  ASSRT(strcmp(val, "") == 0);
  E(hmap_lookup(cfg->option_vals, "opt30", 5, (void **)&val));
  ASSRT(strcmp(val, "3") == 0);

  E(cfg_delete(cfg));
}  /* test3 */


void test4() {
  cfg_t *cfg;
  char *val;
  long lval;
  err_t *err;

  E(cfg_create(&cfg));

  E(cfg_parse_line(cfg, CFG_MODE_ADD, "abc=123", "test4", 1));
  E(cfg_get_str_val(cfg, "abc", &val));
  ASSRT(strcmp(val, "123") == 0);
  E(cfg_get_long_val(cfg, "abc", &lval));
  ASSRT(lval == 123);

  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, "abc=0x10", "test4", 2));
  E(cfg_get_long_val(cfg, "abc", &lval));
  ASSRT(lval == 16);

  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, "abc=xyz", "test4", 3));
  err = cfg_get_long_val(cfg, "abc", &lval);
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_BAD_NUMBER);

  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, "abc=", "test4", 4));
  err = cfg_get_long_val(cfg, "abc", &lval);
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_BAD_NUMBER);

  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, "abc=123 x", "test4", 5));
  err = cfg_get_long_val(cfg, "abc", &lval);
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_BAD_NUMBER);

  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, "abc=9 223 372 036 854 775 807", "test4", 6)); /* maxint */
  E(cfg_get_long_val(cfg, "abc", &lval));
  ASSRT(lval = 9223372036854775807);

  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, "abc=9223372036854775808", "test4", 7)); /* maxint */
  err = cfg_get_long_val(cfg, "abc", &lval);
  ASSRT(err);
  ASSRT(err->code == CFG_ERR_BAD_NUMBER);

  E(cfg_parse_line(cfg, CFG_MODE_UPDATE, "abc=-2", "test4", 8));
  E(cfg_get_long_val(cfg, "abc", &lval));
  ASSRT(lval == -2);

  E(cfg_delete(cfg));
}  /* test4 */


int main(int argc, char **argv) {
  parse_cmdline(argc, argv);

  if (o_testnum == 0 || o_testnum == 1) {
    test1();
    printf("test1: success\n");
  }

  if (o_testnum == 0 || o_testnum == 2) {
    test2();
    printf("test1: success\n");
  }

  if (o_testnum == 0 || o_testnum == 3) {
    test3();
    printf("test1: success\n");
  }

  if (o_testnum == 0 || o_testnum == 4) {
    test4();
    printf("test1: success\n");
  }

  return 0;
}  /* main */
