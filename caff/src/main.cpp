#include <iostream>
#include "CIFF.h"
#include "CAFF.h"
#include "util.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: parser <-caff|-ciff> [path-to-file]" << std::endl;
        return -1;
    }

    std::string option = argv[1];
    std::string filePath = argv[2];
    bool validPath = util::validatePath(filePath);
    if (!validPath){
        return -1;
    }

    if (option == "-caff") {
        std::cout << "Parsing CAFF file: " << filePath << std::endl;
        int res = CAFF::convertFile(
                filePath,
                util::rstrip(filePath, ".caff") + ".webp"
                );
        if (res != 0){
            return -1;
        }
    } else if (option == "-ciff") {
        std::cout << "Parsing CIFF file: " << filePath << std::endl;
        int res = CIFF::convertFile(
                filePath,
                util::rstrip(filePath, ".ciff") + ".webp"
                );
        if (res != 0){
            return -1;
        }

    } else {
        std::cerr << "Invalid option: " << option << std::endl;
        return -1;
    }
    std::cout << "Done!" << std::endl;
    return 0;
}
