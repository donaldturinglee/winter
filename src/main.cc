#include "winter.h"

int main(int argc, char* argv[]) {

	winter::Server server;
	server.start(9001);

	return 0;
}
