#include "field.hpp"
#include "utils.hpp"

namespace database {

field::field(const char *n, size_t n_len, unsigned int f, enum enum_field_types t, reference *ref) : flags_(f), type_(t), ref_(ref) {
	name_ = utils::copy_string(n, n_len);
}

field::~field() {
	if ( name_ != NULL )
		delete[] name_;
}

}
