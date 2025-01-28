#include "operations.h"

// Callback function to process query results
bool query_callback(const as_val *val, void *udata) {
    if (!val) return false; // No more results

    // Cast the value to a record
    const as_record *rec = as_record_fromval(val);
    if (rec) {
        printf("Record read successfully. Title value: %s\n", as_record_get_str(rec, "title"));
    }
    return true; // Continue processing
}

void create_indexes
(
    aerospike* as,
    const char* ns,
    const char* set
)
{
    as_error err;

    // Create index
    if (aerospike_index_string_create(as, &err, NULL, ns, set, "author", "idx_author") != AEROSPIKE_OK) {
        printf("aerospike_index_integer_create() returned %d - %s\n", err.code, err.message);
    }

    if (aerospike_index_string_create(as, &err, NULL, ns, set, "title", "idx_title") != AEROSPIKE_OK) {
        printf("aerospike_index_integer_create() returned %d - %s\n", err.code, err.message);
    }

    if (aerospike_index_create_complex(as, &err, NULL, NULL, ns, set, "details", "idx_details_map_val", AS_INDEX_TYPE_MAPVALUES, AS_INDEX_STRING) != AEROSPIKE_OK) {
        printf("aerospike_index_integer_create() returned %d - %s\n", err.code, err.message);
    }

    if (aerospike_index_create_complex(as, &err, NULL, NULL, ns, set, "details", "idx_details_map_key", AS_INDEX_TYPE_MAPKEYS, AS_INDEX_STRING) != AEROSPIKE_OK) {
        printf("aerospike_index_integer_create() returned %d - %s\n", err.code, err.message);
    }

    if (aerospike_index_create_complex(as, &err, NULL, NULL, ns, set, "materials", "idx_materials_map_val", AS_INDEX_TYPE_MAPVALUES, AS_INDEX_STRING) != AEROSPIKE_OK) {
        printf("aerospike_index_integer_create() returned %d - %s\n", err.code, err.message);
    }
}

