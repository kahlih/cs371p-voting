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
		candidate() {}
		string name;
		deque<deque<string>> ballots;

		candidate operator=(candidate& rhs) {
			
			this->name = rhs.name;
			this->ballots = rhs.ballots;

			return *this;
		}

};

void pre_eval();
void eval();
void parse_input(istream &input);
void go(istream &input, ostream &o);