#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>


int MAX_LENGHT = 1024;

void new_program(const char *name,const char* argv){
    if(execl(name, name, argv, NULL) == -1){
        perror("execl error!\n");
        exit(-1);
    }
}

int create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork error!\n");
        exit(-1);
    }
    return pid;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        perror("Too few arguments. Usage: ./lab03 NAME_OF_FILE");
        exit(1);
    }
    std::string mm_name(argv[1]);
    int fd = shm_open(mm_name.c_str(), O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
    if (fd == -1) {
        perror("shm_open\n");
        exit(1);
    }
    if (ftruncate(fd, sizeof(char) * MAX_LENGHT) == -1) {
        perror("ftruncate\n");
        exit(1);
    }
    char* data = (char*) mmap(NULL, (sizeof(char) * MAX_LENGHT), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char c = getchar();
    int i = 0;
    while (c != EOF && i < MAX_LENGHT && c != '\n') {
        data[i] = c;
        i++;
        c = getchar();
    }
    data[i] = '\n';  
    pid_t pid = create_process();
    if (pid == 0) {     // child 1
        new_program("../build/child1", mm_name.c_str());
    } else {    // parent
        wait(0);
        for (int i = 0; data[i] != '\n'; ++i) {
            putchar(data[i]);
        }
        putchar('\n');
        munmap(data, (sizeof(char) * MAX_LENGHT));
        int err = shm_unlink(mm_name.c_str());
        if(err == -1){
            perror("shm_unlink");
            exit(-1);
        }

    }

    return 0;
}