void create_records
(
    aerospike* as,
    const char* ns,
    const char* set,
    uint16_t number_of_records,
    as_key* record_keys,
    as_record* records
)
{
    as_error err;

    // Create records
    printf("CREATE 10,0000 RECORDS\n");
    for (uint16_t i = 0; i < number_of_records; i++) {
        char* serial = malloc(6 * sizeof(char));
        sprintf(serial, "%u", i);
        char* key = malloc(10 * sizeof(char));
        sprintf(key, "book%u", i);
        char* author = malloc(12 * sizeof(char));
        sprintf(author, "author%u", i);
        char* title = malloc(11 * sizeof(char));
        sprintf(title, "title%u", i);

        as_key_init(&record_keys[i], ns, set, key);

        as_record_inita(&records[i], 6);

        int64_t published_year = 1997;
        if (i == 100)
            published_year = 2002;
        
        as_record_set_str(&records[i], "author", author);
        as_record_set_str(&records[i], "title", title);
        as_record_set_int64(&records[i], "publishedYear", published_year);

        as_string field_details_1;
        char field_details_1_value[9] = "serialNo";
        as_string_init(&field_details_1, field_details_1_value, false);
        as_string field_details_2;
        char field_details_2_value[13] = "totalChapter";
        as_string_init(&field_details_2, field_details_2_value, false);
        as_string field_details_3;
        char field_details_3_value[10] = "totalPage";
        as_string_init(&field_details_3, field_details_3_value, false);

        as_hashmap details;
        as_hashmap_init(&details, 3);
        char* serial_no_value = malloc(9 * sizeof(char));
        sprintf(serial_no_value, "no_%u", i);
        as_string serial_no;
        as_string_init(&serial_no, serial_no_value, false);
        as_hashmap_set(&details, (as_val *) &field_details_1, (as_val *) &serial_no);
        as_hashmap_set(&details, (as_val *) &field_details_2, (as_val *) as_integer_new(5));
        as_hashmap_set(&details, (as_val *) &field_details_3, (as_val *) as_integer_new(50));

        as_record_set_map(&records[i], "details", (as_map *) &details);

        as_string field_materials_1;
        char field_materials_1_value[6] = "cover";
        as_string_init(&field_materials_1, field_materials_1_value, false);
        as_string field_materials_2;
        char field_materials_2_value[8] = "content";
        as_string_init(&field_materials_2, field_materials_2_value, false);
        as_string paper;
        char paper_value[6] = "Paper";
        as_string_init(&paper, paper_value, false);

        as_hashmap materials;
        as_hashmap_init(&materials, 2);
        as_hashmap_set(&materials, (as_val *) &field_materials_1, (as_val *) &paper);
        as_hashmap_set(&materials, (as_val *) &field_materials_2, (as_val *) &paper);

        as_record_set_map(&records[i], "materials", (as_map *) &materials);

        as_arraylist review_list;
        as_arraylist_inita(&review_list, 2);

        as_string field_reviews_1;
        char field_reviews_1_value[9] = "reviewer";
        as_string_init(&field_reviews_1, field_reviews_1_value, false);
        as_string field_reviews_2;
        char field_reviews_2_value[5] = "rate";
        as_string_init(&field_reviews_2, field_reviews_2_value, false);
        as_string field_reviews_3;
        char field_reviews_3_value[8] = "comment";
        as_string_init(&field_reviews_3, field_reviews_3_value, false);
        as_string good;
        char good_value[5] = "Good";
        as_string_init(&good, good_value, false);
        as_string excellent;
        char excellent_value[10] = "Excellent";
        as_string_init(&excellent, excellent_value, false);

        // Review 1
        as_hashmap review_1;
        as_hashmap_init(&review_1, 3);
        char* reviewer_1_value = malloc(16 * sizeof(char));
        sprintf(reviewer_1_value, "reviewer%u_1", i);
        as_string reviewer_1;
        as_string_init(&reviewer_1, reviewer_1_value, false);
        as_hashmap_set(&review_1, (as_val *) &field_reviews_1, (as_val *) &reviewer_1);
        as_hashmap_set(&review_1, (as_val *) &field_reviews_2, (as_val *) as_integer_new(5));
        as_hashmap_set(&review_1, (as_val *) &field_reviews_3, (as_val *) &excellent);
        as_arraylist_append(&review_list, (as_val *) &review_1);

        // Review 2
        as_hashmap review_2;
        as_hashmap_init(&review_2, 3);
        char* reviewer_2_value = malloc(16 * sizeof(char));
        sprintf(reviewer_2_value, "reviewer%u_2", i);
        as_string reviewer_2;
        as_string_init(&reviewer_2, reviewer_2_value, false);
        as_hashmap_set(&review_2, (as_val *) &field_reviews_1, (as_val *) &reviewer_2);
        as_hashmap_set(&review_2, (as_val *) &field_reviews_2, (as_val *) as_integer_new(4));
        as_hashmap_set(&review_2, (as_val *) &field_reviews_3, (as_val *) &good);
        as_arraylist_append(&review_list, (as_val *) &review_2);

        as_record_set_list(&records[i], "reviews", (as_list *) &review_list);

        // Set write policy with send_key = true
        as_policy_write policy;
        as_policy_write_init(&policy);
        policy.key = AS_POLICY_KEY_SEND;  // Ensure the primary key is sent with the write request

        if (aerospike_key_put(as, &err, &policy, &record_keys[i], &records[i]) != AEROSPIKE_OK) {
            fprintf(stderr, "Error writing record: %s\n", err.message);
        }

        free(serial);
        free(key);
        free(author);
        free(title);
        free(serial_no_value);
        free(reviewer_1_value);
        free(reviewer_2_value);
    }
}

