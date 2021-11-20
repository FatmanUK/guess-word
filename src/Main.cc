#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <sstream>
using std::istringstream;
using std::ostringstream;

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "docopt.h"

const unsigned short ver_maj{0};
const unsigned short ver_min{1};
const unsigned short ver_rel{0};

const char *define_help_message() {
	const char *help_msg =
R"(guess-word.

    Usage:
      guess-word (-h|--help)
      guess-word --version
      guess-word --length=<n> --guesses=<g> [--time=<t>]

    Options:
      -h --help       Show help.
      --version       Show version.
      --length=<n>    Length of word [default: 4].
      --guesses=<g>   Number of guesses [default: 6].
      --time=<t>      Time for each guess [default: 10].
)";
	return help_msg;
}

char ask_yn(string q) {
	char end_it{'x'};
	while (		end_it != 'n'
			&& end_it != 'N'
			&& end_it != 'y'
			&& end_it != 'Y') {
		cout << q << " y/n" << endl;
		cin >> end_it;
	}
	return end_it;
}

string str_from_arg(const docopt::value &o) {
	ostringstream ostr{};
	ostr << o;
	return ostr.str().substr(1, ostr.str().length() - 2);
}

int int_from_arg(const docopt::value &o) {
	int i{0};
	istringstream istr{str_from_arg(o)};
	istr >> i;
	return i;
}

string select_word(const docopt::value &do_length) {
	string s_length{str_from_arg(do_length)};
	int i_length{int_from_arg(do_length)};
	cout << "Length: " << i_length << " " << s_length << endl;
	// TODO
	return string{"base"};
}

void guessing_loop(
		const string &s_word,
		const docopt::value &do_guesses,
		const docopt::value &do_time
) {
	string s_guesses{str_from_arg(do_guesses)};
	string s_time{str_from_arg(do_time)};
	int i_guesses{int_from_arg(do_guesses)};
	int i_time{int_from_arg(do_time)};
	cout << "Guesses: " << i_guesses << " " << s_guesses << endl;
	cout << "Time: " << i_time << " " << s_time << endl;
	// TODO
}

int main(int num_args, const char **args) {
	const char *help_msg = define_help_message();
#ifndef NDEBUG
	cout << "Build: debug" << endl;
#endif
	ostringstream version;
	version << args[0] << " ";
	version << ver_maj << ".";
	version << ver_min << ".";
	version << ver_rel;
	auto dargs{
		docopt::docopt(
			help_msg,
			{args + 1, args + num_args},
			true,
			version.str()
		)
	};
	char end_this{'x'};
	do {
		guessing_loop(
			select_word(dargs["--length"]),
			dargs["--guesses"],
			dargs["--time"]
		);
		end_this = ask_yn("Again?");
	} while (end_this == 'y' || end_this == 'Y');
	return 0;
}
