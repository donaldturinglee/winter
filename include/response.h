#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <string>
#include <sys/socket.h>

namespace winter {

	class Response {
	public:
		Response(int client_socket);
		void send(int status, std::string message);
	private:
		int status_;
		int client_socket_;
	};

} // namespace winter

#endif // RESPONSE_H
