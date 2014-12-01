#include <iostream>
// #include "Menu.hpp"
// #include "Menu.cpp"
#include "JSON.hpp"
#include "JSON.cpp"

using std::cout;
using std::endl;
/** Show menu: returns 0 to indicate exit */
int sayHello() {
	cout << "Hello!" << endl;
	return 0;
}

int sayHelloAndStay() {
	static int i = 2;
	cout << "Hello!" << endl;
	i--;
	return i;
}

int main() {
	JSON::Object obj;
	obj["a"] = JSON::Value("Hello");
	obj["b"] = JSON::Value(3);
	obj["C"] = JSON::Value(false);
	obj["blah\"quote"] = JSON::Value(2);
	obj["c"] = JSON::Value(JSON::Array());
	JSON::Value val = JSON::Value(obj);

	cout << val << endl;

	return 0;
}
