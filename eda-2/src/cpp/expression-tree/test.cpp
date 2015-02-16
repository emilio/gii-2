#include <iostream>
#include "lib/tree.hpp"
#include "lib/tree.cpp"

eda::expr::binary_op<int> sum = [] (int l, int r) {
	return l + r;
};

eda::expr::binary_op<int> mul = [] (int l, int r) {
	return l * r;
};

eda::expr::unary_op<int> neg = [] (int a) {
	return -a;
};

int main() {
	typedef eda::expr::expression_tree<int> tree;
	typedef std::shared_ptr<tree> ptr;

	ptr a(new tree(2));
	ptr c(new tree(neg, a));

	std::cout << c->get_value() << std::endl;
	// tree t(sum, ptr(new tree(mul, ptr(new tree(3)), ptr(new tree(3)))), ptr(new tree(4)));
	// std::cout << t.get_value() << std::endl;

	return 0;
}
