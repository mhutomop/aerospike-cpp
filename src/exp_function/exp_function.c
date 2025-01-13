#include "exp_function.h"

as_exp* get_exp_filter_author_and_title
(
    const char* author,
    const char* title
)
{
    as_exp_build(predexp, as_exp_and(
        as_exp_cmp_eq(as_exp_bin_str("author"), as_exp_str(author)),
        as_exp_cmp_eq(as_exp_bin_str("title"), as_exp_str(title))
    ));

    return predexp;
}