#ifndef DISK_HASH_H_
#define DISK_HASH_H_
#include <string.h>

#ifdef _WIN32
#   include <windows.h>
#else
#   include <unistd.h>
#endif

/// Secondary memory hashmap

/// the comparer function, similar to the ordering functions
/// in C's stdlib
int (*disk_hasher_comparer_fn_t) (const void*, const void*, size_t);
/// the hasher function: the object and it's size
size_t (*disk_hasher_hasher_fn_t) (const void*, size_t);

/// The set of functions that we'll need
typedef struct disk_hasher_fn_set {
    disk_hasher_comparer_fn_t key_comparer;
    disk_hasher_comparer_fn_t element_comparer;
    disk_hasher_hasher_fn_t key_hasher;
    disk_hasher_hasher_fn_t element_hasher;
} disk_hasher_fn_set_t;

/// A bucket is a chunk of size:
///   sizeof(size_t) + tuple_size * elements_per_bucket + sizeof(size_t)
/// Where tuple_size is element_size + key_size
/// The first size_t is the element_count.
/// The last one is an index to the next linked bucket
typedef struct disk_hasher {
    size_t initial_bucket_count;
    size_t actual_bucket_count;
    size_t elements_per_bucket;
    size_t key_size;
    size_t element_size;
    size_t tuple_size;
    size_t bucket_size;
    disk_hasher_fn_set_t fn;
    FILE* file;
} disk_hasher_t;

#ifdef _WIN32
size_t file_size(FILE* f) {
    size_t ret,
           previous = ftell(f);

    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, previous);

    return ret;
}
#else
size_t file_size(FILE* f) {
    struct stat data;
    int fd = fileno(f);

    if ( fd == -1 )
        return 0;

    if ( fstat(fd, &data) == -1 )
        return 0;

    return data.st_size;
}
#endif

/// General hashing function, we hash the bytes of the element
/// sdbm hashing: http://www.cse.yorku.ca/~oz/hash.html
size_t disk_hasher_default_hasher(const void* element, size_t size) {
    size_t i;
    size_t hash = 0;
    const char* bytes = (const char*) element;

    assert(element != NULL);

    for ( i = 0; i < size; ++i )
       hash = hash * 65599 + bytes[i];

    return hash;
}

/// Specialized hashing function for strings:
/// We hash just the valid data (0..strlen(str))
inline
size_t disk_hasher_str_hasher(const void* element, size_t size) {
    return disk_hasher_default_hasher(element, strlen((const char*)element));
}

/// Default comparer: Compare byte by byte
int disk_hasher_default_comparer(const void* _1, const void* _2, size_t size) {
    return memcmp(_1, _2, size);
}

inline
int disk_hasher_str_comparer(const void* _1, const void* _2, size_t _) {
    return strcmp((const char*) _1, (const char*) _2);
}

/// TODO: Add keys. This just works to check if an element is in a set,
/// it's pretty useless
disk_hasher_t* disk_hasher_new(FILE* f,
                               size_t initial_bucket_count,
                               size_t elements_per_bucket,
                               size_t key_size,
                               size_t element_size,
                               disk_hasher_comparer_fn_t key_cmp,
                               disk_hasher_hasher_fn_t key_hasher,
                               disk_hasher_comparer_fn_t cmp,
                               disk_hasher_hasher_fn_t hasher) {
    disk_hasher_t* ret;

    if ( f == NULL )
        return NULL;

    ret = (disk_hasher_t*) malloc(sizeof(disk_hasher_t));

    ret->initial_bucket_count = initial_bucket_count;
    ret->actual_bucket_count = initial_bucket_count;
    ret->elements_per_bucket = elements_per_bucket;
    ret->key_size = key_size;
    ret->element_size = element_size;
    ret->tuple_size = key_size + element_size;
    ret->bucket_size = 2 * sizeof(size_t) + ret->tuple_size * elements_per_bucket;
    ret->file = f;

    if ( ! cmp )
        cmp = disk_hasher_default_comparer;

    if ( ! hasher )
        hasher = disk_hasher_default_hasher;

    ret->comparer = comparer;
    ret->hasher = hasher;

    disk_hasher_init(ret);

    return ret;
}

/// This function checks the sanity of the file
/// Truncates it if necessary
void disk_hasher_init(disk_hasher_t* self) {
    size_t size = file_size(self->file);

    /// If there's more data than what we expect, assume is fine,
    /// If not, truncate
    if ( size < self->bucket_size * self->initial_bucket_count ) {
        size = self->bucket_size * self->initial_bucket_size;
        disk_hasher_truncate(self);
    }

    /// Calculate the real bucket size
    self->actual_bucket_count = size / self->bucket_size;
}

/// Get the correct bucket index for a key
size_t disk_hasher_bucket_index(disk_hasher_t* self, const void* key) {
    return self->fn.key_hasher(key) % self->initial_bucket_count;
}

/// Reads a size_t element from the current position of the file
inline
size_t disk_hasher_read_size(disk_hasher_t* self) {
    size_t ret;
    fread(&ret, sizeof(size_t), 1, self->file);
    return ret;
}

/// Position the file pointer in the beginning of the bucket
void disk_hasher_to_bucket(disk_hasher_t* self, size_t bucket_ index) {
    assert(bucket < self->actual_bucket_count);
    fseek(self->file, bucket_index * self->bucket_size, SEEK_SET);
}

/// Gets the element count for a bucket
size_t disk_hasher_bucket_element_count(disk_hasher_t* self, size_t bucket_index) {
    disk_hasher_to_bucket(self, bucket_index);

    return disk_hasher_read_size(self);
}

/// Gets the continuation bucket for a given bucket
size_t disk_hasher_next_bucket(disk_hasher_t* self, size_t bucket_index) {
    disk_hasher_to_bucket(self, bucket_index);
    fseek(self->bucket_size - sizeof(size_t), SEEK_CUR);

    return disk_hasher_read_size(self);
}

/// Reads the next key and value and stores it inside key_buff and val_buff
size_t disk_hasher_read_next_tuple(disk_hasher_t* self, void* key_buff, void* val_buff) {
    fread(&key_buff, self->key_size, self->file);
    fread(&val_buff, self->element_size, self->file);
}

/// Get a set of elements from the set, given a key, and put them into `buffer`
/// returns the number of elements read, or ((size_t)-1) in case of error
///
/// TODO: Consider using ssize_t? Check in non-unix systems
size_t disk_hasher_retrieve(disk_hasher_t* self, const void* key, void* buffer, size_t max_elements) {
    size_t index = disk_hasher_bucket_index(self, key);
    size_t elements = disk_hasher_element_count(self, index);
    void* key_buff;
    size_t i;

    if ( elements == 0 )
        return 0;

    key_buff = malloc(self->key_size);
    // TODO

    free(key_buff);

}

/// Truncate the file if necessary, filling it with zeros
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
