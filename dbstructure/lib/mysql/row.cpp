#include "mysql.hpp"
#include <cstring>
namespace mysql {
const char * row::operator[](const char *field_name) const {
	for ( unsigned int i = 0; i < field_count_; ++i )
		if ( strcmp(fields_[i].name, field_name) == 0 )
			return row_[i];

	return NULL;
}
} //  namespace mysql
