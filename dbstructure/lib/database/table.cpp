#include "table.hpp"
#include "utils.hpp"
namespace database {

table::table(const char *name, const mysql::field *f, unsigned int field_count)
: field_count_(field_count) {
	name_ = utils::copy_string(name);
	fields_ = std::vector<field*>(field_count);

	for ( unsigned int i = 0; i < field_count; ++i ) {
		fields_[i] = new field(f[i].name, f[i].name_length, f[i].flags, f[i].type);
	}
}

table::~table() {
	delete[] name_;

	for ( std::vector<reference *>::iterator it = references_.begin(); it != references_.end(); ++it )
		delete *it;

	for ( std::vector<field *>::iterator it = fields_.begin(); it != fields_.end(); ++it )
		delete *it;
}

field * table::get_field_by_name(const char *name) {
	for ( std::vector<field *>::iterator it = fields_.begin(); it != fields_.end(); ++it ) {
		if ( strcmp((*it)->name(), name) == 0 )
			return *it;
	}
	return NULL;
}

const field * table::get_field_by_name(const char *name) const {
	for ( std::vector<field *>::const_iterator it = fields_.begin(); it != fields_.end(); ++it )
		if ( strcmp((*it)->name(), name) == 0 )
			return *it;
	return NULL;
}

void table::add_reference(reference* r) {
	database::field *f;
	references_.push_back(r);

	f = get_field_by_name(r->column_name());
	if ( f != NULL )
		f->ref(r);
}

}
