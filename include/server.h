#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>
#include <stdexcept>

#include "router.h"

#define ISVALIDSOCKET(s) ((s) >= 0)

namespace winter {
	class Server {
	public:
		Server(Router router) : router_(router) {}
		void send_response(int client_socket, std::string response) {
			send(client_socket, response.c_str(), response.size(), 0);
		}
		void start(int port) {
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

					for(int i = 0; i < router_.get_routes().size(); ++i) {
						if(request.find(router_.get_routes()[i].get_name()) != std::string::npos) {
							route_matched = true;
							winter::Request req;
							winter::Response res(client_socket);
							router_.get_routes()[i].execute(req, res);

							std::string response_message = "Hello, Route: " + router_.get_routes()[i].get_name();
							std::string response = "HTTP/1.1 200 OK\r\n"
												"Content-Length: " + std::to_string(response_message.size())
												+ "\r\n\r\n"
												+ response_message;
							break;
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
	private:
		int port_;
		Router router_;
	};
} // namespace winter

#endif // SERVER_H
