#include "Main.hh"

const char *define_help_message() {
	const char *help_msg =
R"(guess-word.

    Usage:
      guess-word (-h|--help)
      guess-word --version
      guess-word --length=<n> [--guesses=<g>] [--time=<t>]

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

template <class Streamable>
void dbgln(string key, Streamable value) {
#ifndef NDEBUG
	cerr << "# " << key << ": " << value << endl;
#endif
}

unsigned short pick_random(unsigned short num_values) {
	typedef high_resolution_clock hrc;
	hrc::time_point tp_seed = hrc::now();
	unsigned int sc_seed = tp_seed.time_since_epoch().count();
	dbgln("Seed", sc_seed);
	default_random_engine re_random;
	re_random.seed(sc_seed);
	uniform_int_distribution<int> id_random(0, num_values - 1);
	return id_random(re_random);
}

string select_word(const docopt::value &do_length) {
	string s_length{str_from_arg(do_length)};
	int i_length{int_from_arg(do_length)};
	dbgln("iLength", i_length);
	dbgln("sLength", s_length);
	dbgln("Confdir", CONFDIR);
	ostringstream path;
	path << CONFDIR << '/' << "words-" << s_length << ".txt";
	dbgln("Path", path.str());
	ifstream i{path.str(), std::ios_base::in};
	if (!i.good()) {
		ostringstream err;
		err << "No wordlist found for length " << i_length;
		err << ".";
		throw runtime_error(err.str());
	}
	auto num_words = std::count(
		std::istreambuf_iterator<char>(i),
		std::istreambuf_iterator<char>(),
		'\n'
	);
	dbgln("Words", num_words);
	unsigned short rnd{pick_random(num_words)};
	dbgln("Rnd", rnd);
	++rnd; ++rnd; // inc twice due to loop structure
	i.seekg(0, i.beg); // return to start of file
	string chosen{""};
	for ( ; --rnd; ) {
		getline(i, chosen);
	}
	dbgln("Chosen", chosen);
	i.close();
	return chosen;
}

void output_letters(char *guess_matrix, char *highlight_matrix) {
	// TODO: bleah. Fancy this up a bit.
	cout << guess_matrix << endl;
	cout << highlight_matrix << endl;
}

void check_letters(
	string s_word,
	string guess,
	char **guess_matrix,
	char **highlight_matrix
) {
	// TODO: make this work
	string deletable{s_word};
	for (
		unsigned short index{0};
		index <= s_word.length();
		++index
	) {
		dbgln("Deletable", deletable);
		// right letter, right place
/*
		if (guess[index] == s_word[index]) {
			guess_matrix[index] = guess[index];
			deletable[index] = ' ';
		} else {
			// right letter, wrong place
			// if letter is in deletable, mark and
			//     delete it
			for (
				auto it = deletable.begin();
				it < deletable.end();
				++it
			) {
				if (guess_matrix[index] == *it) {
					unsigned short i{0};
					i += it - deletable.begin();
//					highlight_matrix[i] = '^';
					deletable.erase(it);
				}
			}
		}
*/
	}
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
	dbgln("iGuesses", i_guesses);
	dbgln("sGuesses", s_guesses);
	dbgln("iTime", i_time);
	dbgln("sTime", s_time);
	dbgln("Word", s_word);
	char *guess_matrix = new char[s_word.length() + 1];
	char *highlight_matrix = new char[s_word.length() + 1];
	memset(guess_matrix, 0, s_word.length() + 1);
	memset(highlight_matrix, 0, s_word.length() + 1);
	// populate default guess_matrix
	guess_matrix[0] = s_word[0]; // we get the first letter
	for (
		unsigned short index{1};
		index < s_word.length();
		++index
	) {
		guess_matrix[index] = '.';
	}

	while (i_guesses-- > 0) {
		output_letters(guess_matrix, highlight_matrix);
		cout << "Enter your guess:" << endl;
		string guess{""};
		cin >> guess;
		// process guess
		// word guessed correctly
		if (guess == s_word) {
			cout << "Congrats, you won!" << endl;
			return;
		}
		check_letters(
			s_word,
			guess,
			&guess_matrix,
			&highlight_matrix
		);
		cout << "No, it's not \"" << guess << "\"." << endl;
	}

	cout << "Oh, bad luck. It was \"" << s_word << "\"." << endl;

	delete guess_matrix;
}

int main(int num_args, const char **args) {
	const char *help_msg = define_help_message();
#ifndef NDEBUG
	cerr << "Build: debug" << endl;
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
		try {
			guessing_loop(
				select_word(dargs["--length"]),
				dargs["--guesses"],
				dargs["--time"]
			);
			end_this = ask_yn("Play again?");
		} catch (exception &e) {
			cerr << e.what() << endl;
			return 1;
		}
	} while (end_this == 'y' || end_this == 'Y');
	return 0;
}
