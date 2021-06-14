#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 32

// display help info
void print_help()
{
    printf("copy - a simple copying utility\nUsage:\n");
    printf("copy [-m] <file_name> <new_file_name> - copys a file_name to new_file_name (optional mmap mode with -m parameter) \n");
    printf("copy [-h] - print this help message\n");
}

// copying with read() and write()
void copy_read_write(int fd_from, int fd_to)
{
    // create buffer and start reading the old file
    char buf[BUFFER_SIZE];
    ssize_t num_of_bytes = read(fd_from, buf, BUFFER_SIZE);

    // 0 in num_of_bytes indicates EOF
    while (num_of_bytes != 0)
    {
        if (num_of_bytes == -1)
        {
            printf("Error while reading the file\n");
            exit(EXIT_FAILURE);
        }

        write(fd_to, buf, num_of_bytes);
        num_of_bytes = read(fd_from, buf, BUFFER_SIZE);
    }
}

// copying with mmap
void copy_mmap(int fd_from, int fd_to)
{
    // get the size of the file to copy
    off_t file_size = lseek(fd_from, (off_t)0, SEEK_END);

    if (file_size == -1)
    {
        printf("Error while reading the size of file\n");
        exit(EXIT_FAILURE);
    }

    // map file to copy into memory
    void *address_from = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd_from, (off_t)0);

    // resize new file and map it
    lseek(fd_to, file_size - 1, SEEK_SET);
    write(fd_to, "", 1);
    void *address_to = mmap(NULL, file_size, PROT_WRITE, MAP_SHARED, fd_to, (off_t)0);

    if (address_from == (void *)-1 || address_to == (void *)-1)
    {
        printf("Error while mapping the file\n");
        exit(EXIT_FAILURE);
    }

    // copy memory
    memcpy(address_to, address_from, file_size);

    // unmap memory
    munmap(address_from, file_size);
    munmap(address_to, file_size);
}

// function with main functionality of the program
bool copy(bool mode_flag, char *file_to_copy, char *new_file)
{
    // creating file descriptor for old file and error checking
    int fd_from = open(file_to_copy, O_RDONLY);
    if (fd_from == -1)
    {
        printf("Could not open file %s\n", file_to_copy);
        return EXIT_FAILURE;
    }

    // creating file descriptor for new file and error checking
    int fd_to = open(new_file, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd_to == -1)
    {
        printf("Could not create new file %s\n", new_file);
        return EXIT_FAILURE;
    }

    if (!mode_flag)
        copy_read_write(fd_from, fd_to);
    else
        copy_mmap(fd_from, fd_to);

    // close file descriptors
    close(fd_from);
    close(fd_to);
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

    // copy file and exit with the right exit code provided by copy function
    return copy(mode_flag, argv[optind], argv[optind + 1]);
}