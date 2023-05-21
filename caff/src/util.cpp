#include "util.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <fstream>
#include <stdint.h>
#include <optional>
#include <filesystem>

std::optional<std::vector<char>> util::readBytes(std::istream &file, uint64_t numBytes) {
    auto data = std::vector<char>(numBytes);
    file.read(data.data(), numBytes);
    if (file.fail()) {
        return std::nullopt;
    }
    return data;
}
bool util::compareArrays(const char* array1, const char* array2, std::size_t length) {
    return std::memcmp(array1, array2, length) == 0;
}

util::Date util::parseDate(char byteArray[6]){
    uint16_t year = convertLittleEndianToUint<uint16_t>(byteArray);
    uint8_t month = static_cast<uint8_t>(byteArray[2]);
    uint8_t day = static_cast<uint8_t>(byteArray[3]);
    uint8_t hour = static_cast<uint8_t>(byteArray[4]);
    uint8_t minute = static_cast<uint8_t>(byteArray[5]);

    return Date(year, month, day, hour, minute);
}
bool util::validateDate(
        uint16_t year,
        uint8_t month,
        uint8_t day,
        uint8_t hour,
        uint8_t minute
        ) {
    return !(year < 1970 || year > 2050 ||
        month<1 || month>12 ||
        day>31 || day<1 || 
        hour>24 || hour<1 || 
        minute > 60 || minute<1);
}

void util::printDate(const util::Date& date) {
    std::cout << "Date: " << date.year << "-" << static_cast<int>(date.month) << "-"
              << static_cast<int>(date.day) << " ";
    std::cout << "Time: " << static_cast<int>(date.hour);
    std::cout << ":" << static_cast<int>(date.minute) << std::endl;
}

std::optional<int> util::indexOfNewline(const char* input, int length){
    for (size_t i = 0; i < length; i++) {
        if (input[i] == '\n') {
            return i;
        }
    }
    return std::nullopt;
}

std::optional<std::vector<std::string>> util::parseTags(const char* input, int length) {
    std::vector<std::string> tags;

    for (int i = 0; i < length; ++i) {
        const char* start = input + i;
        while (i < length && input[i] != '\0' && input[i] != '\n') {
            ++i;
        }
        if (i < length && input[i] == '\n') {
            std::cerr << "Invalid input: Tag contains newline character." << std::endl;
            return std::nullopt;
        }
        tags.emplace_back(start, input + i - start);
    }

    if (tags.empty()) {
        std::cerr << "Invalid input: No tags found." << std::endl;
        return std::nullopt;
    }

    return tags;
}

std::optional<std::string> util::parseCaption(const char* input) {
    const char* newlinePtr = std::strchr(input, '\n');
    if (newlinePtr == nullptr) {
        std::cerr << "Input rejected: No newline character found." << std::endl;
        return std::nullopt;
    }

    return std::string(input, newlinePtr - input);
}

std::string util::rstrip(const std::string& input, const std::string& chars) {
    size_t pos = input.find_last_not_of(chars);
    if (pos != std::string::npos) {
        return input.substr(0, pos + 1);
    }
    return input;
}

bool util::validatePath(const std::string& path) {
    std::filesystem::path filePath(path);
    return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
}

std::optional<std::vector<char>> util::readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file " << filePath << std::endl;
        return std::nullopt;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);

    if (!file) {
        std::cerr << "Failed to read file " << filePath << std::endl;
        return std::nullopt;
    }

    return buffer;
}
