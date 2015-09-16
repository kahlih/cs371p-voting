// ----------------------------
// projects/voting/Voting.c++
// Copyright (C) 2015
// Glenn P. Downing
// ----------------------------

// --------
// includes
// --------
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "Voting.h"

using namespace std;

vector<candidate> candidates;


void print_state(){
	for (candidate c : candidates){
		cout << "Name : " << c.name << endl;
		cout << "Count: " << c.count << endl;
		for (deque<string> b : c.ballots){
			for (string s : b){
				cout << s << " ";
			}
			cout << endl;
		}
		cout<<endl;
	}
}


void eval(){

}

// ------------
// parse_input
// ------------

void parse_input(istream &input, ostream &o) {

	string num_tests;
	getline(input, num_tests); // 2

	string dummy;
	getline(input, dummy);

	int tests = atoi(num_tests.c_str());
	// test cases
	for(int t = 0; t < tests; t++) {
		string num_candidates;
		getline(input, num_candidates);

		// create candidates
		for(int i = 0; i < atoi(num_candidates.c_str()); i++) {
			string name;
			getline(input, name);

			candidate curr_cand(name, 0);
			candidates.push_back(curr_cand);
		}

		// read ballots
		string ballot_line;
		while(getline(input,ballot_line) && strcmp(ballot_line.c_str(), "") != 0) {
			deque<string> dq;
			boost::split(dq,ballot_line,boost::is_any_of(" "));
			candidates[atoi(dq.front().c_str())-1].ballots.push_back(dq);
		}

		//print_state();
		candidates.clear();

	}

}


//To Do:
/* 
	Refactor code from parse_input into go()

*/

/*
void go(istream &input, ostream &o) {
	parse_input(input,o);
	eval();
}
*/