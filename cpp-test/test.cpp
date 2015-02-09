#include <iostream>
#include <cstdlib>
#include <ctime>

#define LEN 1000
int main() {
	int i;
	int* ptr = new int[LEN];
	int* oldptr = ptr;

	srand(time(NULL));
	std::cout << i << std::endl;

	for ( int j = 0; j < LEN; ++j )
		ptr[j] = rand(); // Trying to generate some garbage

	delete[] ptr;

	ptr = new int[LEN]; // Will it initialize?

	std::cout << "PTRS: " << (void *) ptr << " | " << (void *) oldptr << std::endl; // should be the same

	for ( int j = 0; j < LEN; ++j )
		std::cout << ptr[j] << '\t';

	return 0;
}
