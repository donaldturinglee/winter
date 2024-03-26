#include "server.h"

namespace winter {
    Server::Server(winter::Router router) : router_(router) {}
    void Server::send_response(int client_socket, std::string response) {
        send(client_socket, response.c_str(), response.size(), 0);
    }
    void Server::start(int port) {
        port_ = port;
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        struct addrinfo* bind_address;
        getaddrinfo(0, std::to_string(port_).c_str(), &hints, &bind_address);

        int server_socket = socket(bind_address->ai_family,
                                   bind_address->ai_socktype,
                                   bind_address->ai_protocol);
        if(!ISVALIDSOCKET(server_socket)) {
            throw std::runtime_error("socket() failed.");
        }

        int option{1};
        if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) < 0) {
            throw std::runtime_error("setsockopt() failed.");
        }

        if(bind(server_socket, bind_address->ai_addr, bind_address->ai_addrlen)) {
            throw std::runtime_error("bind() failed.");
        }
        freeaddrinfo(bind_address);

        // Listening socket queue
        if(listen(server_socket, SOMAXCONN) < 0) {
            throw std::runtime_error("listen() failed.");
        }

        std::cout << "Server is listening on port " << port_ << '\n';

        struct sockaddr_storage client_address;
        socklen_t client_length = sizeof(client_address);

        while(true) {
            int client_socket{-1};
            try {
                client_socket = accept(server_socket, (struct sockaddr*)& client_address, &client_length);
                if(!ISVALIDSOCKET(client_socket)) {
                    throw std::runtime_error("accept() failed.");
                }
                char buffer[1024];
                int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                if(!ISVALIDSOCKET(bytes_received)) {
                    throw std::runtime_error("accept() failed.");
                }
                std::string request(buffer, bytes_received);
                bool route_matched{false};

                int method_end = 0;
                for(int i = 0; i < request.size(); ++i) {
                    if(request[i] == ' ') {
                        method_end = i;
                        break;
                    }
                }

                std::string method = request.substr(0, method_end);

                for(int i = 0; i < router_.get_routes().size(); ++i) {
                    if(request.find(router_.get_routes()[i].get_name()) != std::string::npos) {
                        if(router_.get_routes()[i].get_method() == to_string(method)) {
                            route_matched = true;
                            winter::Request req;
                            winter::Response res(client_socket);
                            router_.get_routes()[i].execute(req, res);
                            break;
                        }
                    }
                }

                if(!route_matched) {
                    std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                    send_response(client_socket, response);
                }

            } catch(const std::runtime_error& ex) {
                std::cerr << ex.what() << '\n';
            }
            if(ISVALIDSOCKET(client_socket)) {
                close(client_socket);
            }
        }
        close(server_socket);
    }
} // namespace winter