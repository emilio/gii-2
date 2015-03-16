#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#define STACK_SIZE 30000
#define DEAD_CODE_LIMIT 100
typedef unsigned char byte;

enum class State {
	READING,
	WHILE_IGNORING
};

int main(int argc, char** argv) {

	if ( argc < 2 ) {
		std::cerr << "Usage: " << argv[0] << " <file_name>" << std::endl;
		exit(1);
	}


	std::ifstream source(argv[1]);
	byte stack[STACK_SIZE] = {0};
	size_t pointer = 0;


	std::vector<char> tokens;
	size_t current_token_index = 0;

	std::stack<size_t> entry_points;
	std::stack<State> states;

	states.push(State::READING);

	char token;

	while ( true ) {
		State state = states.top();

		// std::cerr << "State: " << static_cast<int>(state) << '\t';
		// std::cerr << "Token index: " << current_token_index << '\t';

		if ( current_token_index == tokens.size() ) {
			if ( (source >> token) )
				tokens.push_back(token);
			else
				break;
		} else {
			token = tokens[current_token_index];
		}


		// std::cerr << "Token: " << token << std::endl;

		if ( state == State::WHILE_IGNORING && ! (token == ']' || token == '[') ) {
			current_token_index++;
			continue;
		}

		// std::cerr << "Interpreting token" << std::endl;

		switch ( token ) {
			case '>':
				pointer++;
				break;
			case '<':
				pointer--;
				break;
			case '+':
				stack[pointer]++;
				break;
			case '-':
				stack[pointer]--;
				break;
			case '.':
				putchar(stack[pointer]);
				fflush(stdout);
				break;
			case ',':
				stack[pointer] = getchar();
				fflush(stdin);
				// std::cerr << "Read: " << static_cast<int>(stack[pointer]) << std::endl;
				break;
			case '[':
				entry_points.push(current_token_index);
				// std::cerr << "Entering while " << static_cast<int>(stack[pointer]) << " | " << entry_points.top() << std::endl;
				if ( state == State::WHILE_IGNORING || ! stack[pointer] )
					states.push(State::WHILE_IGNORING);
				break;
			case ']':
				if ( state == State::WHILE_IGNORING )
					states.pop();
				else
					current_token_index = entry_points.top() - 1;

				entry_points.pop();
				break;
			default:
				break; // ignore
		}

		current_token_index++;

		/** Dead code elimination */
		if ( current_token_index > DEAD_CODE_LIMIT && entry_points.empty() ) {
			tokens.clear();
			current_token_index = 0;
		}
	}
}
