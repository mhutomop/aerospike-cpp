#pragma once
#include <vector>
#include "../data_types/book_data_type.h"

void create_records
(
    std::vector<Book> &book_collection,
    const uint16_t &number_of_records
);
void update_by_primary_key
(
    std::vector<Book> &book_collection,
    const char* key,
    const char* title
);
void search_by_primary_key
(
    std::vector<Book> &book_collection,
    const char* key
);
void search_by_multiple_field_value
(
    std::vector<Book> &book_collection,
    const char* author,
    const char* title
);
void search_by_field_value_with_range
(
    std::vector<Book> &book_collection,
    const uint16_t published_year_start,
    const uint16_t published_year_end
);
void delete_records(std::vector<Book> &book_collection);
