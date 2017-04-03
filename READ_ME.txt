Instructions for running the files given.

- to compile the files enter g++ -std=c++11 Word_parser.cpp.
- to run the exeecutable enter ./a.out txtFiles/words.english.txt txtFiles/info.zone

*****************************************************************************************
Program Summary:
	I coded this string parser using a hashtable to search for each word that was in the file. First I take in the 
dictionary file and populate the hashtable using a hashing function that uses prime numbers and modulo arithmetic to find the 
hashing index. I use linear stepping to account for any collisions. I figured that the time that was wasted populating the hashtable would 
be gained back by the O(1) lookup properties for the hashtable, and since we had such a big data set to go through it would be worth it. 
The hashtable I coded in the file called HashTable, surprisingly enough.
	
After populating the hashtable I use the file_IO object to read in data from the zone file. it reads the data in and then parses out all the 
characters that arent letters. This way I figured I would have less to search and would make the search easier overall. I then start parsing out the strings line by line 
and comparing them to words in the dictionary. Also I wasnt sure exactly how you wanted me to do this but for example, a word like AFILIAS, which was in the zone.info file,
is not a word in the dictionary but FILIA which is a substring of AFILIA is. So I decided that if a large continuous string of letters isnt a word I also check its substrings
and if I get a match on any of them I choose them. If there is overlap on the substrings that are words, I choose the  biggest one.

I have provided a text file called output.txt that my program prints its output to. I already ran my program all the way through and printed the output to this file
for your reference. also the program prints the seconds / 10000 domains parsed to the console.

thanks 
	Alex Gimson.