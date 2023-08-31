#include <stdio.h>
#include <string.h>
#include "filesystem.h"
#include "interface.h"

/**
 * @brief Parse a command line into arguments
 * 
 * @param cmd_argv The place to store the parsed arguments.
 * @param str The command line to be parsed.
 * @return int The number of arguments
 */
int parse_command(char cmd_argv[MAX_ARGS_COUNT][STRING_SIZE], char *str);