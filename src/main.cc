#include "winter.h"

int main(int argc, char* argv[]) {


	winter::Router router;

	router.get("/hello", [&](winter::Request& request, winter::Response& response){
		int status{200};
		std::string message{"Success for hello"};
		response.send(status, message);
	});


	winter::Server server(router);
	server.start(8080);

	return 0;
}
