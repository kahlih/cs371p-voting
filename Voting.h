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
		candidate(string pname) {
			name = pname;
		}
		string name;
		deque<deque<string>> ballots;


};

void pre_eval();
void eval();
void parse_input(istream &input);
void go(istream &input, ostream &o);