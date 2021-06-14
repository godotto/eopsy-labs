#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

// display help info
void print_help()
{
    printf("copy - a simple copying utility\nUsage:\n");
    printf("copy [-m] <file_name> <new_file_name> - copys a file_name to new_file_name (optional mmap mode with -m parameter) \n");
    printf("copy [-h] - print this help message\n");
}

int main(int argc, char *argv[])
{
    char param;             // getopt option
    bool mode_flag = false; // selected mode (with or without -m)

    while ((param = getopt(argc, argv, ":hm")) != -1)
    {
        switch (param)
        {
            // if -h, print help info
            case 'h':
                print_help();
                return EXIT_SUCCESS;
            // if -m, mark mmap mode 
            case 'm':
                mode_flag = true;
                break;
            // print help info and return failure code when argument does not exist
            default:
                printf("Wrong argument\n");
                print_help();
                return EXIT_FAILURE;
        }
    }

    // if number of arguments is incorrect, print help info end terminate with failure code
    if (argc - optind != 2)
    {
        printf("Wrong number of arguments\n");
        print_help();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}