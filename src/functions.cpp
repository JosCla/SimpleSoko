// functions.cpp

// C++ Standard Library
#include <vector>
using std::vector;
#include <string>
using std::string;

// Our files
#include "functions.h"

// Splits a string into its characters
vector<char> splitToChar(string str) {
	vector<char> ret;
	for (unsigned int i = 0; i < str.length(); ++i) {
		ret.push_back(str.at(i));
	}
	return ret;
}

// Splits a string across a specified character
vector<string> split(string str, char splitChar) {
	vector<string> ret;
	
	long unsigned int prev = 0;
	long unsigned int next = 0;

	next = str.find(splitChar, next);
	while (next != string::npos) {
		ret.push_back(str.substr(prev, next - prev));
		prev = ++next;
		next = str.find(splitChar, next);
	}

	ret.push_back(str.substr(prev));

	return ret;
}

// Splits a string into lines, preserving words, given a maximum line width
vector<string> splitToLines(string str, unsigned int lineWidth) {
	vector<string> ret = {""};
	unsigned int currLine = 0;

	// Splitting the string across spaces
	vector<string> words = split(str, ' ');

	// Going through each word and adding them
	for (unsigned int i = 0; i < words.size(); ++i) {
		// Checking if the word will overflow onto the next line
		if (ret.at(currLine).length() + words.at(i).length() > lineWidth) {
			if (ret.at(currLine).length() > 0) {
				// If we already have words on this line, advance a line
				++currLine;
				ret.push_back("");
			}

			// If we can fit our next word entirely onto this line, do it
			if (words.at(i).length() < lineWidth) {
				ret.at(currLine) += words.at(i);
			} else {
				// But if we can't, we need to treat this word differently
				unsigned int currPos = 0;
				while (currPos < words.at(i).length()) {
					ret.at(currLine) += words.at(i).substr(currPos, lineWidth);
					currPos += 40;
					++currLine;
					ret.push_back("");
				}
			}
		} else {
			ret.at(currLine) += words.at(i);
		}

		// Adding a space on the end if necessary
		if (ret.at(currLine).length() < lineWidth) {
			ret.at(currLine) += " ";
		}
	}

	return ret;
}
