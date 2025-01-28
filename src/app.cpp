#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include "operations/operations.h"

int64_t get_time_in_us() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

    return duration.count();
}

int main() {
    // Aerospike configuration and client setup
    aerospike as;
    as_config config;
    as_config_init(&config);

    // Set host and port
    as_config_add_host(&config, "127.0.0.1", 43120); // Default Aerospike port

    aerospike_init(&as, &config);

    as_error err; 
    if (aerospike_connect(&as, &err) != AEROSPIKE_OK) {
        std::cerr << "Failed to connect to Aerospike" << std::endl;
        aerospike_destroy(&as);
        return 1;
    } else
        std::cout << "Success connect to Aerospike" << std::endl;

    // Define namespace & set
    const char *ns = "test";
    const char *set = "books";
    const uint16_t number_of_records = 10000;
    std::array<as_key, number_of_records> record_keys;
    std::array<as_record, number_of_records> records;

    create_indexes(&as, ns, set);

    std::cout << "=================================================================================" << std::endl;
    auto start = get_time_in_us();
    create_records(&as, ns, set, number_of_records, record_keys.data(), records.data());
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << " us" << std::endl;

    // Read the records by primary key
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    search_by_primary_key(&as, ns, set, "book100");
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << " us" << std::endl;

    // Read the records by multiple bin value
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    search_by_multiple_bin_value(&as, ns, set, "author100", "title100");
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << " us" << std::endl;

    // Read the records by bin value with range
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    search_by_bin_value_with_range(&as, ns, set, 2000, 2005);
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << " us" << std::endl;

    // Read the records by map value
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    search_by_map_value(&as, ns, set, "no_100");
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << " us" << std::endl;

    // Delete the records
    std::cout << "=================================================================================" << std::endl;
    start = get_time_in_us();
    delete_records(&as, number_of_records, record_keys.data(), records.data());
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << " us" << std::endl;

    // Cleanup
    aerospike_close(&as, &err);
    aerospike_destroy(&as);

    return 0;
}
