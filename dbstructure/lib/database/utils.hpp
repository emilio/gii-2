#ifndef __DATABASE_UTILS_HPP
#define __DATABASE_UTILS_HPP value
#include <cstring>
namespace database {
namespace utils {

char *copy_string(const char *, size_t);
inline char *copy_string(const char *source) {	return copy_string(source, strlen(source)); }

} // database::utils
} // database
#endif
