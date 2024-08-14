#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <functional>
#include "engine/chat_client_engine.hpp"



int main(int argc, char* argv[]) {

    // Read main values: IP, PORT
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT>\n";
        return 1;
    }
    

    std::string ip = argv[1];
    int port = std::stoi(argv[2]);




    ChatClientEngine client(ip, port);
    client.setMessageCallback([](const ChatClientMessage& message) {
        std::cout << "Message from " << message.nickname << " [" << message.address << "]: " << message.text << "\n";
    });

    client.start();

    std::string username;
    std::cout << "Enter your username: ";
    
    std::getline(std::cin, username);
    for (char c : username) {
        if (!isalnum(c)) {
            std::cerr << "Username must be alphanumeric.\n";
            return -1;
        }
    }

    client.setUsername(username);

    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "/quit") {
            break;
        }
        client.sendMessage(input);
    }

    client.stop();

    return 0;
}
