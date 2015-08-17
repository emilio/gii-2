#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define REGS_PER_BUCKET 5
#define BUCKETS 20
#define OVERFLOW_BUCKETS 4
// #define DEBUG(msg, ...) fprintf(stderr, "[debug]" msg "\n", ##__VA_ARGS__)
#define DEBUG(...)
#define STATIC_ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(*arr))

typedef struct alumn {
    char dni[9];
    char first_name[19];
    char mid_name[19];
    char last_name[19];
    char region[11];
} alumn_t;

typedef struct bucket {
    alumn_t alumns[REGS_PER_BUCKET];
    int element_count;
} bucket_t;

size_t hash(const char* dni) { return atoi(dni) % BUCKETS; }

size_t hash_alumn(alumn_t* alumn) { return hash(alumn->dni); }

void copy_alumn(alumn_t* src, alumn_t* dest) { *dest = *src; }

int hash_truncate_file(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    bucket_t bucket;
    size_t i;

    DEBUG("Truncating file `%s`. Buckets [%d; %zu]", filename,
          BUCKETS + OVERFLOW_BUCKETS, sizeof(bucket));

    bucket.element_count = 0;

    if (!fp)
        return -1;

    for (i = 0; i < (BUCKETS + OVERFLOW_BUCKETS); ++i) {
        if (!fwrite(&bucket, sizeof(bucket_t), 1, fp)) {
            perror("fwrite");
            fclose(fp);
            return -1;
        }
    }

    return 0;
}

void hash_write_bucket(FILE* fp, size_t index, bucket_t* bucket) {
    assert(fp);
    assert(index < BUCKETS + OVERFLOW_BUCKETS);

    fseek(fp, sizeof(bucket_t) * index, SEEK_SET);
    fwrite(bucket, sizeof(bucket_t), 1, fp);
}

void hash_read_bucket(FILE* fp, size_t index, bucket_t* bucket) {
    size_t read_elements;
    assert(fp);
    assert(index < BUCKETS + OVERFLOW_BUCKETS);

    fseek(fp, sizeof(bucket_t) * index, SEEK_SET);
    read_elements = fread(bucket, sizeof(bucket_t), 1, fp);

    DEBUG("Read %zu elements, 1 expected", read_elements);
    if (!read_elements) {
        if (ferror(fp))
            perror("fread");
        else
            DEBUG("Unexpected <eof>");
    }
}

int hash_insert(FILE* fp, alumn_t* alumn) {
    bucket_t bucket;
    size_t bucket_index = hash_alumn(alumn);
    size_t i;

    assert(fp);

    hash_read_bucket(fp, bucket_index, &bucket);

    DEBUG("Inserting alumn with dni %s in bucket %zu (%d elements now)",
          alumn->dni, bucket_index, bucket.element_count);

    /// Normal insertion
    if (bucket.element_count < REGS_PER_BUCKET) {
        copy_alumn(alumn, &bucket.alumns[bucket.element_count++]);
        hash_write_bucket(fp, bucket_index, &bucket);
        return 0;
    }

    /// Increment the count in advance, since we have the cube in main memory
    /// We'll decrement it later if couldn't insert, which is a rare case
    bucket.element_count++;
    hash_write_bucket(fp, bucket_index, &bucket);

    DEBUG("...Searching in overflow area adter increasing bucket %zu element "
          "count to %d",
          bucket_index, bucket.element_count);
    /// Go to overflow zone and search sequentially
    i = 0;
    while (i < OVERFLOW_BUCKETS) {
        hash_read_bucket(fp, BUCKETS + i, &bucket);

        DEBUG("....Searching in bucket (%zu), %d elements", BUCKETS + i,
              bucket.element_count);
        /// TODO remove repetition here
        if (bucket.element_count < REGS_PER_BUCKET) {
            copy_alumn(alumn, &bucket.alumns[bucket.element_count++]);
            DEBUG("......adding %s to overflow area (bucket %zu)",
                  bucket.alumns[bucket.element_count - 1].dni, BUCKETS + i);
            hash_write_bucket(fp, BUCKETS + i, &bucket);
            return 0;
        }
        i++;
    }

    hash_read_bucket(fp, bucket_index, &bucket);
    bucket.element_count--;
    hash_write_bucket(fp, bucket_index, &bucket);

    return -1; // No space left
}

/// If it was found returns 0, else -1
int hash_retrieve(FILE* fp, const char* dni, alumn_t* buffer) {
    size_t index = hash(dni);
    bucket_t bucket;
    size_t i, j;

    DEBUG("Searching for alumn %s in bucket %zu", dni, index);

    hash_read_bucket(fp, index, &bucket);

    if (bucket.element_count == 0)
        return -1;

    for (i = 0; i < bucket.element_count && i < REGS_PER_BUCKET; ++i) {
        if (strcmp(bucket.alumns[i].dni, dni) == 0) {
            copy_alumn(bucket.alumns + i, buffer);
            return 0;
        }
    }

    /// We searched for every element possible
    if (i < REGS_PER_BUCKET)
        return -1;

    DEBUG("...Searching in overflow area");

    j = 0;
    while (j < OVERFLOW_BUCKETS) {
        hash_read_bucket(fp, BUCKETS + j, &bucket);

        /// These cant have more than REGS_PER_BUCKET elements
        for (i = 0; i < bucket.element_count; ++i) {
            DEBUG("......found %s", bucket.alumns[i].dni);
            if (strcmp(bucket.alumns[i].dni, dni) == 0) {
                copy_alumn(bucket.alumns + i, buffer);
                return 0;
            }
        }

        j++;
    }

    return -1;
}

int file_from_sequential_to_hash(const char* sequential_file,
                                 const char* hash_file) {
    FILE* sequential_fp;
    FILE* hash_fp;
    alumn_t alumn;
    if (hash_truncate_file(hash_file) == -1)
        return -1;

    sequential_fp = fopen(sequential_file, "rb");
    if (!sequential_fp)
        return -1;
    hash_fp = fopen(hash_file, "rb+");
    if (!hash_fp) {
        fclose(sequential_fp);
        return -1;
    }

    DEBUG("Hash file open at %p", hash_fp);

    while (!feof(sequential_fp)) {
        fread(&alumn, sizeof(alumn_t), 1, sequential_fp);
        hash_insert(hash_fp, &alumn);
    }

    fclose(sequential_fp);
    fclose(hash_fp);

    return 0;
}

int main() {
    int ret;
    size_t i;
    FILE* fp;
    alumn_t alumn;

    hash_truncate_file("alumnos.hash");
    ret = file_from_sequential_to_hash("alumnos.dat", "alumnos.hash");

    assert(ret == 0);

    printf("Hash file created correctly, searching in it...\n");

    fp = fopen("alumnos.hash", "rb");
    assert(fp);

    // Found, found, not found, found with overflow
    const char* tests[] = {"7839293", "7124874", "70912324", "11000258"};

    for (i = 0; i < STATIC_ARRAY_LENGTH(tests); ++i) {
        ret = hash_retrieve(fp, tests[i], &alumn);

        printf(" [%c] %s\n", ret == 0 ? 'x' : ' ', tests[i]);
        if (ret == 0)
            printf("   %s %s %s\n", alumn.first_name, alumn.mid_name,
                   alumn.last_name);
    }

    fclose(fp);

    return 0;
}
