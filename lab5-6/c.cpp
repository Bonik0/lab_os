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
    
    while(std::cin >> s){
        if (s == "create"){
            std::cin >> id;
            Node *node = tree.find(id); 

            if (node != nullptr) {
                std::cout << "Error: Already exists" << "\n";
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
                zmqmessage * mes = new zmqmessage({create, 0, id});
                soket.send_message(mes);
                tree.insert(id);
                delete mes;
            }

        }
        if (s == "ping"){
            zmqmessage * mes = new zmqmessage({ping, 0, 0});
            soket.send_message(mes);
            delete mes;
        } 
        if (s == "pingall"){
            zmqmessage * mes = new zmqmessage({pingall, 0, 0});
            soket.send_message(mes);
            soket.recive_message(mes);
            std::vector<int> fork_ids;
            while(mes->perant != 1){
                fork_ids.push_back(mes->id);
                soket.recive_message(mes);
            }
            fork_ids.push_back(mes->id);
            for(int i = 0; i < fork_ids.size(); i++){
                std::cout << fork_ids[i] << " ";
            }
            delete mes;
        }
        if (s == "exec"){
            int n;
            std::cin >> n;
            if(tree.find(n) == nullptr){
                std::cout << "Error: "<< n <<": Not found" << "\n";
                continue;
            }
            zmqmessage * mes = new zmqmessage({exec, 0, n});
            soket.send_message(mes);
            soket.recive_message(mes);
            std::cout << "Ok:" << n <<":" << mes->id << "\n";
            delete mes;
        }
        if (s == "remove"){
            std::cin >> id;
            if (tree.find(id) == nullptr){
                std::cout << "Error: "<< id <<": Not found" << "\n";
                continue;
            }
            if (id == tree.get_root()->id){
                zmqmessage * mes = new zmqmessage({destroy, 0, 0});
                soket.send_message(mes);
                tree.remove_t(id);
                soket.id = -1;
                delete mes;
                continue;
            } 
            zmqmessage * mes = new zmqmessage({remove_t, 0, id});
            soket.send_message(mes);
            tree.remove_t(id);
            delete mes;
        }
    }


    //zmqmessage * mes = new zmqmessage{success, 0, 0};
    //while (true)
    //{
    //    first.send_message(mes);
    //}
    

}