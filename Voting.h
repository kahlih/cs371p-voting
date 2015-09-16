// --------------------------
// projects/voting/Voting.h
// Copyright (C) 2015
// Glenn P. Downing
// --------------------------

#ifndef Voting_h
#define Voting_h
#endif

// --------
// includes
// --------
#include <deque>
#include <string>


using namespace std;

class candidate {

	public:
		candidate(string pname, long long pcount) {
			name = pname;
			count = pcount;
		}
		string name;
		long long count;
		deque<deque<string>> ballots;


};


void parse_input(istream &input, ostream &o);