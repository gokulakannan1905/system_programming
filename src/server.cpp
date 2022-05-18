//This program accepts the incoming tcp clients and provides connection to tcp server 

#include <iostream>
#include "../include/func.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<sys/shm.h>

using namespace std;

int main(int argc, char* argv[]){

	//creating a shared memory to store the total clients connected to the server
	makeShm();
	
	//creating a socket
	int sock = createSocket("0.0.0.0",8015);

	//creating a struct to store client address
	struct sockaddr_in client_addr;

	//storing the length of the client address 
    	socklen_t client_addr_len = sizeof(client_addr);

	//accept incoming clients and create new process for each client
    	while(1){
        int client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_addr_len);
	if(client_sock < 0){
            perror("accept error");
            return -1;
        }
        std::cout << "client accepted successfully" << std::endl;

	//incrementing total number of clients stored in shared memory
        short total_clients_connected = incrementConnection();
	cout << "Total Number of clients connected to server : " << total_clients_connected << endl;

        //fork a new process
        pid_t pid = fork();
        if(pid < 0){
            perror("fork error");
            return -1;
        }
        else if(pid == 0){
            //child process
            close(sock);
            char buf[1024];
            while(1){
                memset(buf, 0, sizeof(buf));
		//receive from the client and store in buffer
                int len = recv(client_sock, buf, sizeof(buf), 0);
                if(len < 0){
                    perror("recv error");
                    return -1;
                }
                else if(len == 0){
                    std::cout << "client closed" << std::endl;
                    break;
                }
                else{
                    std::cout << "client send: " << buf << std::endl;
			if(strcmp(buf,"Bye")==0 || strcmp(buf,"bye")==0){
				std::cout<< "Client went offline" << std::endl;
				close(client_sock);
				exit(0);
			}
                    memset(buf,0,sizeof(buf));
                    
		    //read from stdin and send to client
                    std::cout << "please input: ";
                    std::cin.getline(buf,sizeof(buf));

		    //send the message to client
                    send(client_sock, buf, strlen(buf), 0);
                }
            }
	    //close client socket
            close(client_sock);
            exit(0);
        }
        else{
            //parent process
            close(client_sock);
        }
    }
    //delete the shared memory
    deleteShm();

    //close the server socket
    close(sock);
}
