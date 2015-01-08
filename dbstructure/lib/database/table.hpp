#ifndef __DATABASE_TABLE_HPP
#define __DATABASE_TABLE_HPP
#include "field.hpp"
#include "reference.hpp"
#include "../mysql/mysql.hpp"
#include <vector>
namespace database {
class table {
	char *name_;
	std::vector<field *> fields_;
	unsigned int field_count_;
	std::vector<reference *> references_;
public:
	const char *                    name()                          const { return name_; }
	const std::vector<field *>&     fields()                        const { return fields_; }
	const std::vector<reference *>& references()                    const { return references_; }
	unsigned int                    field_count()                   const { return field_count_; }
	const field *                   get_field_by_name(const char *) const;
	field *                         get_field_by_name(const char *);
	void                            add_reference(reference *r);

	table(const char *n, const mysql::field *f, unsigned int c);
	~table();
};
} // namespace database
#endif
