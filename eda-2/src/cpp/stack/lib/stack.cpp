#include "stack.hpp"

namespace eda {

template<class T, class Allocator>
void stack<T, Allocator>::push(const T& val) noexcept {
	if ( size_ == capacity_ )
		grow();

	data[size_++] = val;
};

template<class T, class Allocator>
void stack<T, Allocator>::push(T&& val) noexcept {
	if ( size_ == capacity_ )
		grow();

	data[size_++] = val;
};

template<class T, class Allocator>
void stack<T, Allocator>::reserve(size_type capacity) noexcept {
	// If no data yet...
	if ( data == nullptr ) {
		data = alloc.allocate(capacity);
		capacity_ = capacity;
	// If we have to reserve something...
	} else if (capacity_ < capacity) {
		T* tmp = data;
		data = alloc.allocate(capacity);
		capacity_ = capacity;
		std::copy(tmp, tmp + size_, data);
		alloc.deallocate(tmp);
	}

	// Else we already have enough capacity
}

template<class T, class Allocator>
void stack<T, Allocator>::grow() noexcept {
	if ( capacity_ == 0 )
		reserve(initial_capacity);
	else
		reserve(capacity_ * 2);
}
} // namespace
