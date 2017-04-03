#include<string>
#include<iostream>
#ifndef HASHTABLE_H
#define HASHTABLE_H

struct HashEntry {
	unsigned long long location;
	unsigned long long key;
	std::string substring;
};

class HashTable {
private:
	const int PRIME = 31;
	unsigned int size;
	struct HashEntry ** ptr;
	unsigned long long count;
	unsigned long long hash_index;
public:
	HashTable(){
		this->hash_index = 0;
		ptr = NULL;
		this->size = 157411;
		this->count = 0;

		this->ptr = new HashEntry *[this->size];

		for(int i = 0; i < size; i++)
			ptr[i] = new HashEntry;
		for (unsigned long long i = 0; i < this->size; i++) {
			ptr[i]->key = NULL;
			ptr[i]->location = NULL;
		}
	}
	unsigned long long parseString(const std::string &s)const {
		//declare a long long because the hash will be BIG
		unsigned long long hash = 0;
		char c;
		for (unsigned int i = 0; i < s.length();i++) {
			char c = toupper(s[i]);
			int ascii = 'a' - c;
			hash = PRIME * hash + ascii;
		}
		return hash;
	}
	unsigned long long checkForValue(const std::string &value) {

	    const unsigned long long tempKey = parseString(value);
		this->hash_index = tempKey;
	    this->hash_index = hash_index % this->size;

		while (ptr[hash_index]->key != NULL && ptr[hash_index]->key != tempKey) 
			hash_index = (hash_index + 1) % size;
		if (ptr[hash_index]->key == NULL)
			return -1;
		else 
			return ptr[hash_index]->key;
	}
	void insertHashKey(const std::string &value) {
		//set up the hash key and index number,
		//at this point they are the same, havent modded
		const unsigned long long temp_key = parseString(value);
		unsigned long long insert_hash = temp_key % this->size;
		//resize hash table if necessary here

		if (this->count > (this->size / 2)) {

			unsigned int newSize = this->size * 3;
			HashEntry ** tempPtr;
			tempPtr = ptr;
			this->ptr = new HashEntry *[newSize];
			for (int i = 0; i < newSize; i++)
				ptr[i] = new HashEntry;

			if (ptr) {
				for (unsigned long long i = 0; i < newSize; i++) {
					ptr[i]->key = NULL;
					ptr[i]->location = NULL;
				}
				for (int i = 0; i < this->size; i++) {
					if (tempPtr[i]->key!= NULL) {
						unsigned long long newHash = tempPtr[i]->key % newSize;

						while (ptr[newHash]->key != NULL)
							newHash = (newHash + 1) % newSize;

						ptr[newHash]->key = tempPtr[i]->key;
						ptr[newHash]->location = newHash;
					}
				}
				deleteTempPtrs(tempPtr);
				this->size = newSize;
			}
		}
		while(ptr[insert_hash]->key != NULL &&  ptr[insert_hash]->key != temp_key) 
			insert_hash = (insert_hash + 1) % this->size;
	
		ptr[insert_hash]->key = temp_key;
		ptr[insert_hash]->location = insert_hash;
		ptr[insert_hash]->substring = value;
		this->count++;
	}
	const int findKey(const std::string &s)const {
		unsigned long long tempKey = parseString(s);
		unsigned long long keyExists = 0;
		unsigned long long search_hash =  tempKey % this->size;
		while (ptr[search_hash]->key != NULL && ptr[search_hash]->key != tempKey) {
			search_hash = (search_hash + 1) % this->size;
		}
		
		if (ptr[search_hash]->key == tempKey)
			return search_hash;
		else
			return false;
	}
	void deleteTempPtrs(HashEntry **p) {
		for (int i = 0; i < size; i++)
			delete p[i];
		delete[] p;
	}
	~HashTable(){
		for (int i = 0; i < size; i++)
			delete ptr[i];
		delete[] ptr;
	}
};
#endif
