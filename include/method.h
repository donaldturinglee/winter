#ifndef WINTER_METHOD_H
#define WINTER_METHOD_H

#include <string>

namespace winter {
enum Methods {
    kGet,
    kPost,
    kPatch,
    kDelete
};

Methods to_string(std::string method);

} // namespace winter

#endif //WINTER_METHOD_H
