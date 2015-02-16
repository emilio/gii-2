#ifndef EXPRESSION_TREE_H_
#define EXPRESSION_TREE_H_

#include <memory> // std::shared_ptr
#include <functional> // std::function

namespace eda {
namespace expr {

template <typename T>
using binary_op = std::function<T(T,T)>;

template <typename T>
using unary_op = std::function<T(T)>;

template <typename T>
class expression_tree {
public:
	unsigned short children_count() const noexcept { return children_count_; }
	bool has_children()             const noexcept { return children_count() != 0; }
	bool is_leave()					const noexcept { return children_count() == 0; }
	bool is_unary()                 const noexcept { return children_count() == 1; }
	bool is_binary()                const noexcept { return children_count() == 2; }

	/** TODO: try to pass by reference */
	T get_value()                   const noexcept;

	constexpr expression_tree(T val) noexcept :
		children_count_(0), left_(nullptr), right_(nullptr), value_(val) {};

	constexpr expression_tree(
		const binary_op<T>& op,
		std::shared_ptr<expression_tree<T>> lhs,
		std::shared_ptr<expression_tree<T>> rhs
	) noexcept :
		children_count_(2), left_(lhs), right_(rhs), op_(op) {};

	constexpr expression_tree(
		const unary_op<T>& op,
		const std::shared_ptr<expression_tree<T>> child
	) noexcept :
		children_count_(1), left_(child), right_(nullptr), u_op_(op) {};

	~expression_tree() {
		left_.reset();
	   	right_.reset();
		/**
		 * TODO: change design to prevent calling destructor directly
		 * Maybe:
		 *   class expression_tree<T> (with abstract get_value(), is_unary(), is_binary()...)
		 *   class value_expression_tree<T> : public expression_tree<T>
		 *   class binary_expression_tree<T> : public expression_tree<T>
		 *   class unary_expression_tree<T> : public expression_tree<T>
		 */
		if ( is_unary() )
			u_op_.~function();
		else if ( is_binary() )
			op_.~function();
	};
private:
	/** Calculated when constructed */
	unsigned short children_count_;
	std::shared_ptr<expression_tree<T>> left_;
	std::shared_ptr<expression_tree<T>> right_;

	/** TODO: fix destructor when union is used */
	union {
		binary_op<T> op_;
		unary_op<T> u_op_;
		T value_;
	};
};


} // namespace expr
} // namespace eda

#endif
