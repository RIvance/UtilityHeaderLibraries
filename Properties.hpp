
#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"

#ifndef  PROPERTIES_HPP
#define  PROPERTIES_HPP

#include "Logger.hpp"

#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <typeinfo>
#include <iostream>
#include <algorithm>

using std::string;

__attribute__(( init_priority(101) ))
static std::map<string, string> _properties;

// trim from start
static inline void _ltrim(string & s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
    std::not1(std::ptr_fun<int, int>(std::isspace)))
    );
}

// trim from end
static inline void _rtrim(string & s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end()
    );
}

static inline string trim(string str)
{
    _ltrim(str);
    _rtrim(str);
    return str;
}

static inline std::vector<string> split(const string & str, const string & delim)
{
    std::vector<string> tokens;
    size_t prev = 0, pos;

    do {
        pos = str.find(delim, prev);
        if (pos == string::npos) {
            pos = str.length();
        }
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        prev = pos + delim.length();

    } while (pos < str.length() && prev < str.length());

    return tokens;
}

static inline void _loadPropFile(const char* path) noexcept
{
    std::ifstream file(path);
    string line;
    while (std::getline(file, line)) {
        if (line[0] != '#' && !trim(line).empty()) {
            auto pair = split(line, "=");
            if (pair.size() != 2) {
                exit(0);
            }
            string key = trim(pair[0]);
            string value = trim(pair[1]);
            _properties.emplace(key, value);
        }
    }
}

static inline string _findProp(const char* key)
{
    std::map<string, string>::iterator entry;
    if ((entry = _properties.find(key)) == _properties.end()) {
        logger::error("Unable to find property:", key);
        exit(-1);
    } else {
        auto value = entry->second;
        logger::info("Load property:", key, "=", value);
        return value;
    }
}

template <typename T>
static inline T _initProp(const char* key) noexcept
{
    logger::error("Unsupported property type:", typeid(T).name());
}

template < >
inline bool _initProp<bool>(const char* key) noexcept
{
    string skey = key;
    return skey == "true" || skey == "True" || skey == "TRUE";
}

template < >
inline string _initProp<string>(const char* key) noexcept
{
    return _findProp(key);
}

template < >
inline int8_t _initProp<int8_t>(const char* key) noexcept
{
    return (int8_t) std::stoi(_findProp(key));
}

template < >
inline int16_t _initProp<int16_t>(const char* key) noexcept
{
    return (int16_t) std::stoi(_findProp(key));
}

template < >
inline int32_t _initProp<int32_t>(const char* key) noexcept
{
    return std::stoi(_findProp(key));
}

template < >
inline int64_t _initProp<int64_t>(const char* key) noexcept
{
    return std::stoll(_findProp(key));
}

template < >
inline uint16_t _initProp<uint16_t>(const char* key) noexcept
{
    return (uint16_t) std::stoul(_findProp(key));
}

template < >
inline uint32_t _initProp<uint32_t>(const char* key) noexcept
{
    return std::stoul(_findProp(key));
}

template < >
inline uint64_t _initProp<uint64_t>(const char* key) noexcept
{
    return std::stoull(_findProp(key));
}

template < >
inline float _initProp<float>(const char* key) noexcept
{
    return std::stof(_findProp(key));
}

template < >
double _initProp<double>(const char* key) noexcept
{
    return std::stod(_findProp(key));
}

template < >
long double _initProp<long double>(const char* key) noexcept
{
    return std::stold(_findProp(key));
}

#define _CONCAT_INNER(a, b) a ## b
#define _CONCAT(a, b) _CONCAT_INNER(a, b)

#define PROPERTY_CONFIG(path)               \
    __attribute__((constructor))            \
    void _CONCAT(_propLoad_, __LINE__)()    \
    {                                       \
        _loadPropFile(path);                \
    }                                       \

#define $(var) var = _initProp<typeof(var)>(#var)

#endif // PROPERTIES_HPP

#pragma clang diagnostic pop
