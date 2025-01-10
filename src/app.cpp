#include <array>
#include <chrono>
#include <iostream>
#include <aerospike/aerospike.h>
#include <aerospike/aerospike_key.h>
#include <aerospike/as_status.h>
#include <aerospike/as_record.h>
#include <aerospike/as_query.h>
#include <aerospike/aerospike_info.h>
#include <aerospike/aerospike_query.h>

int64_t get_time_in_us() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

    return duration.count();
}

// Callback function to process query results
bool query_callback(const as_val *val, void *udata) {
    if (!val) return false; // No more results

    // Cast the value to a record
    if (const as_record *rec = as_record_fromval(val)) {
        std::cout << "Record read successfully. Title value: " << as_record_get_str(rec, "title") << std::endl;
    }
    return true; // Continue processing
}

int main() {
    // Aerospike configuration and client setup
    aerospike as;
    as_config config;
    as_config_init(&config);

    // Set host and port
    as_config_add_host(&config, "127.0.0.1", 3000); // Default Aerospike port

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

    // Construct the CREATE INDEX command
    // const char *bin_name = "author"; // Replace with your bin name
    // const char *index_name = "idx_author";
    // const char *index_type = "STRING"; // STRING, NUMERIC, or GEO2DSPHERE

    // char command[512];
    // snprintf(command, sizeof(command),
    //          "sindex-create:ns=%s;set=%s;indexname=%s;indexdata=%s,%s",
    //          ns, set, index_name, bin_name, index_type);

    // // Send the command to the cluster
    // char *response = NULL;
    // if (aerospike_info_any(&as, &err, NULL, command, &response) != AEROSPIKE_OK) {
    //     fprintf(stderr, "Failed to create index: %s [%d]\n", err.message, err.code);
    //     free(response); // Free the response if allocated
    //     aerospike_close(&as, NULL);
    //     aerospike_destroy(&as);
    //     return -1;
    // }

    // Create records
    std::cout << "=================================================================================" << std::endl;
    std::cout << "CREATE 10,0000 RECORDS" << std::endl;
    auto start = get_time_in_us();
    std::array<as_key, number_of_records> record_keys;
    std::array<as_record, number_of_records> records;
    for (uint16_t i = 0; i < number_of_records; i++) {
        std::string key = "book" + std::to_string(i);
        std::string author = "author" + std::to_string(i);
        std::string title = "title" + std::to_string(i);

        as_key_init(&record_keys[i], ns, set, key.c_str());

        as_record_inita(&records[i], 3);
        
        as_record_set_str(&records[i], "author", author.c_str());
        as_record_set_str(&records[i], "title", title.c_str());
        as_record_set_int64(&records[i], "published_year", 1997);

        // Set write policy with send_key = true
        as_policy_write policy;
        as_policy_write_init(&policy);
        policy.key = AS_POLICY_KEY_SEND;  // Ensure the primary key is sent with the write request


        if (aerospike_key_put(&as, &err, &policy, &record_keys[i], &records[i]) != AEROSPIKE_OK) {
            std::cerr << "Error writing record: " << err.message << std::endl;
        }
    }
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    // Read the records by bin value
    std::cout << "=================================================================================" << std::endl;
    std::cout << "SEARCH BY BIN VALUE" << std::endl;
    start = get_time_in_us();
    // Prepare the query
    as_query query;
    as_query_init(&query, ns, set); // Namespace: "test", Set: "books"
    as_query_where_inita(&query, 1); // Number of conditions
    as_query_where(&query, "author", as_string_equals("author100")); // Condition: "author" = "author100"

    // Execute the query
    if (aerospike_query_foreach(&as, &err, nullptr, &query, query_callback, nullptr) != AEROSPIKE_OK) {
        std::cerr << "Error reading record: " << err.message << std::endl;
    }
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    // Read the records by primary key
    std::cout << "=================================================================================" << std::endl;
    std::cout << "SEARCH BY PRIMARY KEY" << std::endl;
    start = get_time_in_us();
    as_key record_key;
    as_key_init(&record_key, ns, set, "book100");
    if (as_record *read_record = nullptr; aerospike_key_get(&as, &err, nullptr, &record_key, &read_record) == AEROSPIKE_OK) {
        std::cout << "Record read successfully. Title value: "
                  << as_record_get_str(read_record, "title") << std::endl;
    } else {
        std::cerr << "Error reading record: " << err.message << std::endl;
    }
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    // Delete the records
    std::cout << "=================================================================================" << std::endl;
    std::cout << "DELETE RECORDS" << std::endl;
    start = get_time_in_us();
    for (uint16_t i = 0; i < number_of_records; i++) {
        if (aerospike_key_remove(&as, &err, nullptr, &record_keys[i]) != AEROSPIKE_OK) {
            std::cerr << "Error deleting record: " << err.message << std::endl;
        }

        as_key_destroy(&record_keys[i]);
        as_record_destroy(&records[i]);
    }
    std::cout << "Time: " + std::to_string(get_time_in_us() - start) << std::endl;

    // Cleanup
    aerospike_close(&as, &err);
    aerospike_destroy(&as);

    return 0;
}
