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
		void get(const std::string& url, std::function<void(Request& http_request, Response& http_response)> callback) {
			Route route(url, callback);
			routes_.push_back(route);
		}
		void post(const std::string& url, std::function<void(Request& http_request, Response& http_response)> callback) {
			Route route(url, callback);
			routes_.push_back(route);
		}
		std::vector<Route> get_routes() {
			return routes_;
		}
		Route find_route(std::string name) {
			std::vector<Route> routes = routes_;
			for(int i = 0; i < routes.size(); ++i) {
				if(routes.at(i).get_name() == name) {
					return routes.at(i);
				}
			}
			throw std::runtime_error("Route not found: " + name);
		}
	private:
		std::vector<Route> routes_;
	};

} // namespace winter

#endif // ROUTER_H
