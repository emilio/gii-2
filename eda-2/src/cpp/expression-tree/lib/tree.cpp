#include "tree.hpp"
#include <iostream>
namespace eda {
namespace expr {

template <typename T>
T expression_tree<T>::get_value() const noexcept {
	if ( is_leave() )
		return value_;

	if ( is_unary() )
		return u_op_(left_->get_value());

	return op_(left_->get_value(), right_->get_value());
}
} // namespace expr
} // namespace eda