void search_by_primary_key
(
    aerospike* as,
    const char* ns,
    const char* set,
    const char* key
)
{
    as_error err;

    printf("SEARCH BY PRIMARY KEY\n");
    as_key record_key;
    as_key_init(&record_key, ns, set, key);
    as_record* read_record = NULL;
    if (aerospike_key_get(as, &err, NULL, &record_key, &read_record) == AEROSPIKE_OK) {
        printf("Record read successfully. Title value: %s\n", as_record_get_str(read_record, "title"));
    } else {
        fprintf(stderr, "Error reading record: %s\n", err.message);
    }
}

void search_by_multiple_bin_value
(
    aerospike* as,
    const char* ns,
    const char* set,
    const char* author,
    const char* title
)
{
    as_error err;

    printf("SEARCH BY BIN VALUE\n");
    
    // Prepare the query
    as_query query;
    as_exp_build(predexp, as_exp_and(
        as_exp_cmp_eq(as_exp_bin_str("author"), as_exp_str(author)),
        as_exp_cmp_eq(as_exp_bin_str("title"), as_exp_str(title))
    ));
    as_policy_query policy;
    policy.base.filter_exp = predexp;
    as_query_init(&query, ns, set); // Namespace: "test", Set: "books"

    // Execute the query
    if (aerospike_query_foreach(as, &err, &policy, &query, query_callback, NULL) != AEROSPIKE_OK) {
        fprintf(stderr, "Error reading record: %s\n", err.message);
    }
    as_query_destroy(&query);
}

void search_by_bin_value_with_range
(
    aerospike* as,
    const char* ns,
    const char* set,
    const uint16_t published_year_start,
    const uint16_t published_year_end
)
{
    as_error err;

    printf("SEARCH BY BIN VALUE WITH RANGE\n");
    
    // Prepare the query
    as_query query;
    as_exp_build(predexp, as_exp_and(
        as_exp_cmp_gt(as_exp_bin_int("publishedYear"), as_exp_int(published_year_start)),
        as_exp_cmp_lt(as_exp_bin_int("publishedYear"), as_exp_int(published_year_end))
    ));
    as_policy_query policy;
    policy.base.filter_exp = predexp;
    as_query_init(&query, ns, set); // Namespace: "test", Set: "books"

    // Execute the query
    if (aerospike_query_foreach(as, &err, &policy, &query, query_callback, NULL) != AEROSPIKE_OK) {
        fprintf(stderr, "Error reading record: %s\n", err.message);
    }
    as_query_destroy(&query);
}

void search_by_map_value
(
    aerospike* as,
    const char* ns,
    const char* set,
    const char* serial_no
)
{
    as_error err;

    printf("SEARCH BY MAP VALUE\n");
    
    // Prepare the query
    as_query query;
    as_exp_build(predexp, as_exp_cmp_eq(
        as_exp_map_get_by_key(NULL, AS_MAP_RETURN_VALUE, AS_EXP_TYPE_STR, as_exp_str("serialNo"), as_exp_bin_map("details")),
        as_exp_str(serial_no)
    ));
    as_policy_query policy;
    policy.base.filter_exp = predexp;
    as_query_init(&query, ns, set); // Namespace: "test", Set: "books"

    // Execute the query
    if (aerospike_query_foreach(as, &err, &policy, &query, query_callback, NULL) != AEROSPIKE_OK) {
        fprintf(stderr, "Error reading record: %s\n", err.message);
    }
    as_query_destroy(&query);
}

void delete_records
(
    aerospike* as,
    uint16_t number_of_records,
    as_key* record_keys,
    const as_record* records
)
{
    as_error err;

    printf("DELETE RECORDS\n");
    for (uint16_t i = 0; i < number_of_records; i++) {
        if (aerospike_key_remove(as, &err, NULL, &record_keys[i]) != AEROSPIKE_OK) {
            fprintf(stderr, "Error deleting record: %s\n", err.message);
        }

        as_key_destroy(&record_keys[i]);
    }
}
