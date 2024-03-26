#ifndef ROUTER_H
#define ROUTER_H

#include "route.h"
#include <string>
#include <vector>
#include <functional>
#include "request.h"
#include "response.h"

namespace winter {

	class Router {
	public:
		Router() {}
		void get(const std::string& url, std::function<void(Request& http_request, Response& http_response)> callback);
		void post(const std::string& url, std::function<void(Request& http_request, Response& http_response)> callback);
		std::vector<Route> get_routes();
		Route find_route(std::string name);
	private:
		std::vector<Route> routes_;
	};

} // namespace winter

#endif // ROUTER_H
