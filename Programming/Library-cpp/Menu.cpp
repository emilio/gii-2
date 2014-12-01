#include "Menu.hpp"

namespace Menu {
using std::cin;
using std::cout;
using std::vector;
using std::string;

Option::Option(Option::callback_type cb, string desc) {
	callback = cb;
	description = desc;
}

int Option::execute() {
	return callback();
}


Menu::Menu(string desc) {
	description = desc;
}

Menu& Menu::addOption(Option::callback_type cb, string desc)  {
	options.push_back(Option(cb, desc));
	return *this;
}

int Menu::partial_execute() {
	typedef vector<Option>::size_type size;
	size i;
	cout << description << endl;
	for ( i = 0; i != options.size(); i++ ) {
		cout << i + 1 << ") " << options[i].description << endl;
	}

	cout << i + 1 << ") " << EXIT_LABEL << endl;

	size r = 0;

	do {
		cout << CHOOSE_LABEL;
		cin >> r;
	} while ( r == 0 || r > options.size() + 1 );

	// Exit, noop
	if ( r == options.size() + 1) {
		return 0;
	}

	return options[r - 1].execute();
}

/** Renders the menu indefinitely and returns the last response, always 0 */
void Menu::render() {
	while ( partial_execute() );
}

} // namespace
