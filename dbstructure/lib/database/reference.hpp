#ifndef __DATABASE_REFERENCE_HPP
#define __DATABASE_REFERENCE_HPP
namespace database {
class reference {
	char *table_name_;
	char *column_name_;
	char *referenced_table_name_;
	char *referenced_column_name_;

public:
	const char * table_name()             const { return table_name_; }
	const char * column_name()            const { return column_name_; }
	const char * referenced_table_name()  const { return referenced_table_name_; }
	const char * referenced_column_name() const { return referenced_column_name_; }

	reference(const char *tname, const char *cname, const char *rtname, const char *rcname);
	~reference();
};
} // namespace database
#endif
