#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <elf.h> // Include the ELF header file if required

void check_elf(unsigned char *e_ident);
void print_magic(unsigned char *e_ident);
void print_class(unsigned char *e_ident);
void print_data(unsigned char *e_ident);
void print_version(unsigned char *e_ident);
void print_abi(unsigned char *e_ident);
void print_osabi(unsigned char *e_ident);
void print_type(unsigned int e_type, unsigned char *e_ident);
void print_entry(unsigned long int e_entry, unsigned char *e_ident);
void close_elf(int elf);

// ...

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        exit(98);
    }

    const char *filename = argv[1];
    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        perror("Error opening file");
        exit(98);
    }

    Elf64_Ehdr header; // Use Elf64_Ehdr if you are working with ELF64 files
    ssize_t bytes_read = read(fd, &header, sizeof(Elf64_Ehdr));

    if (bytes_read != sizeof(Elf64_Ehdr) ||
        strncmp((char *)header.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "%s is not an ELF file\n", filename);
        close_elf(fd);
        exit(98);
    }

    check_elf(header.e_ident);
    printf("ELF Header:\n");
    print_magic(header.e_ident);
    print_class(header.e_ident);
    print_data(header.e_ident);
    print_version(header.e_ident);
    print_osabi(header.e_ident);
    print_abi(header.e_ident);
    print_type(header.e_type, header.e_ident);
    print_entry(header.e_entry, header.e_ident);

    close_elf(fd);

    return 0;
}
