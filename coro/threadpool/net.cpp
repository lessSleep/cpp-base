#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#include <cstring> //strlen
#include <string.h>  //strlen
#include <unistd.h>  //close

#include "ThreadPool.h"
//  g++ net.cpp -lpthread
void block() {
    // create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // specify an address for the socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    // bind the socket to our specified IP and port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    // listen for incoming connections
    listen(server_fd, 3);
    
    // accept incoming connections
    int client_fd = accept(server_fd, NULL, NULL);
    
    // send a message to the client
    const char *message = "Hello, client!";
    send(client_fd, message, strlen(message), 0);
    
    // close the sockets
    close(client_fd);
    close(server_fd);

}

int main() {

    ThreadPool pool(4);

    // enqueue and store future
    auto result  = pool.enqueue([](int answer) { return answer; }, 42);

    auto result1 = pool.enqueue([](int answer) { return answer; }, 55);

    // get result from future
    std::cout << result.get() << std::endl;

    std::cout << result1.get() << std::endl;

    // std::this_thread::sleep_for(std::chrono::seconds(5)); //这种方法不完美，总有运气成分。
    block(); //完美的方法，也不占用cpu。
    
    return 0;
}