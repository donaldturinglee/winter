#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <functional>
#include "request.h"
#include "response.h"
#include "method.h"

namespace winter {

	class Route {
	public:
		Route(std::string name, Methods method, std::function<void(Request& http_request, Response& http_response)> callback);

		std::string get_name() const;

        Methods get_method() const;

		void execute(Request& http_request, Response& http_response);
	private:
		std::string name_;
        Methods method_;
		std::function<void(Request& http_request, Response& http_response)> callback_;
	};

} // namespace winter

#endif // ROUTE_H
