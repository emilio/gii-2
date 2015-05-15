#ifndef DISK_HASH_H_
#define DISK_HASH_H_

/// A bucket is a chunk of size:
///   sizeof(size_t) + element_size * elements_per_bucket + sizeof(size_t)
/// The first size_t is the element_count.
/// The last one is an index to the next linked bucket
typedef struct disk_hasher {
    size_t initial_bucket_count;
    size_t actual_bucket_count;
    size_t element_size;
    size_t elements_per_bucket;
    size_t bucket_size;
    FILE* file;
} disk_hasher_t;

size_t file_size(FILE* f) {
    size_t ret,
           previous = ftell(f);

    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, previous);

    return ret;
}

disk_hasher_t* disk_hasher_new(FILE* f,
                               size_t initial_bucket_count,
                               size_t element_size,
                               size_t elements_per_bucket) {
    disk_hasher_t* ret;

    if ( f == NULL )
        return NULL;

    ret = (disk_hasher_t*) malloc(sizeof(disk_hasher_t));

    ret->initial_bucket_count = initial_bucket_count;
    ret->actual_bucket_count = initial_bucket_count;
    ret->element_size = element_size;
    ret->elements_per_bucket = elements_per_bucket;
    ret->bucket_size = 2 * sizeof(size_t) + element_size * elements_per_bucket;
    ret->file = f;

    disk_hasher_init(ret);

    return ret;
}

/// This function checks the sanity of the file
/// Truncates it if necessary
void disk_hasher_init(disk_hasher_t* self) {
    size_t size = file_size(self->file);

    /// If there's more data than what we expect, assume is fine
    if ( size < self->bucket_size * self->initial_bucket_count )
        disk_hasher_truncate(self);
}

void disk_hasher_truncate(disk_hasher_t* self) {
    size_t i;
    void* buffer = (void*) malloc(self->bucket_size);

    assert(buffer);

    memset(buffer, 0, self->bucket_size);

    fseek(self->file, 0, SEEK_SET);

    for ( i = 0; i < self->initial_bucket_count; ++i )
        fwrite(buffer, self->buffer_size, 1, self->file);

    free(buffer);
}

#endif
