//Source code for tcp socket creation and creating,deleting,writing to shared memory.  
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<sys/shm.h>
#include<iostream>
#define KEYFILE "/home/cguser15/may17/src/keyfile"
//create shared memory to store no.of clients 
void makeShm(){
        key_t key;
	int shm_id;
	short* no_of_clients;
	key = ftok(KEYFILE,65);
        shm_id = shmget(key,1,0666 | IPC_CREAT );
        if(shm_id == -1){
                perror("error creating shared memory");
		exit(EXIT_FAILURE);
        }

        //shmat() function returns the address of shared memory
        no_of_clients = (short*)shmat(shm_id,(void*)0,0);
	
	//set initial connection value to 0
	*no_of_clients = 0;

	//detach the shared memory
	//shmdt(no_of_clients);
}

//delete shared memory
void deleteShm(){
	key_t key;
	int shmid;
	key = ftok(KEYFILE,65);
	shmid = shmget(key,1,0666);
        shmctl(shmid,IPC_RMID,0);
}

//writing to shared memory
short incrementConnection(){
        key_t key;
	int shm_id;
	key = ftok(KEYFILE,65);
        shm_id = shmget(key,1,0666 );
        if(shm_id == -1){
                perror("Error Accessing Shared Memory");
                exit(EXIT_FAILURE);
        }
        //shmat return the address of shared memory
        short* no_of_clients = (short*)shmat(shm_id,(void*)0,0);
        *no_of_clients+=1;

	//detach shared memory
	//shmdt(no_of_clients);

	//returning the value stored in shared memory
	return *no_of_clients;
}

//creating a socket for TCP server and returning the socket file descriptor
int createSocket(const char* ip,int port){
    //create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket error");
        return -1;
    }
    std::cout << "socket created" << std::endl;

    //create server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    //binding the ip and port to server socket
    int ret = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret < 0){
        perror("bind error");
        return -1;
    }
    std::cout << "IP and PORT binded to server address" << std::endl;

    //listen for client
    ret = listen(sock, 5);
    if(ret < 0){
        perror("listen error");
        return -1;
    }
    std::cout << "server started, will receive incoming connection on "<< port << " port" << std::endl;
    return sock;
}
