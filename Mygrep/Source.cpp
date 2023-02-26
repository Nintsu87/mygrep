#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// function with 2 arguments
void tryOpening(string file_name, string search_string) {
	ifstream infile;
	bool checker = false;
	infile.open(file_name);
	string line;
	// trying to open the user input file and give error if not possible
	if (infile) {
		try {
			while (getline(infile, line)) {
				if (infile.fail()) {
					throw "Couldn't open the file. (failbit)\n";
				}
				else if (infile.bad()) {
					throw "Couldn't handle i/o. (badbit)\n";
				}
				// if the string wanted to find was in file line, print the line
				if (line.find(search_string) < line.length()) {
					checker = true;
					cout << line << endl;
				}
			}
			infile.close();
			// if no findings, tell user
			if (!checker) {
				cout << "\"" << search_string << "\" NOT found in \"" << file_name << "\"\n";
			}

		}
		catch (const char* message) {
			cout << message;
		}

	}
	// check for errors
	else {
		if (infile.fail()) {
			cout << "Couldn't open the file. (failbit)\n";
		}
		else if (infile.bad()) {
			cout << "Couldn't handle i/o. (badbit)\n";
		}
	}
}


// function for opening file with 3 arguments command included
void tryOpening(string file_name, string search_string, string command) {
	ifstream infile;
	infile.open(file_name);
	string line,
		line2,
		search_string2;
	bool line_count_b = false,
		row_count_b = false,
		reverse_b = false,
		ignore_cases_b = false,
		nothing_b = false;
	int line_count = 0,
		row_count = 0;

	// go trough command and create booleans if correct one found
	for (int i = 0; i < command.length(); i++){
		
		if (command.find("l") < command.length()) {
			row_count_b = true; // row numbers
		}
		
		if (command.find("o") < command.length()) {
			line_count_b = true; // line counting
		}

		if (command.find("r") < command.length()) {
			reverse_b = true; // reverse
		}

		if (command.find("i") < command.length()) {
			ignore_cases_b = true; // ignore cases
		}
	}
	// if non of them are added, there will be a note about no options
	if (!(row_count_b || line_count_b || reverse_b || ignore_cases_b)) {
		cout << "You didn't use any actual option. Doing only the simple search.\n";
	}
	
	if (infile) {
		// try reading the file
		try {
			while (getline(infile, line)) {
				
				if (infile.fail()) {
					throw "Couldn't open the file. (failbit)\n";
				}
				else if (infile.bad()) {
					throw "Couldn't handle i/o. (badbit)\n";
				}
				// count rows by every line in the file
				row_count++;

				// create new strings for ignore case shananigans
				search_string2 = search_string;
				line2 = line;

				// if chosen ignore case, turn shananigan variables into upper
				if (ignore_cases_b) {
					for (auto& c : search_string2) c = (char)toupper(c);
					for (auto& c : line2) c = (char)toupper(c);
				}

				// if reverse is on, go trough all lines that does NOT find search variable
				if (reverse_b) {

					if (!(line2.find(search_string2) < line.length())) {
						line_count++;
						if (row_count_b) {
							cout << row_count << ": ";
						}
						cout << line << endl;
					}

				}
				// go trough all lines that does find search variable
				else {
					if (line2.find(search_string2) < line.length()) {
						line_count++;
						if (row_count_b) {
							cout << row_count << ": ";
						}
						cout << line << endl;
					
					}
				}

			}
			// without reverse print alittle different than with reverse
			if (line_count_b && !reverse_b) {
				cout << "\nOccurrences of lines containing \"" << search_string << "\": " << line_count << endl;
			}
			else if (line_count_b && reverse_b) {
				cout << "\nOccurrences of lines NOT containing \"" << search_string << "\": " << line_count << endl;
			}
			// close the file
			infile.close();

			// if no lines were found print accordingly
			if (line_count == 0) {
				cout << "\"" << search_string << "\" NOT found in \"" << file_name << "\"\n";
			}

		}
		catch (const char* message) {
			cout << message;
		}

	}
	else {
		if (infile.fail()) {
			cout << "Couldn't open the file. (failbit)\n";
		}
		else if (infile.bad()) {
			cout << "Couldn't handle i/o. (badbit)\n";
		}
	}
}

int main(int argc, char* argv[]) {
	string text = "",
		toyText = "",
		searchInput = "";
	vector<int> indexes;
	bool isInString = false;
	int counter = 0,
		indexi;

	// if user only writes the .exe-file on command line
	if (argc == 1) {
		// take user input
		cout << "Give a string from which to search for: ";
		getline(cin, text);
		cout << "Give search string: ";
		getline(cin, searchInput);
		toyText = text;

		// go trough the search from text
		for (int i = 0; i < text.length(); i++) {
			// look for search variable in the text
			indexi = toyText.find(searchInput);
			if (indexi < toyText.length()) {
				// count how many of them there is for better handling 
				counter++;
				// add index on vector for more detailed printing
				indexes.push_back(indexi + (text.length() - toyText.length()));
				// create substring from the text to search more times
				toyText = toyText.substr((indexi + searchInput.length()), (toyText.length() - searchInput.length()));
			}
		}

		// if any search variable was in the text, print all of indexes
		if (counter >= 1) {
			cout << "\"" << searchInput << "\" found in \"" << text << "\" in position ";
			for (int i = 0; i < indexes.size(); i++) {
				cout << indexes[i];
				if (i != indexes.size() - 1) {
					cout << ", ";
				}
			}
			cout << endl;
		}
		// if non was found print that
		else {
			cout << "\"" << searchInput << "\" NOT found in \"" << text << "\"\n";
		}
	}

	// if there is 3 commandline arguments use arguments to open the file and search lines
	else if (argc == 3) {
		string file_name = argv[2],
			search_word = argv[1];
		tryOpening(file_name, search_word);	
	}

	// if there is 4 commandline arguments
	else if (argc == 4) {
		string full_command = argv[1],
			// create substring for actual commands
			command = full_command.substr(2, (full_command.length() - 2)),
			// create substring for testing input
			arg1test = full_command.substr(0, 2),
			file_name = argv[3],
			search_word = argv[2];

		// if the options argument starts with "-o", run the function
		if (arg1test == "-o" && command.length() > 0) {
			if (command.find("o") < command.length() || command.find("l") < command.length() || command.find("i") < command.length() || command.find("r") < command.length()) {
				tryOpening(file_name, search_word, command);
			}
			else {
				cout << "Argument after .exe file is not correct: " << argv[1];
				return 1;
			}
		}
		// if not, give user some feedback and close the program
		else {
			cout << "You forgot the actual options: " << argv[1];
			return 1;
		}
	}
	// if only 2 arguments, give feedback
	else if (argc == 2) {
		cout << "Nothing programmed with 2 command line arguments";
	}

	// if user gives too many arguments, give feedback
	else {
		cout << "You have too many arguments: ";
		for (int i = 1; i < argc; i++) {
			cout << argv[i] << " ";
		}
	}

	return 0;
}