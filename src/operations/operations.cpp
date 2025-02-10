#include <iostream>
#include "operations.h"

void set_digest
(
    CryptoPP::byte* digest,
    const char* key
)
{
    // Combine namespace, set name, and primary key
    std::string key_data = key;

    // Create a RIPEMD-160 hash object
    CryptoPP::RIPEMD160 hash;

    // Generate the digest
    hash.CalculateDigest(digest, reinterpret_cast<const CryptoPP::byte*>(key_data.data()), key_data.size());
}

void create_records
(
    std::vector<Book> &book_collection,
    const uint16_t &number_of_records
)
{
    // Create records
    std::cout << "CREATE 10,0000 RECORDS" << std::endl;
    for (uint16_t i = 0; i < number_of_records; i++) {
        Book book;
        std::string key = "book" + std::to_string(i);
        std::string author = "author" + std::to_string(i);
        std::string title = "title" + std::to_string(i);

        uint16_t published_year = 1997;
        if (i == 100)
            published_year = 2002;
        
        book.key = key;
        set_digest(book.primary_key, key.c_str());
        book.author = author;
        book.title = title;
        book.published_year = published_year;

        std::string serial_no_value = "no_" + std::to_string(i);
        book.details = {
            .serial_no = serial_no_value,
            .total_chapter = 5,
            .total_page = 50
        };

        book.materials = {
            .cover = "Paper",
            .content = "Paper"
        };

        std::string reviewer_1_value = "reviewer" + std::to_string(i) + "_1";
        Review review_1 = {reviewer_1_value, 4, "Good"};
        std::string reviewer_2_value = "reviewer" + std::to_string(i) + "_2";
        Review review_2 = {reviewer_2_value, 5, "Excellent"};
        book.reviews.push_back(review_1);
        book.reviews.push_back(review_2);

        book_collection.push_back(book);
    }
}

void update_by_primary_key
(
    std::vector<Book> &book_collection,
    const char* key,
    const char* title
)
{
    std::cout << "UPDATE BY PRIMARY KEY" << std::endl;

    CryptoPP::byte primary_key[CryptoPP::RIPEMD160::DIGESTSIZE];
    set_digest(primary_key, key);

    auto it = std::find_if(book_collection.begin(), book_collection.end(), [&primary_key](const Book& book) {        
        return (std::memcmp(book.primary_key, primary_key, CryptoPP::RIPEMD160::DIGESTSIZE) == 0);
    });
    if (it != book_collection.end()) {
        it->title = title;
        std::cout << "Record update successfully. Title value:" + std::string(it->title) << std::endl;
    } else {
        std::cerr << "Record not found" << std::endl;
    }
}

void search_by_primary_key
(
    std::vector<Book> &book_collection,
    const char* key
)
{
    std::cout << "SEARCH BY PRIMARY KEY" << std::endl;

    CryptoPP::byte primary_key[CryptoPP::RIPEMD160::DIGESTSIZE];
    set_digest(primary_key, key);

    auto it = std::find_if(book_collection.begin(), book_collection.end(), [&primary_key](const Book& book) {        
        return (std::memcmp(book.primary_key, primary_key, CryptoPP::RIPEMD160::DIGESTSIZE) == 0);
    });
    if (it != book_collection.end()) {
        std::cout << "Record read successfully. Title value:" + std::string(it->title) << std::endl;
    } else {
        std::cerr << "Record not found" << std::endl;
    }
}

void search_by_multiple_field_value
(
    std::vector<Book> &book_collection,
    const char* author,
    const char* title
)
{
    std::cout << "SEARCH BY MULTIPLE FIELD VALUE" << std::endl;
    
    auto it = std::find_if(book_collection.begin(), book_collection.end(), [&author, &title](const Book& book) {        
        return (book.author == author && book.title == title);
    });
    if (it != book_collection.end()) {
        std::cout << "Record read successfully. Title value:" + std::string(it->title) << std::endl;
    } else {
        std::cerr << "Record not found" << std::endl;
    }
}

void search_by_field_value_with_range
(
    std::vector<Book> &book_collection,
    const uint16_t published_year_start,
    const uint16_t published_year_end
)
{
    std::cout << "SEARCH BY FIELD VALUE WITH RANGE" << std::endl;
    
    auto it = std::find_if(book_collection.begin(), book_collection.end(), [&published_year_start, &published_year_end](const Book& book) {        
        return (book.published_year > published_year_start && book.published_year << published_year_end);
    });
    if (it != book_collection.end()) {
        std::cout << "Record read successfully. Title value:" + std::string(it->title) << std::endl;
    } else {
        std::cerr << "Record not found" << std::endl;
    }
}

void delete_records(std::vector<Book> &book_collection) {
    std::cout << "DELETE RECORDS" << std::endl;

    book_collection.clear();
}
