#ifndef CAFF_HPP
#define CAFF_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include "util.h"
#include <cstring>
namespace CAFF {

    int convertFile(const std::string inPath, const std::string outPath);

    enum BlockID : uint8_t {
        HEADER_ID = 0x1,
        CREDITS_ID = 0x2,
        ANIMATION_ID = 0x3
    };

    struct Block {
        const BlockID id;
        const uint64_t size;
        const std::vector<char> data;
        Block(BlockID _id, uint64_t _size, std::vector<char> _data):
            id(_id), size(_size), data(_data){}
    };

    std::optional<Block> readBlock(std::istream &file);

    void printBlockInfo(const Block, bool truncateOutput);

    struct HeaderBlock {
        char magic[4];
        const uint64_t size;
        const uint64_t numPics;
        HeaderBlock(const char* _magic, uint64_t _size, uint64_t _numPics): 
            size(_size), numPics(_numPics) {
                std::memcpy(magic, _magic, sizeof(magic));
            }
    };
    const char HEADER_MAGIC[4] = {'C', 'A', 'F', 'F'};

    std::optional<HeaderBlock> parseHeaderBlock(const Block);

    void printHeaderInfo(const HeaderBlock);

    struct CreditsBlock {
        util::Date creationDate;
        const uint64_t creatorSize;
        const std::string creator;
        CreditsBlock(
                const util::Date& date,
                uint64_t size,
                const std::string& creator
                ): creationDate(date), creatorSize(size), creator(creator) {}
    };

    std::optional<CreditsBlock> parseCreditsBlock(const Block);
    void printCreditsBlockInfo(const CreditsBlock& credits);

    struct AnimationBlock {
        const uint64_t duration;
        const std::vector<char> ciffData;
        AnimationBlock(uint64_t _duration, std::vector<char> _ciffData):
            duration(_duration),  ciffData(_ciffData) {}
    };

    std::optional<AnimationBlock> parseAnimationBlock(const Block);

    void printAnimationBlockInfo(const AnimationBlock block, bool truncateOutput);
}


#endif
