#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <functional>
#include "request.h"
#include "response.h"

namespace winter {

	class Route {
	public:
		Route(std::string name, std::function<void(Request& http_request, Response& http_response)> callback)
			: name_(name), callback_(callback) {}

		std::string get_name() const {
			return name_;
		}

		void execute(Request& http_request, Response& http_response) {
			callback_(http_request, http_response);
		}
	private:
		std::string name_;
		std::function<void(Request& http_request, Response& http_response)> callback_;
	};

} // namespace winter;

#endif // ROUTE_H
