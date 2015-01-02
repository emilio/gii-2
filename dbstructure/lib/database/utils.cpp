#include "utils.hpp"

namespace database {
namespace utils {

char *copy_string(const char *source, size_t len) {
	char *ret = new char[len + 1];
	strncpy(ret, source, len);
	ret[len] = '\0';
	return ret;
}

}
}
