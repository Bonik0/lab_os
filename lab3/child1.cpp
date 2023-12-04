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
    for (int i = 0; data[i] != '\n'; i++) {
        data[i] = toupper(data[i]);
    }
    int pid = create_process();
    if (pid == 0) {     // child 2
        new_program("../build/child2", argv[1]);
    } else {    // child 1
        wait(0);
    }
    munmap(data, (sizeof(char) * MAX_LENGHT));
    return 0;
}