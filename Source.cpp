#include <iostream>
#include "RBTree.h"
#include <fstream>
#include <string>
#include <sstream>
#include <exception>
#include <regex>

int countOccurrences(const std::string& sentence, const std::string& word);

void showMenu();

class invalidFileName :public std::exception {
public:

	invalidFileName() {

		reason_ = "Error: Invalid file name";

	}

	const char* what() const noexcept override {

		return reason_.c_str();

	}
private:

	std::string reason_;
};

class emptyFile :public std::exception {
public:

	emptyFile() {

		reason_ = "Error: File is empty";

	}

	const char* what() const noexcept override {

		return reason_.c_str();

	}
private:

	std::string reason_;
};

class invalidKey :public std::exception {
public:

	invalidKey() {

		reason_ = "Error: Invalid key";

	}

	const char* what() const noexcept override {

		return reason_.c_str();

	}
private:

	std::string reason_;
};

int main() {

	try {

		int chz = 0;
		int inp = 0;
		bool flag = true;

		std::regex mask(R"((.*)\.txt)");

		std::string file;
		std::string text;
		std::string word;
		RBTree dictionary;
		std::stringstream iss;

		std::cout << "Enter file name, to insert text in dictionary: ";
		std::cin >> file;

		if (!std::regex_match(file, mask)) {
			throw invalidFileName();
		}

		std::ifstream in(file);

		if (!in.is_open()) {
			throw  invalidFileName();
		}
		else if (in.peek() == EOF) {
			throw emptyFile();
		}

		while (std::getline(in, text)) {

			iss << text;

		}

		in.close();

		for (int i = 0; i < text.size(); i++) {

			iss >> std::ws >> word;
			dictionary.insertElem(word, countOccurrences(text, word));

		}

		while (flag == true) {

			showMenu();

			std::cin >> chz;

			if (!std::cin || std::cin.peek() == ',' || std::cin.peek() == '.') {
				throw invalidKey();
			}

			switch (chz) {
			case 1:

				word = "";
				std::cout << "Enter word to delete: ";
				std::cin >> word;

				dictionary.deleteElem(word);
				break;

			case 2:

				std::cout << "Enter count of frequent words to print: ";
				std::cin >> inp;

				if (!std::cin || std::cin.peek() == ',' || std::cin.peek() == '.' || inp == 0) {
					throw RBTree::invalidAmount();
				}

				dictionary.printMostFrequentWords(inp);
				break;

			case 3:

				exit(0);

			default:

				flag = false;
				throw invalidKey();

			}
			
			

		}

	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		exit(-1);
	}


	return 0;
}

int countOccurrences(const std::string& sentence, const std::string& word) {

	int count = 0;
	int pos = 0;

	while ((pos = sentence.find(word, pos)) != std::string::npos) {
		count++;
		pos += word.length();
	}

	return count;
}

void showMenu() {
	std::cout << std::endl;
	std::cout << "Enter: " << std::endl;
	std::cout << "1 - to delete word from dictionary" << std::endl;
	std::cout << "2 - to print N most frequent words in dictionary" << std::endl;
	std::cout << "3 - to exit" << std::endl;
}

