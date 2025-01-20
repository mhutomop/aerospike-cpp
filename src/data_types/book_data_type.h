#pragma once
#include <cstdint>
#include <vector>
#include <cryptopp/ripemd.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

#pragma pack(push, 0)
struct Details {
    std::string serial_no;
    uint16_t total_chapter;
    uint16_t total_page;
};
#pragma pack(pop)

#pragma pack(push, 0)
struct Materials {
    const char* cover;
    const char* content;
};
#pragma pack(pop)

#pragma pack(push, 0)
struct Review {
    std::string reviewer;
    uint8_t rate;
    std::string comment;
};
#pragma pack(pop)

#pragma pack(push, 0)
struct Book {
    CryptoPP::byte primary_key[CryptoPP::RIPEMD160::DIGESTSIZE];
    std::string key;
    std::string author;
    std::string title;
    uint16_t published_year;
    Details details;
    Materials materials;
    std::vector<Review> reviews;
};
#pragma pack(pop)
