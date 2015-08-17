#ifndef DISK_HASH_H_
#define DISK_HASH_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

/// Secondary memory hashmap
/// When you create a hashmap with n buckets, you are creating
/// a set of n (fixed) indexes that point

/// TODO at one point this should be dynamic
/// TODO error checking
#define BUCKET_COUNT 20

#define NO_BUCKET ((size_t)-1)

/// the comparer function, similar to the ordering functions
/// in C's stdlib
typedef int (*disk_hasher_comparer_fn_t)(const void*, const void*, size_t);
/// the hasher function: the object and it's size
typedef size_t (*disk_hasher_hasher_fn_t)(const void*, size_t);

/// The set of functions that we'll need
typedef struct disk_hasher_fn_set {
    disk_hasher_comparer_fn_t key_comparer;
    disk_hasher_comparer_fn_t element_comparer;
    disk_hasher_hasher_fn_t key_hasher;
    disk_hasher_hasher_fn_t element_hasher;
} disk_hasher_fn_set_t;

/// The index type, consistent of the head of its bucket list
typedef struct disk_hasher_index { size_t bucket_index; } disk_hasher_index_t;

/// The indexer, consistent of an array of N indexes and a head of a linked list
/// pointing to the first free bucket
typedef struct disk_hasher_indexer {
    disk_hasher_index_t indices[BUCKET_COUNT];
    size_t first_free_bucket;
} disk_hasher_indexer_t;

/// A bucket is a chunk of size:
///   sizeof(size_t) + tuple_size * elements_per_bucket + sizeof(size_t)
/// Where tuple_size is element_size + key_size
/// The first size_t is the element_count.
/// The last one is an index to the next linked bucket
typedef struct disk_hasher {
    size_t bucket_count;
    size_t key_size;
    size_t element_size;
    size_t tuple_size;
    size_t bucket_size;
    size_t index_size;
    disk_hasher_fn_set_t fn;
    disk_hasher_indexer_t* indexer;
    FILE* file;
} disk_hasher_t;

#ifdef _WIN32
size_t file_size(FILE* f) {
    size_t ret, previous = ftell(f);

    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, previous);

    return ret;
}
#else
size_t file_size(FILE* f) {
    struct stat data;
    int fd = fileno(f);

    if (fd == -1)
        return 0;

    if (fstat(fd, &data) == -1)
        return 0;

    return data.st_size;
}
#endif

/// General hashing function, we hash the bytes of the element
/// sdbm hashing: http://www.cse.yorku.ca/~oz/hash.html
size_t disk_hasher_default_hasher(const void* element, size_t size) {
    size_t i;
    size_t hash = 0;
    const char* bytes = (const char*)element;

    assert(element != NULL);

    for (i = 0; i < size; ++i)
        hash = hash * 65599 + bytes[i];

    return hash;
}

/// Specialized hashing function for strings:
/// We hash just the valid data (0..strlen(str))
inline size_t disk_hasher_str_hasher(const void* element, size_t size) {
    return disk_hasher_default_hasher(element, strlen((const char*)element));
}

/// Default comparer: Compare byte by byte
int disk_hasher_default_comparer(const void* _1, const void* _2, size_t size) {
    return memcmp(_1, _2, size);
}

inline int disk_hasher_str_comparer(const void* _1, const void* _2, size_t _) {
    return strcmp((const char*)_1, (const char*)_2);
}

/// TODO right now bucket_count is overwritten
disk_hasher_t* disk_hasher_new(FILE* f, size_t bucket_count, size_t key_size,
                               size_t element_size,
                               disk_hasher_comparer_fn_t key_cmp,
                               disk_hasher_hasher_fn_t key_hasher,
                               disk_hasher_comparer_fn_t cmp,
                               disk_hasher_hasher_fn_t hasher);

/// This function checks the sanity of the file
/// Truncates it if necessary
void disk_hasher_init(disk_hasher_t* self);

/// Get the correct bucket index for a key
size_t disk_hasher_bucket_index(disk_hasher_t* self, const void* key) {
    return self->fn.key_hasher(key, self->key_size) % self->bucket_count;
}

/// Skip some bits
void disk_hasher_skip(disk_hasher_t* self, long amount) {
    fseek(self->file, amount, SEEK_CUR);
}

/// Reads a size_t element from the current position of the file
size_t disk_hasher_read_size(disk_hasher_t* self) {
    size_t ret;
    fread(&ret, sizeof(size_t), 1, self->file);
    return ret;
}

/// Position the file pointer in the beginning of the bucket
void disk_hasher_to_bucket(disk_hasher_t* self, size_t bucket_index);

/// Gets the element count for a bucket
size_t disk_hasher_bucket_element_count(disk_hasher_t* self,
                                        size_t bucket_index);

/// Gets the continuation bucket for a given bucket
size_t disk_hasher_next_bucket(disk_hasher_t* self, size_t bucket_index);

/// General utilities
void disk_hasher_read_next_key(disk_hasher_t* self, void* key_buff);

void disk_hasher_read_next_value(disk_hasher_t* self, void* val_buff);

/// Reads the next key and value and stores it inside key_buff and val_buff
void disk_hasher_read_next_tuple(disk_hasher_t* self, void* key_buff,
                                 void* val_buff);

/// Get an element from the set, returning 0, or none (-1) given a key
int disk_hasher_retrieve(disk_hasher_t* self, const void* key, void* buffer);

void disk_hasher_insert(disk_hasher_t* self, const void* key,
                        const void* value);

/// Truncate the file if necessary, filling it with zeros
void disk_hasher_truncate(disk_hasher_t* self);

disk_hasher_indexer_t* disk_hasher_read_indexer(disk_hasher_t*);
/// Save de index n
void disk_hasher_save_index(disk_hasher_t* self, size_t index);

/// Create a bucket returning its index, and the file pointer at the beginning
size_t disk_hasher_create_bucket(disk_hasher_t* self);

#endif
