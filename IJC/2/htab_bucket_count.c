/**
 * @description  Findo out number of buckets in hash table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include "htab.h"


size_t htab_bucket_count(htab_t *t) {
    return t->arr_size;
}
