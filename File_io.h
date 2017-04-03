
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <time.h>
#include "HashTable.h"

#ifndef FILE_IO_H
#define FILE_IO_H
struct blanks_location
{
	int blank_index;
	int consecutive_blanks;

	blanks_location(int blank_index, int consecutive_blanks) : blank_index(0), consecutive_blanks(0) {}
};


class File_IO {
private:
	const int MIN_LENGTH = 3;
	const int MAX_LENGTH = 20;
	int line_number;

	std::vector<std::string>string_storage;
	std::string buffer;
	std::string placeholder;
	std::string tempBuff;
	std::fstream &sstream;
	std::vector<struct blanks_location> blank_space_info;

public:
	File_IO(std::fstream &sstream) :sstream(sstream) {
		buffer = "";
		placeholder = "";
		tempBuff = "";
		line_number = 1;
	}

	void readTxtInput(HashTable &dictionaryHashTable) {
		if (sstream.is_open()) {
			int i = 0;
			while (getline(sstream, tempBuff)) {
				if (i == 0) {
					for (int j = 0; j < tempBuff.length(); j++) {
						char c = tempBuff[j];
						if (isalpha(c))
							placeholder += c;
						else
							placeholder += "*";
						placeholder.clear();
					}
				}
				else {
					tempBuff;
					dictionaryHashTable.insertHashKey(tempBuff);
				}
				tempBuff.clear();
				i++;
			}
			//sstream.close();
		}
	}

	void readZoneInput(const HashTable &dictionaryHashTable) {
		tempBuff = "";
		std::string zone_substring = "---";
		std::string zone_buffer = "------------------------";
		int blank_index_loc[30] = { 0 };
		double start = time(NULL);
		int count = 0;
		//int thousands = 10000;
		std::ofstream output;
		output.open("textOutput.txt");
		if (sstream.is_open()) {
			std::string s;
			while (getline(sstream, tempBuff)) {
				//double len = tempBuff.length();
				string_storage.push_back(parseDomain(tempBuff, dictionaryHashTable, blank_index_loc));
				if(output.is_open()){
					s = string_storage.back();
					string_storage.pop_back();
					output<< s <<"\n";
				}
				blank_space_info.clear();
				if(count % 10000 == 0 && count != 0){
					double end = time(NULL);
					std::cout<< end - start <<" to process "<<count<<" domains \n";
					
				}
				count++;
			}
			double end = time(NULL);
		    std::cout<< end - start <<" to process "<<"the whole file. \n";				
		}
	}
	std::string parseDomain(const std::string &line, const HashTable &dictionaryHashTable, int blank_index_loc[]) {

		//std::string parsedString = "";
		int blank_index_count = 0;
		int valid_char_count = 0;
		std::deque<char>string_container;
		blanks_location b(0, 0);

		for (int i = 0; i < line.length(); i++) {
			if (line[i] > 64 && line[i] < 91 || line[i] == '-' || line[i] == '\'') {
				if (valid_char_count == 0 && i != 0) {
					blank_space_info.push_back(b);
					blank_index_count = 0;
					b.blank_index = 0;
					b.consecutive_blanks = 0;
				}
				string_container.push_back(line[i]);
				valid_char_count++;
			}
			else {
				if (blank_index_count == 0) {
					if (valid_char_count <= 2 && i != 0) {
						if (valid_char_count == 1) {
							string_container[i - 1] = ' ';
							blank_index_count += 1;
						}
						else {
							std::string s;
							s = std::string() + string_container[i - 2] + string_container[i - 1];
							if (!dictionaryHashTable.findKey(s)) {
								string_container[i - 2] = ' ';
								string_container[i - 1] = ' ';
								blank_index_count += 2;
							}
						}
					}
					b.blank_index = i - blank_index_count;
					blank_index_count++;
					b.consecutive_blanks = blank_index_count;
				}
				else {
					blank_index_count++;
					b.consecutive_blanks = blank_index_count;
				}
				string_container.push_back(' ');
				valid_char_count = 0;
				if (i == line.length() - 1)
					blank_space_info.push_back(b);
			}
		}
		std::string parsedString = searchTable(string_container, dictionaryHashTable);
		return parsedString;
	}

	std::string searchTable(std::deque<char> &char_deque, const HashTable &dictionaryHashTable) {

		std::string substring = "";
		std::string parsedString = "";
		int deque_length = char_deque.size();
		if (deque_length != 0) {
			for (int i = 0; i < blank_space_info.size(); i++) {
				int start, end;
				if (blank_space_info[0].blank_index != 0 && i == 0) {
					start = 0;
					if (blank_space_info.size() > 1)
						end = blank_space_info[1].blank_index;
				}
				else {
					start = blank_space_info[i].blank_index + blank_space_info[i].consecutive_blanks;
					if (i == blank_space_info.size() - 1)
						end = char_deque.size();
					else
						end = blank_space_info[i + 1].blank_index;
				}
				for (int j = start;j < end;j++) {
					if (j < char_deque.size())
						substring += char_deque[j];
					else break;
				}
				std::string b = "";
				b = recursiveHelper(dictionaryHashTable, substring);

				if (b != "") {
					parsedString += b;
					parsedString += " ";
				}

				substring = "";
			}
		}
		return parsedString;
	}
	std::string recursiveHelper(const HashTable &dictionaryHashTable, std::string &substring) {

		struct wordIndex {
			int first;
			int last;
			std::string word;
		};
		struct wordIndex w;
		std::vector<struct wordIndex>overlapTracker;
		int start = 0;
		int end = substring.length();
		std::string returnstring ="";

		std::string subOfsubstring = substring;

		if (substring.length() > 1)
		{
			while (subOfsubstring.length() > 1 && start < substring.length() - 1) {
				if (subOfsubstring != "" && dictionaryHashTable.findKey(subOfsubstring)) {
					if (subOfsubstring.length() == substring.length() || subOfsubstring.length() == substring.length() -1)
						return subOfsubstring;
					else if (subOfsubstring.length() == substring.length() - 2) {
						return subOfsubstring;
					}
					else {
						w.first = start;
						w.last = end;
						w.word = subOfsubstring;
						overlapTracker.push_back(w);
					}
				}
				if ((end + start) == substring.length()) {
					start = 0;
					end--;
					subOfsubstring = substring.substr(start, end);
				}
				else {
					start++;
					subOfsubstring = substring.substr(start, end);
				}
			}
			for (int i = 0; i < overlapTracker.size(); i++) {
				bool isSubstring = false;
				for (int j = 0; j < overlapTracker.size(); j++) {
					if (i != j) {
						if (overlapTracker[i].first >= overlapTracker[j].first &&
							overlapTracker[i].last <= overlapTracker[j].last) {
							isSubstring = true;
							break;
						}
						if (overlapTracker[i].first >= overlapTracker[j].first &&
							overlapTracker[i].first <= overlapTracker[j].last) {
							if (overlapTracker[i].word.length() < overlapTracker[i].word.length()) {
								isSubstring = true;
								break;
							}
						}
						if (overlapTracker[i].first <= overlapTracker[j].first &&
							overlapTracker[i].last >= overlapTracker[j].first) {
							if (overlapTracker[i].word.length() < overlapTracker[i].word.length()) {
								isSubstring = true;
								break;
							}
						}
					}
				}
				if (isSubstring == false) {
					returnstring += overlapTracker[i].word;
					returnstring += " ";
				}
			}
		}
		return returnstring;
	}
};
#endif