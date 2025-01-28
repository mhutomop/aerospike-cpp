#include <aerospike/aerospike.h>
#include <aerospike/aerospike_key.h>
#include <aerospike/as_arraylist.h>
#include <aerospike/as_exp.h>
#include <aerospike/as_hashmap.h>
#include <aerospike/as_status.h>
#include <aerospike/as_record.h>
#include <aerospike/as_query.h>
#include <aerospike/aerospike_info.h>
#include <aerospike/aerospike_query.h>

#ifdef __cplusplus
extern "C" {
#endif
    
void create_indexes
(
    aerospike* as,
    const char* ns,
    const char* set
);
void create_records
(
    aerospike* as,
    const char* ns,
    const char* set,
    uint16_t number_of_records,
    as_key* record_keys,
    as_record* records
);
void search_by_primary_key
(
    aerospike* as,
    const char* ns,
    const char* set,
    const char* key
);
void search_by_multiple_bin_value
(
    aerospike* as,
    const char* ns,
    const char* set,
    const char* author,
    const char* title
);
void search_by_bin_value_with_range
(
    aerospike* as,
    const char* ns,
    const char* set,
    const uint16_t published_year_start,
    const uint16_t published_year_end
);
void search_by_map_value
(
    aerospike* as,
    const char* ns,
    const char* set,
    const char* serial_no
);
void delete_records
(
    aerospike* as,
    uint16_t number_of_records,
    as_key* record_keys,
    const as_record* records
);

#ifdef __cplusplus
}
#endif
