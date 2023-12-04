#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>



int MAX_LENGHT = 1024;



int main(int argc, char** argv) {
    std::string mm_name(argv[1]);
    int fd = shm_open(mm_name.c_str(), O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
    if (fd == -1) {
        perror("shm_open\n");
        exit(-1);
    }
    if (ftruncate(fd, sizeof(char) * MAX_LENGHT) == -1) {
        perror("ftruncate\n");
        exit(-1);
    }
    char* data = (char*) mmap(NULL, (sizeof(char) * MAX_LENGHT), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    for (int i = 1; data[i] != '\n'; ++i) {
        if (data[i] == ' ') {
            data[i] = '_';
        }
    }
    munmap(data, (sizeof(char) * MAX_LENGHT));
    return 0;
}