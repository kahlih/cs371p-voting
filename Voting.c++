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
#include <iostream>
#include <cstring>
#include <sstream>
//#include <boost/algorithm/string.happ>
#include "Voting.h"

using namespace std;

vector<candidate> candidates_running;
vector<candidate> candidate_loosers;


void print_state(){
	for (candidate c : candidates_running){
		cout << "Name : " << c.name << endl;
		for (deque<string> b : c.ballots){
			for (string s : b){
				cout << s << " ";
			}
			cout << endl;
		}
		cout<<endl;
	}
}

inline void pre_eval(){
	int m = 0;
	for (candidate c : candidates_running){
		m = max(c.ballots.size(),m);
	}
	for (candidate c : candidates_running){
		if (c.ballots.size() != m){
			candidate_loosers.push_back(c);
			candidates_running.pop_front
		}
	}
}

void eval(){


}

// ------------
// parse_input
// ------------

void parse_input(istream &input) {

	string num_candidates;
	getline(input, num_candidates);

	// create candidates_running
	for(int i = 0; i < atoi(num_candidates.c_str()); i++) {
		string name;
		getline(input, name);

		candidate curr_cand(name);
		candidates_running.push_back(curr_cand);
	}

	// read ballots
	string ballot_line;
	while(getline(input,ballot_line) && strcmp(ballot_line.c_str(), "") != 0) {
		deque<string> dq;

		string b;
		stringstream stream(ballot_line);
		while (getline(stream, b, ' ')){
			dq.push_back(b);
		}
		int index = atoi(dq.front().c_str())-1;
		dq.pop_front();
		candidates_running[index].ballots.push_back(dq);
	}

	//print_state();

}


void go(istream &input, ostream &o) {
	string num_tests;
	getline(input, num_tests); // 2

	string dummy;
	getline(input, dummy);

	int tests = atoi(num_tests.c_str());
	// test cases
	for(int t = 0; t < tests; t++) {
		parse_input(input);
		pre_eval();
		eval();
		candidates_running.clear();

	}
}