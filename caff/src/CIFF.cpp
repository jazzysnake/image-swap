#include "util.h"
#include "CIFF.h"
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "../include/webp/encode.h"

void CIFF::printCIFFInfo(const CIFF ciff) {
    std::cout << "###CIFF obj###" << '\n';
    std::cout << "\tmagic: " << ciff.magic << '\n';
    std::cout << "\theaderSize: " << ciff.headerSize << '\n';
    std::cout << "\tcontentSize: " << ciff.contentSize << '\n';
    std::cout << "\twidth: " << ciff.width << '\n';
    std::cout << "\theight: " << ciff.height << '\n';
    std::cout << "\tcaption: " << ciff.caption << '\n';
    std::cout << "\ttags: ";
    for (const std::string& tag : ciff.tags) {
        std::cout << tag << ' ';
    }
    std::cout << "\n#############\n";
}

std::optional<CIFF::CIFF> CIFF::parseCIFF(const char* data, const uint64_t size){
    char magic[4];
    std::memcpy(magic, data, 4);
    if(!util::compareArrays(HEADER_MAGIC, magic, 4)){
        std::cerr << "Header magic mismatch!" << std::endl;
        return std::nullopt;
    }

    uint64_t headerSize = util::convertLittleEndianToUint(&data[4]);
    uint64_t contentSize = util::convertLittleEndianToUint(&data[12]);
    uint64_t width = util::convertLittleEndianToUint(&data[20]);
    uint64_t height = util::convertLittleEndianToUint(&data[28]);

    if(contentSize != width*height*3 ||
        // width and height can be zero but no pixels must be present
        (width*height == 0 && size != headerSize+contentSize)){
        std::cerr << "Malformed CIFF size data!" << std::endl;
        return std::nullopt;
    }
    // size of header - (size data + magic)
    uint64_t captionAndTagsSize = headerSize - (4*8+4); 
    auto res = util::indexOfNewline(&data[36], captionAndTagsSize);
    if(!res.has_value()){
        std::cerr << "No newline terminated caption was found!" << std::endl;
        return std::nullopt;
    }
    int newLineIndex = res.value();
    std::string caption = std::string(&data[36], newLineIndex);
    auto res1 = util::parseTags(
            &data[36+newLineIndex+1], captionAndTagsSize-newLineIndex-1
            );
    if(!res1.has_value()){
        std::cerr << "No null-terminated tags were found!" << std::endl;
        return std::nullopt;
    }
    std::vector<std::string> tags = res1.value();

    CIFF ciff(magic, headerSize, contentSize, width, height, caption, tags, &data[headerSize]);
    // TODO debug print
    //printCIFFInfo(ciff);
    return ciff;
}

void CIFF::toWebp(const CIFF ciff, std::string path){
    uint8_t* outputData;
    uint64_t webpSize = WebPEncodeRGB(
            reinterpret_cast<const uint8_t*>(ciff.content),
            ciff.width,
            ciff.height,
            ciff.width * 3,
            100,
            &outputData
            );
    FILE* outputFile = fopen(path.c_str(), "wb");
    fwrite(outputData, 1, webpSize, outputFile);
    fclose(outputFile);
    WebPFree(outputData);
}

int CIFF::convertFile(std::string inPath, std::string outPath){
    auto res = util::readFile(inPath);
    if (!res.has_value()){
        return -1;
    }
    auto res1 = parseCIFF(res.value().data(), res.value().size());
    if (!res1.has_value()){
        return -1;
    }
    CIFF ciff = res1.value();
    toWebp(ciff, outPath);

    return 0;
}
