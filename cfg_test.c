/* err_tst.c - self-test.
 *
 * This code and its documentation is Copyright 2019, 2019 Steven Ford,
 * http://geeky-boy.com and licensed "public domain" style under Creative
 * Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
 * To the extent possible under law, the contributors to this project have
 * waived all copyright and related or neighboring rights to this work.
 * In other words, you can use this code for any purpose without any
 * restrictions. This work is published from: United States. The project home
 * is https://github.com/fordsfords/err
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

#define E(e_test) do { \
  if ((e_test) != ERR_OK) { \
    fprintf(stderr, "ERROR [%s:%d]: '%s' returned -1\n", __FILE__, __LINE__, #e_test); \
    exit(1); \
  } \
} while (0)

#define ASSRT(assrt_cond) do { \
  if (! (assrt_cond)) { \
    fprintf(stderr, "ERROR [%s:%d]: assert '%s' failed\n", __FILE__, __LINE__, #assrt_cond); \
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
    "For details, see https://github.com/fordsfords/err\n",
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

  E(cfg_create(&cfg));

  E(cfg_parse_line(cfg, "", "test1a", 1));
  ASSRT(cfg->option_vals->num_entries == 0);
  E(cfg_parse_line(cfg, "   ", "test1a", 2));
  ASSRT(cfg->option_vals->num_entries == 0);
  E(cfg_parse_line(cfg, "#  ", "test1a", 3));
  ASSRT(cfg->option_vals->num_entries == 0);
  E(cfg_parse_line(cfg, " # ", "test1a", 4));
  ASSRT(cfg->option_vals->num_entries == 0);

  E(cfg_parse_line(cfg, "aaa", "test1a", 5));
  ASSRT(cfg->option_vals->num_entries == 1);
  E(hmap_lookup(cfg->option_vals, "aaa", 3, (void **)&val));
  ASSRT(strlen(val) == 0);
  ASSRT(cfg->option_locations->num_entries == 1);
  E(hmap_lookup(cfg->option_locations, "aaa", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:5") == 0);

  E(cfg_parse_line(cfg, " aab # ", "test1a", 6));
  ASSRT(cfg->option_vals->num_entries == 2);
  E(hmap_lookup(cfg->option_vals, "aab", 3, (void **)&val));
  ASSRT(strlen(val) == 0);
  ASSRT(cfg->option_locations->num_entries == 2);
  E(hmap_lookup(cfg->option_locations, "aab", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:6") == 0);

  E(cfg_parse_line(cfg, "aac 113", "test1a", 7));
  ASSRT(cfg->option_vals->num_entries == 3);
  E(hmap_lookup(cfg->option_vals, "aac", 3, (void **)&val));
  ASSRT(strcmp(val, "113") == 0);
  ASSRT(cfg->option_locations->num_entries == 3);
  E(hmap_lookup(cfg->option_locations, "aac", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:7") == 0);

  E(cfg_parse_line(cfg, "  aad 114  #  xyz", "test1a", 8));
  ASSRT(cfg->option_vals->num_entries == 4);
  E(hmap_lookup(cfg->option_vals, "aad", 3, (void **)&val));
  ASSRT(strcmp(val, "114") == 0);
  ASSRT(cfg->option_locations->num_entries == 4);
  E(hmap_lookup(cfg->option_locations, "aad", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:8") == 0);

  /* Overwrite previous one. */
  E(cfg_parse_line(cfg, " aab  112#", "test1a", 9));
  ASSRT(cfg->option_vals->num_entries == 4);
  E(hmap_lookup(cfg->option_vals, "aab", 3, (void **)&val));
  ASSRT(strcmp(val, "112") == 0);
  ASSRT(cfg->option_locations->num_entries == 4);
  E(hmap_lookup(cfg->option_locations, "aab", 3, (void **)&val));
  ASSRT(strcmp(val, "test1a:9") == 0);

  /* Illegal. */
  err = cfg_parse_line(cfg, " aab  112 xyz", "test1a", 9);
  ASSRT(err);
  ASSRT(err->);
  ASSRT(cfg->option_vals->num_entries == 4);

  E(cfg_delete(cfg));
}  /* test1 */


int main(int argc, char **argv) {
  parse_cmdline(argc, argv);

  if (o_testnum == 0 || o_testnum == 1) {
    test1();
    printf("test1: success\n");
  }

  return 0;
}  /* main */
