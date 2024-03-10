#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <string>
#include <sys/socket.h>

namespace winter {

	class Response {
	public:
		Response(int client_socket) : client_socket_(client_socket) {}
		void send(int status, std::string message) {
			status_ = status;
			std::cout << status_ << " " << message << '\n';
			std::string response = "HTTP/1.1 200 OK\r\n"
								"Content-Length: " + std::to_string(message.size())
								+ "\r\n\r\n"
								+ message;
			::send(client_socket_, response.c_str(), response.size(), 0);
		}
	private:
		int status_;
		int client_socket_;
	};

} // namespace winter

#endif // RESPONSE_H
