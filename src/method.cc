#include "method.h"

#include <algorithm>
#include <stdexcept>

namespace winter {
    Methods to_string(std::string method) {
        std::transform(method.begin(), method.end(), method.begin(), ::toupper);
        if(method == "GET") {
            return Methods::kGet;
        } else if(method == "POST") {
            return Methods::kPost;
        } else if(method == "PATCH") {
            return Methods::kPatch;
        } else if(method == "DELETE") {
            return Methods::kDelete;
        } else {
            throw std::invalid_argument("Invalid method");
        }
    }
}