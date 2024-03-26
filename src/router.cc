#include "router.h"

namespace winter {
    void Router::get(const std::string &url, std::function<void(Request &, Response &)> callback) {
        Route route(url, Methods::kGet, callback);
        routes_.push_back(route);
    }
    void Router::post(const std::string &url, std::function<void(Request &, Response &)> callback) {
        Route route(url, Methods::kPost, callback);
        routes_.push_back(route);
    }
    std::vector<Route> Router::get_routes() {
        return routes_;
    }
    Route Router::find_route(std::string name) {
        std::vector<Route> routes = routes_;
        for(int i = 0; i < routes.size(); ++i) {
            if(routes.at(i).get_name() == name) {
                return routes.at(i);
            }
        }
        throw std::runtime_error("Route not found: " + name);
    }
} // namespace winter