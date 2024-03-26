#include "winter.h"

int main(int argc, char* argv[]) {


	winter::Router router;

	router.get("/hello", [&](winter::Request& request, winter::Response& response){
		std::string message{"Success for hello"};
		response.send(message);
	});

    router.post("/user", [&](winter::Request& request, winter::Response& response){
       response.send( "Success for user");
    });

    router.get("/user", [&](winter::Request& request, winter::Response& response){
        response.send("Success for user");
    });

	winter::Server server(router);
	server.start(8080);

	return 0;
}
