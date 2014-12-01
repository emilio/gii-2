#include "JSON.hpp"
namespace JSON {
string escape(const string& str) {
	string ret = str;
	string::size_type start = 0;

	while ( (start = str.find("\"", start)) != string::npos ) {
		ret = ret.replace(start, 1, "\\\"");
		start += 2;
	}

	return ret;
}
Value::Value() {
	type = NULL_VALUE_TYPE;
}

Value::Value(const bool val) {
	type = val ? TRUE_VALUE_TYPE : FALSE_VALUE_TYPE;
}

Value::Value(const double val) {
	type = NUMBER_VALUE_TYPE;
	number_value = val;
}

Value::Value(const int val) {
	type = NUMBER_VALUE_TYPE;
	number_value = val;
}

Value::Value(const char * val) {
	type = STRING_VALUE_TYPE;
	string_value = string(val);
}

Value::Value(const string& val) {
	type = STRING_VALUE_TYPE;
	string_value = val;
}

Value::Value(const Array& val) {
	type = ARRAY_VALUE_TYPE;
	array_value = val;
}

Value::Value(const Object& val) {
	type = OBJECT_VALUE_TYPE;
	object_value = val;
}

ostream& operator<<(ostream& os, const Value& val) {
	if ( val.is_null() ) {
		return os<<"null";
	}

	if ( val.is_boolean() ) {
		return os << (val.get_boolean() ? "true" : "false");
	}

	if ( val.is_number() ) {
		return os << val.get_double();
	}

	if ( val.is_string() ) {
		return os<< "\"" << escape(val.get_string()) << "\"";
	}

	if ( val.is_array() ) {
		const Array& vec = val.get_array();

		os << "[";
		if ( ! vec.empty() ) {
			Array::const_iterator it = vec.begin();
			os << *it;
			++it;

			for ( ; it != vec.end(); ++it ) {
				os << ", ";
				os << *it;
			}
		}

		os << "]";

		return os;
	}

	if ( val.is_object() ) {
		return os << val.get_object();
	}

	return os;
}

ostream& operator<<(ostream& os, const Object& obj) {
	os << "{";
	if ( ! obj.empty() ) {
		Object::const_iterator it = obj.begin();

		os << endl;

		os << "\"" << escape(it->first) << "\": ";

		os << it->second;

		++it;

		for ( ; it != obj.end(); ++it ) {
			os << ", ";
			os << endl;
			os << "\"" << escape(it->first) << "\": " << it->second;
		}
	}
	return os << "}";
}

Value parse_object(char *src) {
	// TODO
}

Value parse_array(char *src) {
	// TODO
}

Value parse_value(const char *src) {
	char *original_str = src;
	char *str;
	Value val;

	// remove whitespace
	while ( isspace(*str) ) ++str;

	str = original_str;

	if ( *str == '{' ) {
		return parse_object(str);
	}

	if ( *str == '[' ) {
		return parse_array(str);
	}
	// String
	if ( *str == '"' ) {
		while ( ++str ) {
			if ( *str == '\\' ) {
				++str;
			}

			// TODO: allow whitespace at the end and throw error if not
			if ( *str == '"' && ! *(str + 1)) {
				*str = '\0';
			}
		}
		return JSON::Value(original_str + 1);
	}

	if ( strcmp(str, "null") == 0 ) {
		return JSON::Value();
	}

	if ( strcmp(str, "false") == 0 ) {
		return JSON::Value(false);
	}

	if ( strcmp(str, "true") == 0 ) {
		return JSON::Value(true);
	}

	return JSON::Value(atof(str));
}
}
