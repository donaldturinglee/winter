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

#define ISVALIDSOCKET(s) ((s) >= 0)

namespace winter {
	class Server {
	public:
		Server() {}
		void start(int port) {
			struct addrinfo hints;
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			struct addrinfo* bind_address;
			getaddrinfo(0, std::to_string(port).c_str(), &hints, &bind_address);

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
			
			std::cout << "Server is listening on port " << port << '\n';
			
			struct sockaddr_storage client_address;
			socklen_t client_length = sizeof(client_address);

			while(true) {
				int client_socket{-1};
				try {
					client_socket = accept(server_socket, (struct sockaddr*)& client_address, &client_length);
					if(!ISVALIDSOCKET(client_socket)) {
						throw std::runtime_error("accept() failed.");
					}
					char request[1024];
					int bytes_received = recv(client_socket, request, sizeof(request), 0);
					if(!ISVALIDSOCKET(bytes_received)) {
						throw std::runtime_error("accept() failed.");
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
	};
} // namespace winter

#endif // SERVER_H
