#pragma once

#include <vector>
#include <string>

namespace Val {
	class WordString {
	public:
		WordString(const std::string& fullString);
		WordString(const WordString& wordString);
		~WordString();

		std::string get(unsigned int index) const;
		void add(const std::string& word);
		void set(const std::string& word, unsigned int index);
		void clear();
		void removeFromFront(const WordString& str);

		//Gets you a string with # of words so concatWords(10) would yield something like "I like development because it aligns with my appreciation of" problem solving, with the end chopped of.
		std::string concatenateWords(unsigned int maxWord);
		//Gets you a string with # of characters so concatChars(10) would yield something like "I like dev"elopment with the elopment chopped of
		std::string concatenateCharacters(unsigned int maxCharacters);
		//Gets you a string with every word in it and whitespace between.
		std::string concatenate();
		
		unsigned int getNumberOfWords() const;
		unsigned int getNumberOfCharacters() const;
		const std::string operator[](unsigned int index) const {
			return words[index];
		};
	protected:
		std::vector<std::string> words;
		unsigned int numberOfCharacters;
	};

}