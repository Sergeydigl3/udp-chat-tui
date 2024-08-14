#pragma once

#include <cstring>
#include <functional>
#include <thread>
#include <string>

#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1007
#define NICKNAME_SIZE 100

struct ChatClientMessage {
    std::string address;
    std::string nickname;
    std::string text;
};

class ChatClientEngine
{
public:
    ChatClientEngine(const std::string &ip, int port) : ip_(ip), port_(port) {};

    void start();

    void stop();

    void setUsername(const std::string &username);

    void sendMessage(const std::string &message);

    void setMessageCallback(std::function<void(const ChatClientMessage &)> callback);

private:
    std::string ip_;
    int port_;
    int socket_;
    struct sockaddr_in broadcastAddr_;
    std::string username_;
    std::thread recv_thread_;
    std::function<void(const ChatClientMessage &)> message_callback_;

    void receiveThread();
};
