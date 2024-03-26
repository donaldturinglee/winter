#include "response.h"
#include <fstream>
#include <vector>
#include <unordered_map>

namespace winter {
    std::unordered_map<std::string, std::string> mime_types = {
            {".jar", "application/java-archive"},
            {".x12", "application/EDI-X12"},
            {".edi", "application/EDIFACT"},
            {".js", "application/javascript"},
            {".bin", "application/octet-stream"},
            {".ogg", "application/ogg"},
            {".pdf", "application/pdf"},
            {".xhtml", "application/xhtml+xml"},
            {".swf", "application/x-shockwave-flash"},
            {".json", "application/json"},
            {".jsonld", "application/ld+json"},
            {".xml", "application/xml"},
            {".zip", "application/zip"},
            {".url", "application/x-www-form-urlencoded"},
            {".mp3", "audio/mpeg"},
            {".wma", "audio/x-ms-wma"},
            {".ra", "audio/vnd.rn-realaudio"},
            {".wav", "audio/x-wav"},
            {".gif", "image/gif"},
            {".jpeg", "image/jpeg"},
            {".jpg", "image/jpeg"},
            {".png", "image/png"},
            {".tiff", "image/tiff"},
            {".txt", "text/plain"},
            {".html", "text/html"}
    };
    Response::Response(int client_socket) : client_socket_(client_socket) {}
    void Response::send(std::string message) {
        //status_ = status;
        std::cout << status_ << " " << message << '\n';
        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Length: " + std::to_string(message.size())
                               + "\r\n\r\n"
                               + message;
        ::send(client_socket_, response.c_str(), response.size(), 0);
    }
    void Response::send_file(std::string path) {
        std::ifstream is(path, std::ios::binary);
        if(!is.is_open()) {
            std::cerr << "Failed to open file\n";
            return;
        }

        // get the file extension
        std::string file_extension = path.substr(path.rfind('.'));

        // get the content-type
        auto mime_type = mime_types.find(file_extension);

        is.seekg(0, std::ios::end);
        std::size_t file_size = is.tellg();
        is.seekg(0, std::ios::beg);

        std::vector<char> buffer(file_size);
        is.read(buffer.data(), file_size);

        // craft the response message
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: ";
        if(mime_type != mime_types.end()) {
            response += mime_type->second;
        } else {
            std::cerr << "Unsupported file version\n";
        }
        response += "\r\nContent-Length: " + std::to_string(file_size) + "\r\n\r\n";
        response += std::string(buffer.begin(), buffer.end());
        ::send(client_socket_, response.c_str(), response.size(), 0);
        is.close();
    }
} // namespace winter