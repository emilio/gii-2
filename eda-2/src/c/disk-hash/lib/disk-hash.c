#include "disk-hash.h"

disk_hasher_t* disk_hasher_new(FILE* f,
                               size_t bucket_count,
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

    ret->bucket_count = BUCKET_COUNT;
    ret->key_size = key_size;
    ret->element_size = element_size;
    ret->tuple_size = key_size + element_size;
    /// A bucket is it's tuple and the next bucket index
    ret->bucket_size = ret->tuple_size + sizeof(size_t);
    ret->file = f;

    /// TODO update when bucket count is dynamic
    ret->index_size = sizeof(disk_hasher_indexer_t);

    if ( ! key_cmp )
        key_cmp = disk_hasher_default_comparer;

    if ( ! key_hasher )
        key_hasher = disk_hasher_default_hasher;

    if ( ! cmp )
        cmp = disk_hasher_default_comparer;

    if ( ! hasher )
        hasher = disk_hasher_default_hasher;

    ret->indexer = NULL;
    ret->fn.key_comparer = key_cmp;
    ret->fn.key_hasher = key_hasher;
    ret->fn.element_comparer = cmp;
    ret->fn.element_hasher = hasher;

    disk_hasher_init(ret);

    return ret;
}

/// This function checks the sanity of the file
/// Truncates it if necessary
void disk_hasher_init(disk_hasher_t* self) {
    size_t size = file_size(self->file);

    /// If there's more data than what we expect, assume is fine,
    /// If not, truncate
    if ( size < self->index_size )
        disk_hasher_truncate(self);

    self->indexer = disk_hasher_read_indexer(self);
}

disk_hasher_indexer_t* disk_hasher_read_indexer(disk_hasher_t* self) {
    disk_hasher_indexer_t* ret = (disk_hasher_indexer_t*) malloc(sizeof(disk_hasher_indexer_t));

    assert(ret);

    fread(ret, sizeof(disk_hasher_indexer_t), 1, self->file);

    return ret;
}

/// Position the file pointer in the beginning of the bucket
void disk_hasher_to_bucket(disk_hasher_t* self, size_t bucket_index) {
    fseek(self->file, self->index_size + self->bucket_size * bucket_index, SEEK_SET);
}

/// Gets the continuation bucket for a given bucket
size_t disk_hasher_next_bucket(disk_hasher_t* self, size_t bucket_index) {
    disk_hasher_to_bucket(self, bucket_index);
    disk_hasher_skip(self, self->bucket_size - sizeof(size_t));

    return disk_hasher_read_size(self);
}

/// General utilities
void disk_hasher_read_next_key(disk_hasher_t* self, void* key_buff) {
    fread(&key_buff, self->key_size, 1, self->file);
}

void disk_hasher_read_next_value(disk_hasher_t* self, void* val_buff) {
    fread(&val_buff, self->element_size, 1, self->file);
}

void disk_hasher_write_key(disk_hasher_t* self, const void* key_buff) {
    fwrite(key_buff, self->key_size, 1, self->file);
}

void disk_hasher_write_value(disk_hasher_t* self, const void* val_buff) {
    fwrite(val_buff, self->element_size, 1, self->file);
}

/// Reads the next key and value and stores it inside key_buff and val_buff
void disk_hasher_read_next_tuple(disk_hasher_t* self, void* key_buff, void* val_buff) {
    disk_hasher_read_next_key(self, key_buff);
    disk_hasher_read_next_value(self, val_buff);
}

/// Get an element from the set, returning 0, or none (-1) given a key
int disk_hasher_retrieve(disk_hasher_t* self, const void* key, void* buffer) {
    size_t index = disk_hasher_bucket_index(self, key);
    void* key_buff;

    index = self->indexer->indices[index].bucket_index;

    if ( index == NO_BUCKET )
        return -1;

    key_buff = malloc(self->key_size);

    assert(key_buff);

    while ( index != NO_BUCKET ) {
        disk_hasher_to_bucket(self, index);
        disk_hasher_read_next_tuple(self, key_buff, buffer);
        if ( self->fn.key_comparer(key_buff, key, self->key_size) == 0 ) {
            free(key_buff);
            return 0;
        }

        // Read the next one
        index = disk_hasher_read_size(self);
    }

    free(key_buff);
    return -1;
}

void disk_hasher_insert(disk_hasher_t* self, const void* key, const void* value) {
    size_t index = disk_hasher_bucket_index(self, key);
    size_t bucket_index;
    size_t elements;
    void* key_buff;

    bucket_index = self->indexer->indices[index].bucket_index;
    key_buff = malloc(self->key_size);

    if ( bucket_index == NO_BUCKET ) {
        bucket_index = disk_hasher_create_bucket(self);
        disk_hasher_write_key(self, value);
        disk_hasher_write_value(self, value);
        self->indexer->indices[index].bucket_index = bucket_index;
        disk_hasher_save_index(self, index);
        return;
    }

    while ( bucket_index != NO_BUCKET ) {
        disk_hasher_read_next_key(self, key_buff);
        if ( self->fn.key_comparer(key, key_buff, self->key_size) == 0 ) {
            disk_hasher_write_value(self, value);
            free(key_buff);
            return;
        } else {
            disk_hasher_skip(self, self->element_size);
            bucket_index = disk_hasher_read_size(self);
        }
    }

    /// If we're here we must create a new bucket, and we're at the end of the last one
    /// TODO

}


/// Truncate the file if necessary, filling it with zeros
void disk_hasher_truncate(disk_hasher_t* self) {
    size_t i;
    disk_hasher_indexer_t indexer;

    indexer.first_free_bucket = NO_BUCKET;

    for ( i = 0; i < self->bucket_count; ++i )
        indexer.indices[i].bucket_index = NO_BUCKET;

    fseek(self->file, 0, SEEK_SET);

    fwrite(&indexer, self->index_size, 1, self->file);
}

/// Save the index
/// TODO we copy the whole index now
void disk_hasher_save_index(disk_hasher_t* self, size_t index) {
    fseek(self->file, 0, SEEK_SET);
    fwrite(self->indexer, self->index_size, 1, self->file);
}

size_t disk_hasher_create_bucket(disk_hasher_t* self) {
    size_t size = file_size(self->file) - self->index_size;
    size_t index = size / self->bucket_size;
    void* bucket = malloc(self->bucket_size);

    *((size_t*)(bucket + self->bucket_size - sizeof(size_t))) = NO_BUCKET;

    disk_hasher_to_bucket(self, index); // should go to the end
    fwrite(bucket, self->bucket_size, 1, self->file);
    fseek(self->file, - self->bucket_size, SEEK_CUR);

    return index;
}
