#ifndef CIFF_HPP
#define CIFF_HPP

#include "CAFF.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <optional>


namespace CIFF {
    const char HEADER_MAGIC[4] = {'C', 'I', 'F', 'F'};
    struct CIFF {
        char magic[4];
        const uint64_t headerSize;
        const uint64_t contentSize;
        const uint64_t width;
        const uint64_t height;
        const std::string caption;
        const std::vector<std::string> tags;
        const char* content;
    
        CIFF(const char* _magic, uint64_t _headerSize, uint64_t _contentSize,
                   uint64_t _width, uint64_t _height, const std::string& _caption,
                   const std::vector<std::string>& _tags, const char* _content):
            headerSize(_headerSize), contentSize(_contentSize), width(_width),
              height(_height), caption(_caption), tags(_tags), content(_content){
            std::memcpy(magic, _magic, sizeof(magic));
        }
    };
    
    std::optional<CIFF> parseCIFF(const char* data, const uint64_t size);

    void printCIFFInfo(const CIFF ciff);

    void toWebp(const CIFF ciff, std::string path);

    int convertFile(std::string inPath, std::string outPath);
}

#endif
