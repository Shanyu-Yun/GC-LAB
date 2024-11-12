
#include "tool.hpp"

namespace toy2d {

std::vector<char> ReadWholeFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary|std::ios::ate);

    if (!file.is_open()) {
        throw std::runtime_error("failed to read shader file");
        return std::vector<char>{};
    }

    auto size = file.tellg();
    std::vector<char> content(size);
    file.seekg(0);

    file.read(content.data(), content.size());

    return content;
}

}