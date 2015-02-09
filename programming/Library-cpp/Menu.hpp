#ifndef __MENU_HPP
#define __MENU_HPP

#include <iostream>
#include <vector>
namespace Menu {

using std::cout;
using std::endl;
using std::string;
using std::vector;

string EXIT_LABEL = "Exit";
string CHOOSE_LABEL = "Choose an option: ";

class Option {
	public:
		// Option callback type is a function returning an int
		// If that int is 0, the menu execution stops
		// Else the menu will render again
		typedef int (*callback_type)(void);
		string description;
	private:
		callback_type callback;
	public:
		Option(callback_type, string);
		int execute();
};

class Menu {
	private:
		string description;
		vector<Option> options;
		int partial_execute();
	public:
		Menu (string);
		/** Renders the menu and waits for response */
		void render();

		Menu& addOption(Option::callback_type cb, string desc);
};
}
#endif
