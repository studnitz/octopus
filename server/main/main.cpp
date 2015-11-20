/*
Simple Server in C/C++
Only able to connect to one client

Part of the 2015/16 BA-Praktikum
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<iostream>
#include <arpa/inet.h>      // INT to IP Conversion

int main(int argc , char *argv[])
{
    int socket_desc; // Server-Sockethandle
    int client_sock; // Client-Sockethandle
    int c;
    int read_size;   // Size of incoming message. if > 0, new message is incoming
    struct sockaddr_in server , client; // Client/Server-structs
    char client_message[2000]; // Buffer for receiving messages

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        std::cout<<"Could not create socket"<<std::endl;
    }
    std::cout<<"Socket created"<<std::endl;

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;            // Akzeptiere Nachrichten von Allen Adresse
    server.sin_port = htons( 3345 );                // Port auf de

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    std::cout<<"Bind done."<<std::endl;

    //Listen
    listen(socket_desc , 3);

    //Accept an incoming connection
    std::cout<<"Waiting for incoming connections..."<<std::endl;
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("Acception failed. Error");
        return 1;
    }
    std::cout<<"Connection accepted"<<std::endl;

    std::cout<<"Client connected: " << inet_ntoa(client.sin_addr)<<std::endl;       // OUTPUT CLIENT IP

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
        std::cout<<client_message<<std::endl;
        if(strncmp(client_message,"TEST",strlen(client_message))==0)

            {
            write(client_sock ,"ACCEPTED", strlen("ACCEPTED"));
        }else{

       write(client_sock ,"NOPE" , strlen("NOPE"));
        }
        memset(&client_message[0], 0, sizeof(client_message)); // Clear message-Buffer
    }

    if(read_size == 0)
    {
        std::cout<<"Client disconnected"<<std::endl;
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}
