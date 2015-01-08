#ifndef __DATABASE_FIELD_CPP
#define __DATABASE_FIELD_CPP
#include <cstring>
#include <mysql/mysql.h>
#include "reference.hpp"

namespace database {
class field {
	char *name_;
	unsigned int flags_;
	enum enum_field_types type_;
	reference *ref_;
public:
	field(const char *, size_t, unsigned int, enum enum_field_types, reference *);
	field(const char *n, size_t n_len, unsigned int f, enum enum_field_types t) : field(n, n_len, f, t, NULL) {};
	field(const char *n, unsigned int f, enum enum_field_types t) : field(n, strlen(n), f, t, NULL) {};

	~field();

	const char * name()           const { return name_; }
	enum enum_field_types type()  const { return type_; }
	unsigned int          flags() const { return flags_; }
	const reference * ref() const { return ref_; }
	void ref(reference* r)       { ref_ = r; }
};
} // namespace database
#endif
