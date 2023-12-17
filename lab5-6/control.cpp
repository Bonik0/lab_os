#include "soket.h"
#include "AVL-tree.hpp"



pid_t create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("fork error!\n");
    }
    return pid;
}

int main(){
    Soket soket(context);
    AVL_tree tree;
    std::string s;
    int id;
    while(std::cin >> s >> id){
        if (s == "create"){
            Node *node = tree.find(id); 

            if (node != nullptr) {
                std::cout << "Node run" << "\n";
                continue;
            }
            if (tree.get_root() == nullptr){
                tree.insert(id);
                soket.bind(id);
                pid_t pid = create_process();
                if (pid == 0){
                    execl(CLIENT_NODE, CLIENT_NODE, std::to_string(id).c_str(), nullptr);     
                }
                
            } else {
                zmqmessage * mes = new zmqmessage{create, 0, id};
                soket.send_message(mes);
                tree.insert(id);
            }

        }
        if (s == "ping"){
            zmqmessage * mes = new zmqmessage{ping, 0, 0};
            soket.send_message(mes);

        }
    }


    //zmqmessage * mes = new zmqmessage{success, 0, 0};
    //while (true)
    //{
    //    first.send_message(mes);
    //}
    

}