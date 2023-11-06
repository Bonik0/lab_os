#include <unistd.h>
#include <iostream>
#include <cctype>

pid_t create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error!\n");
        exit(-1);
    }
    return pid;
}

void create_pipe(int* pipe_fd) {
    if (pipe(pipe_fd) == -1) {
        perror("pipe error!\n");
        exit(-1);
    }
}

void dup_fd(int oldfd, int newfd) {
    if (dup2(oldfd, newfd) == -1) {
        perror("dup2 error!\n");
        exit(-1);
    }
}

void new_program(const char *name){
    if(execl(name, name, NULL) == -1){
        perror("execl error!\n");
        exit(-1);
    }
}

int main() {
    int pipe1_fd[2], pipe2_fd[2];

    create_pipe(pipe1_fd);
    create_pipe(pipe2_fd);

    pid_t child1 = create_process();
    
    if (child1 == 0) {
        close(pipe1_fd[1]);
        close(pipe2_fd[0]);

        int pipe3_fd[2];
        create_pipe(pipe3_fd);

        pid_t child2 = create_process();

        if (child2 == 0) { 
            close(pipe3_fd[0]);
            close(pipe2_fd[1]);

            dup_fd(pipe1_fd[0], STDIN_FILENO);
            dup_fd(pipe3_fd[1], STDOUT_FILENO);

            new_program("child2");

            close(pipe3_fd[1]);
            close(pipe1_fd[0]);
        } else {    
            close(pipe1_fd[0]);
            close(pipe3_fd[1]);

            dup_fd(pipe3_fd[0], STDIN_FILENO);
            dup_fd(pipe2_fd[1], STDOUT_FILENO);

            new_program("child1");

            close(pipe1_fd[0]);
            close(pipe2_fd[1]);
        }
    } else {   
        close(pipe1_fd[0]);
        close(pipe2_fd[1]);

        char c = getchar();
        char new_c;
        while (c != EOF) {
            write(pipe1_fd[1], &c, sizeof(c));
            read(pipe2_fd[0], &new_c, sizeof(c));
            putchar(new_c);
            c = getchar();
        }

        close(pipe1_fd[1]);
        close(pipe2_fd[0]);
    }
    
    return 0;
}
