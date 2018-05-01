#include "WordString.h"

#include "Min.h"

namespace Val {
	WordString::WordString(const std::string & fullString) : words(), numberOfCharacters(0) {
		add(fullString);
	}
	WordString::WordString(const WordString & wordString) : words(wordString.words), numberOfCharacters(wordString.numberOfCharacters) {
	}
	WordString::~WordString() {
	}
	std::string WordString::get(unsigned int index) const {
		return (index < words.size()) ? words[index] : "";
	}
	void WordString::add(const std::string & word) {
		unsigned int start = 0;
		unsigned int length = 0;
		unsigned int whitespaceCount = 0;
		for (const auto& c : word) {
			if (c == ' ') {
				whitespaceCount++;
				if (length != 0) {
					if (start == 0) {
						words.push_back(word.substr(start, length));
					} else {
						words.push_back(word.substr(start + 1, length - 1));
					}
					//start++;
					start += length;
					length = 0;
				}
			}
			length++;
		}
		if (length > 1 && words.size() > 0) {
			words.push_back(word.substr(start + 1, length - 1));
		}
		if (length == word.size()) {
			words.push_back(word);
		}
		numberOfCharacters += (word.size() - whitespaceCount) + (words.size() - 1);
	}
	void WordString::set(const std::string & word, unsigned int index) {
		if (index < words.size()) {
			numberOfCharacters -= words[index].size();
			words[index] = word;
			numberOfCharacters += words[index].size();
		}
	}
	void WordString::clear() {
		words.clear();
		numberOfCharacters = 0;
	}
	void WordString::removeFromFront(const WordString & string) {
		WordString str = string;
		if (words.size() == 0)
			return;
		
		if (str.words.size() > 1) {
			unsigned int length = str.words.size() - 1;
			for (auto iter = words.begin(); iter != words.begin() + (length); iter++) {
				numberOfCharacters -= iter->size();
			}
			numberOfCharacters -= length - 1;
			words.erase(words.begin(), words.begin() + (Min<unsigned int>(length, words.size())));
			str.words.erase(str.words.begin(), str.words.begin() + (length));
		}

		if (str.words[0] != words[0]) {
			numberOfCharacters -= (words[0].size() - str.words[0].size());
			words[0] = words[0].substr(str.words[0].length(), words[0].size() - str.words[0].size());
		} else {
			if (words.size() != 1) {
				numberOfCharacters -= words[0].size();
				words.erase(words.begin(), words.begin() + (1));
			} else {
				words.clear();
				numberOfCharacters = 0;
			}
		}
	}
	std::string WordString::concatenateWords(unsigned int maxWord) {
		std::string result = "";

		unsigned int c = 0;
		for (const auto& word : words) {
			result += word;
			c++;
			if (c == maxWord)
				return result;
			if (c != words.size())
				result += " ";
		}

		return result;
	}
	std::string WordString::concatenateCharacters(unsigned int maxCharacters) {
		std::string result = "";
		result.reserve(maxCharacters);

		unsigned int c = 0, charactersRemaining = maxCharacters, wordCount = 0;
		for (const auto& word : words) {
			if (charactersRemaining == 0)
				break;

			if (c + word.size() < charactersRemaining) {
				result += word;
				charactersRemaining -= word.size();
				c += words.size();
				wordCount++;
			} else if (c + word.size() == charactersRemaining) {
				result += word;
				charactersRemaining = 0;
			} else {
				result += word.substr(0, charactersRemaining);
				charactersRemaining = 0;
			}
			if (charactersRemaining == 0)
				break;

			if (wordCount != words.size()) {
				result += " ";
				c++;
			}
		}

		return result;
	}
	std::string WordString::concatenate() {
		if (words.size() == 1)
			return words[0];

		std::string result = "";
		result.reserve(numberOfCharacters);

		unsigned int c = 0;
		for (const auto& word : words) {
			result += word;
			c++;
			if (c != words.size())
				result += " ";
		}

		return result;
	}
	unsigned int WordString::getNumberOfWords() const {
		return words.size();
	}
	unsigned int WordString::getNumberOfCharacters() const {
		return numberOfCharacters;
	}
}