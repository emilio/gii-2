#ifndef __MACROS_HPP
#define __MACROS_HPP

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define FATAL_ERROR(streamargs) do { \
	std::cerr << std::endl << "(error) " << streamargs << std::endl; \
	exit(0); \
} while (0)
#endif
