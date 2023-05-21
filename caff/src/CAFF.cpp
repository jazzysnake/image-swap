#include "CAFF.h"
#include "CIFF.h"
#include "util.h"

#include <optional>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <sstream>

void CAFF::printBlockInfo(const Block block, bool truncateOutput) {
    std::cout << "###Block obj###" << '\n';
    std::cout << "\tid: " << static_cast<int>(block.id) << '\n';
    std::cout << "\tsize: " << block.size << '\n';
    std::cout << "\tdata:\n";
    for (size_t i=0; i<block.size; i++){
        if(i>=block.size || (i>64 && truncateOutput)){
            break;
        }
        if (i%4==0){
            std::cout << "\t";
        }
        char toPrint;
        std::memcpy(&toPrint, &block.data.data()[i],1);
        std::cout << static_cast<unsigned int>(toPrint) << " ";
        if (i%4==3 && i!=block.size-1){
            std::cout << "\n";
        }
    }
    std::cout << "\n#############\n";
}

void CAFF::printHeaderInfo(const HeaderBlock header) {
    std::cout << "###HeaderBlock obj###" << '\n';
    std::cout << "\tmagic: " << header.magic << '\n';
    std::cout << "\tsize: " << header.size << '\n';
    std::cout << "\tnumPics : " << header.numPics << '\n';
    std::cout << "#############\n";
}

std::optional<CAFF::HeaderBlock> CAFF::parseHeaderBlock(const Block block) {
    const char* data = block.data.data();
    char magic[4];
    std::memcpy(magic, data, 4);
    uint64_t size = util::convertLittleEndianToUint(&data[4]);
    uint64_t numPics = util::convertLittleEndianToUint(&data[12]);

    if(size!=block.size || !util::compareArrays(magic, CAFF::HEADER_MAGIC, 4)){
        return std::nullopt;
    }

    HeaderBlock header(magic, size, numPics);
    printHeaderInfo(header);
    return header;
}

void CAFF::printCreditsBlockInfo(const CAFF::CreditsBlock& credits) {
    std::cout << "###CreditsBlock obj###" << '\n';
    std::cout << "\tcreationDate: ";
    util::printDate(credits.creationDate);
    std::cout << "\tcreatorSize: " << credits.creatorSize << '\n';
    std::cout << "\tcreator: " << credits.creator << '\n';
    std::cout << "#############\n";
}

std::optional<CAFF::CreditsBlock> CAFF::parseCreditsBlock(const CAFF::Block block){
    const char* data = block.data.data();
    char dateBytes[6];
    std::memcpy(dateBytes, data, 6);
    util::Date date = util::parseDate(dateBytes);

    uint64_t creatorSize = util::convertLittleEndianToUint(&data[6]);

    char creatorBytes[creatorSize];
    std::memcpy(creatorBytes, (data+6+8), creatorSize);
    CAFF::CreditsBlock creditBlock(
            date,
            creatorSize,
            std::string(creatorBytes, creatorSize)
            );
    printCreditsBlockInfo(creditBlock);
    return creditBlock;
}

std::optional<CAFF::AnimationBlock> CAFF::parseAnimationBlock(const CAFF::Block block){
    const char* data = block.data.data();
    uint64_t duration = util::convertLittleEndianToUint(data);

    uint64_t ciffSize = block.size-sizeof(uint64_t);
    auto ciffData = std::vector<char>(ciffSize);
    std::memcpy(ciffData.data(), (data+sizeof(uint64_t)), ciffSize);

    CAFF::AnimationBlock animBlock(duration, ciffData);
    return animBlock;
}

void CAFF::printAnimationBlockInfo(const AnimationBlock block, bool truncateOutput) {
    std::cout << "###AnimationBlock obj###" << '\n';
    std::cout << "\tduration: " << block.duration << '\n';
    std::cout << "\tciffData:\n";
    for (size_t i = 0; i < block.ciffData.size(); i++) {
        if (i >= block.ciffData.size() || (i > 64 && truncateOutput)) {
            break;
        }
        if (i % 4 == 0) {
            std::cout << "\t";
        }
        std::cout << static_cast<unsigned short>(block.ciffData[i]) << " ";
        if (i % 4 == 3 && i != block.ciffData.size() - 1) {
            std::cout << "\n";
        }
    }
    std::cout << "\n#############\n";
}

std::optional<CAFF::Block> CAFF::readBlock(std::istream &file){
    uint8_t id;
    file.read(reinterpret_cast<char*>(&id), sizeof(uint8_t));
    char sizeBytes[8];
    file.read(sizeBytes, 8);
    uint64_t size = util::convertLittleEndianToUint(sizeBytes);

    std::optional<std::vector<char>> res = util::readBytes(file, size);
    if(!res.has_value()){
        return std::nullopt;
    }
    return Block(BlockID(id), size, res.value());
}

int CAFF::convertFile(const std::string inPath, const std::string outPath) {
    std::ifstream inputFile(inPath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Failed to open the image file." << std::endl;
        return -1;
    }
    auto res1 = readBlock(inputFile);
    if(!res1.has_value()){
        std::cerr << "Failed to read 1st block of " << inPath << std::endl;
        return -1;
    }
    Block block1 = res1.value();
    printBlockInfo(block1, false);
    parseHeaderBlock(block1);

    auto res2 = readBlock(inputFile);
    if(!res2.has_value()){
        std::cerr << "Failed to read 2nd block of " << inPath << std::endl;
        return -1;
    }
    Block block2 = res2.value();
    printBlockInfo(block2, false);
    parseCreditsBlock(block2);

    auto res3 = readBlock(inputFile);
    if(!res3.has_value()){
        std::cerr << "Failed to read 3rd block of " << inPath << std::endl;
        return -1;
    }
    Block block3 = res3.value();
    printBlockInfo(block3, true);
    AnimationBlock animBlock = parseAnimationBlock(block3).value();
    printAnimationBlockInfo(animBlock, true);
    CIFF::CIFF ciff = CIFF::parseCIFF(animBlock.ciffData.data()).value();
    CIFF::toWebp(ciff, outPath);
    return 0;
}

