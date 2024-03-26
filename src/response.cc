#include "response.h"

namespace winter {
    Response::Response(int client_socket) : client_socket_(client_socket) {}
    void Response::send(int status, std::string message) {
        status_ = status;
        std::cout << status_ << " " << message << '\n';
        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Length: " + std::to_string(message.size())
                               + "\r\n\r\n"
                               + message;
        ::send(client_socket_, response.c_str(), response.size(), 0);
    }
} // namespace winter