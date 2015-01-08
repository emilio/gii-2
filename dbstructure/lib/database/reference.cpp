#include "reference.hpp"
#include "utils.hpp"

namespace database {
reference::reference(const char *tname, const char *cname, const char *rtname, const char *rcname) {
	table_name_ = utils::copy_string(tname);
	column_name_ = utils::copy_string(cname);
	referenced_table_name_ = utils::copy_string(rtname);
	referenced_column_name_ = utils::copy_string(rcname);
}

reference::~reference() {
	delete[] table_name_;
	delete[] column_name_;
	delete[] referenced_table_name_;
	delete[] referenced_column_name_;
}

} // namespace database
