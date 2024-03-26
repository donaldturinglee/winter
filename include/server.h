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
#include "request.h"
#include "response.h"

#define ISVALIDSOCKET(s) ((s) >= 0)

namespace winter {
	class Server {
	public:
		Server(Router router);
		void send_response(int client_socket, std::string response);
		void start(int port);
	private:
		int port_;
		Router router_;
	};
} // namespace winter

#endif // SERVER_H
