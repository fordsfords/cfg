/* example.c - demonstrate use of "cfg" object. */

#include <stdio.h>
#include "err.h"
#include "hmap.h"
#include "cfg.h"


/* These are the configuraiton options defined and their
 * default values for this example program.
 */
char *valid_cfg_options[] = {
  "my_max_length = 1024",
  "my_tag_name=",             /* Default: empty string. */
  NULL
};


ERR_F cfg_example_app()
{
  cfg_t *cfg;
  ERR(cfg_create(&cfg));

  /* Define valid keys and their default values. */
  ERR(cfg_parse_string_list(cfg, CFG_MODE_ADD, valid_cfg_options));

  /* Read in config file (probably not a fixed name). */
  ERR(cfg_parse_file(cfg, CFG_MODE_UPDATE, "my_config_file.txt"));

  /* Retrieve values. */
  long my_max_length;
  ERR(cfg_get_long_val(cfg, "my_max_length ", &my_max_length));

  char *my_tag_name;
  ERR(cfg_get_str_val(cfg, "my_tag_name", &my_tag_name));

  printf("my_max_length=%ld, my_tag_name='%s'", my_max_length, my_tag_name);
  return ERR_OK;
}  /* cfg_example_app */


int main(int argc, char **argv)
{
  (void)argc;  (void)argv;  /* Unused parameters. */

  /* If error returns to outer-most main, abort to dump core.
   * Alternatively could use ERR_EXIT_ON_ERR(). */
  ERR_ABRT_ON_ERR(cfg_example_app(), stderr);

  return 0;
}  /* main */
