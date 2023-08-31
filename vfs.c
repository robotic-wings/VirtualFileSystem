#include "vfs.h"

int main()
{
    file_node storage[STORAGE_SIZE];
    // initalize environment variables
    environment env;
    int storage_tail = -1;
    init_root_node(&env.root_node);
    env.pwd = &env.root_node;
    // define user input string
    char user_input[STRING_SIZE];
    char cmd_argv[MAX_ARGS_COUNT][STRING_SIZE];
    int rc;
    while ((rc = scanf("%[^\n]%*c", user_input)) > 0) // read the whole line input
    {
        int argc = parse_command(cmd_argv, user_input);
        /* A handler to make possibly required variable addresses available to a function,
           which represents the context of the current operation. */
        handle hdl = {.argc = &argc, .storage = storage, .storage_tail = &storage_tail, .env = &env};
        if (argc > 1)
        {
            char *cmd = cmd_argv[0];
            // exit
            if (strcmp("exit", cmd) == 0)
            {
                // delete all actual files
                for (int i = 0; i <= storage_tail; i++)
                {
                    if (storage[i].has_content)
                    {
                        char real_file_name[STRING_SIZE];
                        strcpy(real_file_name, storage[i].file_name);
                        strcat(real_file_name, ".file");
                        remove(real_file_name);
                    }
                }
                // exit program
                return 0;
            }
            // touch <filename>
            else if (strcmp("touch", cmd) == 0)
            {
                char filepath[STRING_SIZE];
                strcpy(filepath, cmd_argv[1]);
                create_file_by_path(&hdl, (char *)&filepath, REGULAR_FILE);
            }
            // mkdir <name>
            else if (strcmp("mkdir", cmd) == 0)
            {
                char filepath[STRING_SIZE];
                strcpy(filepath, cmd_argv[1]);
                create_file_by_path(&hdl, (char *)&filepath, DIRECTORY);
            }
            // pwd
            else if (strcmp("pwd", cmd) == 0)
            {
                char pwd_path[STRING_SIZE];
                get_file_path(pwd_path, env.pwd);
                printf("%s\n", pwd_path);
            }
            // cd <name>
            else if (strcmp("cd", cmd) == 0)
            {
                char filepath[STRING_SIZE];
                strcpy(filepath, cmd_argv[1]);
                change_dir(filepath, &env);
            }
            // ls <name>
            else if (strcmp("ls", cmd) == 0)
            {
                char filepath[STRING_SIZE];
                if (strlen(cmd_argv[1]) == 0)
                {
                    // no argument
                    filepath[0] = '\0';
                }
                else
                {
                    strcpy(filepath, cmd_argv[1]);
                }
                ls(filepath, &env);
            }
            // rm <filename>
            else if (strcmp("rm", cmd) == 0)
            {
                char filepath[STRING_SIZE];
                strcpy(filepath, cmd_argv[1]);
                remove_file(filepath, &env, REGULAR_FILE);
            }
            // rmdir <name>
            else if (strcmp("rmdir", cmd) == 0)
            {
                char filepath[STRING_SIZE];
                strcpy(filepath, cmd_argv[1]);
                remove_file(filepath, &env, DIRECTORY);
            }
            // append <filename> "<contents>"
            else if (strcmp("append", cmd) == 0)
            {
                char filepath[STRING_SIZE];
                char content[STRING_SIZE];
                strcpy(filepath, cmd_argv[1]);
                strcpy(content, cmd_argv[2]);
                append_file(filepath, content, &env);
            }
            // cat <filename>
            else if (strcmp("cat", cmd) == 0)
            {
                char filepath[STRING_SIZE];
                strcpy(filepath, cmd_argv[1]);
                print_file(filepath, &env);
            }
        }
    }
    return 0;
}

int parse_command(char cmd_argv[MAX_ARGS_COUNT][STRING_SIZE], char *str)
{
    int str_pos = 0, argc = 1, buffer_pos = 0, spaced = 0;
    // initialize every argument field to an empty string
    for (int i = 0; i < MAX_ARGS_COUNT; i++)
    {
        cmd_argv[i][0] = '\0';
    }
    // initialize parser FSM
    enum
    {
        NONE,
        QUOTED_STRING,
        STRING
    } current_state = NONE;
    while (str[str_pos] != '\0') // FSM reads the command line until EOF
    {
        char c = str[str_pos];
        if (current_state == NONE)
        {
            if (c == '"')
            {
                // transit from NONE to QUOTED_STRING if a double quote is detected
                current_state = QUOTED_STRING;
            }
            else if (c != ' ')
            {
                // transit from NONE to STRING if a space is detected
                current_state = STRING;
                continue; // str_pos does not increase
            }
        }
        else if (current_state == QUOTED_STRING || current_state == STRING)
        {
            if ((current_state == STRING && c == ' ') || (current_state == QUOTED_STRING && c == '"'))
            {
                // transit from STRING to NONE if a space is detected
                // transit from QUOTED_STRING to NONE to an ending double quote is detected
                cmd_argv[argc - 1][buffer_pos++] = '\0';
                argc++;
                buffer_pos = 0;
                current_state = NONE;
            }
            else
            {
                // read the character being pointed into the argument field
                cmd_argv[argc - 1][buffer_pos++] = c;
            }
        }
        str_pos++;
    }
    // add a terminating character to the last unquoted string
    if (current_state == STRING)
    {
        cmd_argv[argc - 1][buffer_pos++] = '\0';
        argc++;
    }
    return argc;
}
