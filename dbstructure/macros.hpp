#ifndef __MACROS_HPP
#define __MACROS_HPP

#define FATAL_ERROR(streamargs) do { \
	std::cerr << "(error) " << streamargs << std::endl; \
	exit(0); \
} while (0)
#endif
