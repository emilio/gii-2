#ifndef __STACK_H__
#define __STACK_H__
#include <memory>
namespace eda {

/**
 * Contiguous stack of data
 */
template < typename T, class Allocator = std::allocator<T> >
class stack {
public:
	/** type alias */
	typedef size_t size_type;

	/** Initial capacity */
	static const size_type initial_capacity = 10;

	/** Default constructor, empty stack */
	constexpr stack()                                 noexcept :
		data(nullptr), size_(0), capacity_(0) {};
	stack(size_type capacity)                         noexcept :
	   	stack() { reserve(capacity); };

	~stack()                                          noexcept {
		if ( data != nullptr )
			alloc.deallocate(data, capacity_);
	}

	constexpr bool empty()                      const noexcept { return size_ == 0; };

	T& top()                                          noexcept { return data[size_ - 1]; };
	const T& top()                              const noexcept { return data[size_ - 1]; };

	void pop()                                        noexcept { --size_; };

	void push(const T& val)                           noexcept;
	void push(T&& val)                                noexcept;

	void reserve(size_type capacity)                  noexcept;

private:
	void grow() noexcept;

	size_type capacity_;
	size_type size_;
	T* data;
	Allocator alloc;

};
} // namespace
#endif
