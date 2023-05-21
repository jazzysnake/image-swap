#ifndef CAFF_UTIL
#define CAFF_UTIL

#include <vector>
#include <string>
#include <optional>
#include <stdint.h>

namespace util {
    std::optional<std::vector<char>> readBytes(std::istream &file, uint64_t numBytes);
    bool compareArrays(const char* array1, const char* array2, std::size_t length);

    struct Date {
        const uint16_t year;
        const uint8_t month;
        const uint8_t day;
        const uint8_t hour;
        const uint8_t minute;

        Date(uint16_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t min)
            : year(y), month(m), day(d), hour(h), minute(min) {}
    };

    Date parseDate(char byteArray[6]);

    void printDate(const util::Date& date);

    template<class T = uint64_t>
        inline T convertLittleEndianToUint(const char* data){
            T result = 0;
            for (int i = 0; i < sizeof(T); i++)
            {
                result |= static_cast<T>(static_cast<uint8_t>(data[i])) << (8 * i);
            }
            return result;
        }

    std::optional<int> indexOfNewline(const char* input, int length);
    std::optional<std::vector<std::string>> parseTags(const char* input, int length);

    std::optional<std::string> parseCaption(const char* input);

    std::string rstrip(const std::string& input, const std::string& chars);

    bool validatePath(const std::string& path);

    std::optional<std::vector<char>> readFile(const std::string& filePath);
    bool validateDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
}

#endif
