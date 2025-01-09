#include <iostream>
#include <aerospike/aerospike.h>
#include <aerospike/aerospike_key.h>
#include <aerospike/as_status.h>
#include <aerospike/as_record.h>

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

    // Define namespace, set, and key
    const char *ns = "test";
    const char *set = "demo";
    const char *key = "example_key";

    as_key record_key;
    as_key_init(&record_key, ns, set, key);

    // Create a record
    as_record record;
    as_record_inita(&record, 1);
    as_record_set_int64(&record, "example_bin", 123);

    if (aerospike_key_put(&as, &err, nullptr, &record_key, &record) != AEROSPIKE_OK) {
        std::cerr << "Error writing record: " << err.message << std::endl;
    } else {
        std::cout << "Record written successfully" << std::endl;
    }

    // Read the record
    if (as_record *read_record = nullptr; aerospike_key_get(&as, &err, nullptr, &record_key, &read_record) == AEROSPIKE_OK) {
        std::cout << "Record read successfully. Bin value: "
                  << as_record_get_int64(read_record, "example_bin", 0) << std::endl;
        as_record_destroy(read_record);
    } else {
        std::cerr << "Error reading record: " << err.message << std::endl;
    }

    // Delete the record
    if (aerospike_key_remove(&as, &err, nullptr, &record_key) == AEROSPIKE_OK) {
        std::cout << "Record deleted successfully" << std::endl;
    } else {
        std::cerr << "Error deleting record: " << err.message << std::endl;
    }

    // Cleanup
    as_key_destroy(&record_key);
    as_record_destroy(&record);
    aerospike_close(&as, &err);
    aerospike_destroy(&as);

    return 0;
}
