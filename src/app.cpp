#include <chrono>
#include <iostream>
#include <vector>
#include "operations/operations.h"

int64_t get_time_in_us() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

    return duration.count();
}

int main() {
    // Create vector
    const uint16_t number_of_records = 10000;
    std::vector<Book> book_collection;

    std::cout << "=================================================================================" << std::endl;
    auto start = get_time_in_us();
    create_records(book_collection, number_of_records);
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    // Update a record by primary key
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    update_by_primary_key(book_collection, "book100", "title10o");
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << " us" << std::endl;

    // Read the records by primary key
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    search_by_primary_key(book_collection, "book100");
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    // Read the records by multiple bin value
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    search_by_multiple_field_value(book_collection, "author100", "title100");
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    // Read the records by bin value with range
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    search_by_field_value_with_range(book_collection, 2000, 2005);
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    // Delete the records
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    delete_records(book_collection);
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    return 0;
}
