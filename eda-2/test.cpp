#include "stack.hpp"
#include "stack.cpp" // fast compilation
#include <iostream>

int main() {
	eda::stack<int> s;

	for (int i = 0; i < 10; ++i )
		s.push(i);

	while ( ! s.empty() ) {
		std::cout << ' ' << s.top();
		s.pop();
	}

	std::cout << std::endl;
	return 0;
}
