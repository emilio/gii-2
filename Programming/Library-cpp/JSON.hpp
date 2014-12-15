#ifndef __JSON_HPP
#define __JSON_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdint.h>

namespace JSON {

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::ostream;

typedef JSVAL

// TODO: implement
string escape(const string& str);

class Value {
	public:
		typedef map<string, Value> object_type;
		typedef vector<Value> array_type;
	private:
		enum value_type {
			NULL_VALUE_TYPE,
			TRUE_VALUE_TYPE,
			FALSE_VALUE_TYPE,
			NUMBER_VALUE_TYPE,
			STRING_VALUE_TYPE,
			ARRAY_VALUE_TYPE,
			OBJECT_VALUE_TYPE
		};

		union {
			value_type type;
			double number_value;
			string string_value;
			array_type array_value;
			object_type object_value;
		} u;

	public:
		bool get_boolean() const {
			return type == TRUE_VALUE_TYPE ? true : false;
		}

		int get_int() const {
			return (int) number_value;
		}

		double get_double() const {
			return number_value;
		}

		string& get_string() {
			return string_value;
		}

		array_type& get_array() {
			return array_value;
		}

		object_type& get_object() {
			return object_value;
		}

		const string& get_string() const {
			return string_value;
		}

		const array_type& get_array() const {
			return array_value;
		}

		const object_type& get_object() const {
			return object_value;
		}

		bool is_null() const {
			return type == NULL_VALUE_TYPE;
		}

		bool is_boolean() const {
			return type == FALSE_VALUE_TYPE || type == TRUE_VALUE_TYPE;
		}

		bool is_number() const {
			return type == NUMBER_VALUE_TYPE;
		}

		bool is_string() const {
			return type == STRING_VALUE_TYPE;
		}

		bool is_array() const {
			return type == ARRAY_VALUE_TYPE;
		}

		bool is_object() const {
			return type == OBJECT_VALUE_TYPE;
		}

		Value();
		Value(const bool);
		Value(const int);
		Value(const double);
		Value(const char *);
		Value(const string&);
		Value(const array_type&);
		Value(const object_type&);

		friend ostream& operator<<(ostream&, const Value&);
		friend ostream& operator<<(ostream&, const object_type&);
};

typedef Value::array_type Array;
typedef Value::object_type Object;


Value parse(const char *);

}

#endif
