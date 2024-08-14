#include <iostream>
#include "chat_client_engine.hpp"
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// ChatClientEngine::ChatClientEngine(const std::string &ip, int port) : ip_(ip), port_(port) {}

void ChatClientEngine::start()
{
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ == -1)
    {
        std::cerr << "Failed to create socket.\n";
        return;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        std::cerr << "Failed to bind socket.\n";
        return;
    }

    broadcastAddr_.sin_family = AF_INET;
    broadcastAddr_.sin_port = htons(port_);
    broadcastAddr_.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    int broadcastPermission = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) == -1)
    {
        std::cerr << "Failed to set broadcast permission.\n";
        return;
    }

    recv_thread_ = std::thread(&ChatClientEngine::receiveThread, this);
}

void ChatClientEngine::stop()
{
    if (recv_thread_.joinable()) {
        recv_thread_.detach();
    }

    if (socket_ != -1) {
        close(socket_);
        socket_ = -1;
    }
}

void ChatClientEngine::setUsername(const std::string &username)
{
    if (username.size()>NICKNAME_SIZE) {
        std::cerr << "Username too long.\n";
        return;
    }
    // Check username is only alphanumeric
    for (char c : username) {
        if (!isalnum(c)) {
            std::cerr << "Username must be alphanumeric.\n";
            return;
        }
    }
    username_ = username;
}

void ChatClientEngine::sendMessage(const std::string &message)
{
    if (!username_.empty() && !message.empty())
    {   
        if (message.size()>MAX_BUFFER_SIZE) {
            std::cerr << "Message too long.\n";
            return;
        }
        std::string full_message = username_ + ": " + message;
        if (sendto(socket_, full_message.c_str(), full_message.size(), 0, (struct sockaddr *)&broadcastAddr_, sizeof(broadcastAddr_)) == -1)
        {
            std::cerr << "Failed to send message.\n";
        }
    }
}

void ChatClientEngine::setMessageCallback(std::function<void(const ChatClientMessage &)> callback)
{
    message_callback_ = std::move(callback);
}

void ChatClientEngine::receiveThread()
{
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in senderAddr;
    socklen_t senderAddrSize = sizeof(senderAddr);

    while (true)
    {
        memset(buffer, 0, MAX_BUFFER_SIZE);
        if (recvfrom(socket_, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&senderAddr, &senderAddrSize) == -1)
        {
            std::cerr << "Failed to receive message.\n";
            continue;
        }

        std::string address = inet_ntoa(senderAddr.sin_addr);

        if (address == ip_)
        { // Ignore messages from self
            continue;
        }

        std::string message(buffer);
        std::string nickname = message.substr(0, message.find(':'));
        std::string text = message.substr(message.find(':') + 2); // Skip ": " after nickname

        ChatClientMessage received_message{address, nickname, text};
        if (message_callback_)
        {
            message_callback_(received_message);
        }
    }
}
