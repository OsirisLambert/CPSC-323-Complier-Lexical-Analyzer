#include<iostream>
#include<fstream>
#include <iomanip>
#include <string>

using namespace std;

char ch;	// read file and get every character
char buffer[15];	//store characters in an array to determine isKeyword or isNumber or isReal or isIdentifier
char operators[] = "*+-=/><%";
char separator[] = "(){}[],:;";
char keywords[18][10] = { "int", "float", "bool", "if", "else", "then", "endif", "while", "whileend", "do", "doend","for", "forend", "input", "output", "and", "or", "function" };
bool hasSep = false;	// check if has separater
bool hasOpe = false;	// check if has operator
bool printSep = false;	// check if printed separter
bool printOpe = false;	//	check if printed operator
int currentIndex = 0;	// index for check isNumber() and isReal()

bool isKeyword(char buffer[]) {
	for (int i = 0; i < 18; ++i)
		if (strcmp(keywords[i], buffer) == 0)
			return true;
	return false;
}

bool isOperator(int index) {
	if (ch == operators[index])
		return true;
	else
		return false;
}

bool isSeparator(int index) {
	if (ch == separator[index])
		return true;
	else
		return false;
}

bool isReal(char buffer[]) {
	string str(buffer);
	string substr(".");

	size_t found = str.find(substr);

	if (found != string::npos) {
		for (int i = 0; i < found; i++) {
			if (!isdigit(buffer[i]))
				return false;
			continue;
		}
		for (int i = found + 1; i < currentIndex; i++) {
			if (!isdigit(buffer[i]))
				return false;
			continue;
		}
		return true;
	}
	else
		return false;
}

bool isNumber(char buffer[]) {
	for (int i = 0; i <= (currentIndex - 1); i++) {
		if (!isdigit(buffer[i]))
			return false;
		continue;
	}
	return true;
}



int main() {
	bool comment = false;	//check if it is comments
	char detection[2] = { ' ', ' ' };	// detect '!' twice, if detect, add '!' to array according to occur times
	int x = 0;	// used to counting detection array
	string filename;
	cout << "Please enter file name: ";
	cin >> filename;

	ifstream fin(filename);		//read file
	int j = 0;		// index for buffer[]
	
	if (!fin.is_open()) {
		cout << "error while opening the file\n";
		exit(0);
	}
	cout << "TOKENS               Lexemes" << endl; // print table contents

	while (!fin.eof()) {
		ch = fin.get();
	
		if (ch == '!') {	//check if is comment
			detection[x] = ch;
			x++;
			if (detection[1] == ch) {
				comment = false;
				x = 0;
				detection[0] = ' ';
				detection[1] = ' ';
			}
			else
				comment = true;
		}
		if (!comment) {	// if not comment, start lexical analyzer

			for (int i = 0; i < 9; ++i) {	//check separator
				if (isSeparator(i)) {
					hasSep = true;
					printSep = false;
				}
			}

			for (int i = 0; i < 8; ++i) {	//check operator
				if (isOperator(i)){
					hasOpe = true;
					printOpe = false;
				}
			}

			if (isalnum(ch) || ch == '$' || ch == '.') {	// check ch is a number or letter or '$' or '.'
				buffer[j++] = ch;		// if so, add to buffer
				currentIndex++;	
			}
			else if ((ch == ' ' || ch == '\n' || hasSep || hasOpe) && (j != 0)) {	// end of variables by detecting if j(index for buffer[]) != 0
				buffer[j] = '\0';	// set last char in buffer( might be ' ', '\n', separator, operator
				j = 0;	// reset the index for buffer[]

				if (isKeyword(buffer))	// check if the variable is keyword
					cout << "KEYWORD         =     " << buffer << endl;
				else if (isReal(buffer))	//check if the variable is real( number with '.')
					cout << "REAL            =     " << buffer << endl;
				else if (isNumber(buffer)) //check if the variable is number 
					cout << "NUMBER          =     " << buffer << endl;
				else	//check if the variable is identifier
					cout << "IDENTIFIER      =     " << buffer << endl;

				if (hasSep) {	// print separator
					cout << "SEPARATOR       =     " << ch << endl;
					hasSep = false;
					printSep = true;
				}
				else if (hasOpe){	//print operator
					cout << "OPERATOR        =     " << ch << endl;
					hasOpe = false;
					printOpe = true;
				}
				currentIndex = 0;	// reset index for isNumber() and isReal()
			}

			if (hasSep && !printSep) {	// single separator detected, print it
				cout << "SEPARATOR       =     " << ch << endl;
				printSep = true;
				hasSep = false;
			}
			else if (hasOpe && !printOpe) { // single operator detected, print it
				cout << "OPERATOR        =     " << ch << endl;
				printOpe = true;
				hasOpe = false;
			}	
		}
	}

	fin.close();	//close file
	

	system("pause");
	return 0;
}
