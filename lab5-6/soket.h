
#include <iostream>
#include "zmq.h"
#include <cstring>

const int MAIN_id = 4000;
const char * CLIENT_NODE = "cl";
void * context = zmq_ctx_new();

enum actions{
  fail = 0,
  success = 1,
  create = 2,
  destroy = 4,
  ping = 5
};

struct zmqmessage{
    actions act;
    int perant;
    int id;
};


class Soket{
    public:
        void * soket;
        int id;

        Soket(void * context){
            id = -1;
            soket = zmq_socket(context, ZMQ_PAIR);
            
        }
        void bind(int node){
            id = node;
            if(zmq_bind(soket, ("tcp://*:" + std::to_string(MAIN_id + id)).c_str())){
                throw std::runtime_error("ZMQ_bind");
            }
        }
        void unbind(){
            if (id == -1){
                return;
            }
            if(zmq_unbind(soket, ("tcp://*:" + std::to_string(MAIN_id + id)).c_str())){
                throw std::runtime_error("ZMQ_unbind");
            }
            id = -1;
        }

        void connect(int node){
            id = node;
            if(zmq_connect(soket, ("tcp://localhost:" + std::to_string(MAIN_id + id)).c_str())){
                throw std::runtime_error("ZMQ_con");
            }
        }

        void disconnect(){
            if (id == -1){
                return;
            }
            if(zmq_disconnect(soket, ("tcp://*:" + std::to_string(MAIN_id + id)).c_str())){
                throw std::runtime_error("ZMQ_discon");
            }
            id = -1;
        }

        void send_message(const zmqmessage * mes){
            zmq_send(soket, mes, sizeof(zmqmessage), ZMQ_DONTWAIT);
          
        }
        zmqmessage * recive_message(){
            zmqmessage * mes = new zmqmessage();
            zmq_recv(soket, mes, sizeof(zmqmessage), ZMQ_DONTWAIT);
            return mes;
        }
};