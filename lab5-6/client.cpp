
#include "soket.h"

pid_t create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("fork error!\n");
    }
    return pid;
}

void make_node(Soket &child, int id){
    child.bind(id);
    pid_t fork_id = create_process();
    if (fork_id == 0) {
        execl(CLIENT_NODE, CLIENT_NODE, std::to_string(id).c_str(), nullptr);
    }
}

int main(int argc, char ** argv){
    if (argc != 2){
        throw std::logic_error("./NAME_PROGRAMM id");
    } 
    int node_id = std::atoi(argv[1]);

    Soket soket(context);
    soket.connect(node_id);
    Soket left_child(context), right_child(context);
    while(true){
        zmqmessage * command = soket.recive_message();
        
        if (command->act == create){
            if(command->id < node_id && left_child.id == -1){
                make_node(left_child, command->id);
            }
            else if(command->id > node_id && right_child.id == -1){
                make_node(right_child, command->id);
            } 
            else if (command->id < node_id && left_child.id != -1){
                left_child.send_message(command);
            } else {
                right_child.send_message(command);
            }
        }

        if(command->act == ping){
            std::cout << "\n-------------\n";
            std::cout << "IM " << node_id << "\n";
            std::cout << "LEFT CHILD " << left_child.id << "\n";
            std::cout << "RIGHT CHILD " << right_child.id << "\n";
            std::cout << "-------------\n";
            if(left_child.id != -1){
                left_child.send_message(command);
            }
            if(right_child.id != -1){
                left_child.send_message(command);
            }
        }

    }
}