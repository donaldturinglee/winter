#include "route.h"

namespace winter {
    Route::Route(std::string name, winter::Methods method, std::function<void(Request &, Response &)> callback) : name_(name), method_(method), callback_(callback) {}

    std::string Route::get_name() const {
        return name_;
    }

    Methods Route::get_method() const {
        return method_;
    }

    void Route::execute(winter::Request &http_request, winter::Response &http_response) {
        callback_(http_request, http_response);
    }
} // namespace winter