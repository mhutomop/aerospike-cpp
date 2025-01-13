#include <aerospike/as_exp.h>

#ifdef __cplusplus
extern "C" {
#endif
    
as_exp* get_exp_filter_author_and_title
(
    const char* author,
    const char* title
);

#ifdef __cplusplus
}
#endif