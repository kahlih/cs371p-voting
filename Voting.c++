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
#include "Voting.h"

#include <algorithm>
#include <iterator>

using namespace std;

vector<candidate> candidates_running;
vector<candidate> candidates_loosers;
vector<int> running_ids;
int num_of_ballots;

bool debug = false;
bool debug_parse_input = false;
bool debug_pre_eval = false;
bool debug_go = false;
bool debug_check_running = true;

void print_state_running(){
	cout << "candidates_running: " << endl;
	for (candidate c : candidates_running){
		c.print_candidate();
	}
}
void print_state_loosing(){
	cout << "candidates_loosers: " << endl;
	for (candidate c : candidates_loosers){
		c.print_candidate();
	}
}

// basically just a move analyzing the first column
// TODO: pre_eval will find the maximum
// TODO: pre_eval will see who is not the max
// TODO: pre_eval will move those people to losers and remove from running
// above could use get() but that is O(N)
// may need to find another solution for finding/moving
// will be using size of deque as their count
// don't check if we found a winner

// solution to above
// keep a counter of where we are in moving through the candidates and remove at that index
inline void pre_eval(){
	if (debug && debug_pre_eval){
		cout << "Entering Pre_Eval()" << endl;
	}

	if (debug && debug_pre_eval){
		cout << "1" << endl;
	}

	int m = num_of_ballots;
	for (candidate c : candidates_running){
		m = min((int) c.ballots.size(),m);
	}

	if (debug && debug_pre_eval){
		cout << "2" << endl;
		cout << "max is : " << m << endl;
		print_state_running(); 
	}

	for(int i = 0; i < (int) candidates_running.size(); i++) {

		if (debug && debug_pre_eval){
			cout << "i is : " << i << endl;
		}

		candidate c = candidates_running[i];
		if ((int) c.ballots.size() == m){
			candidates_loosers.push_back(c);
			candidates_running.erase(candidates_running.begin()+i,candidates_running.begin()+i+1);
		}
		else {
			running_ids.push_back(c.id);
		}
	}
	if (debug && debug_pre_eval){
		print_state_running();
		cout << "Leaving Pre_Eval()" << endl;	
	}
}

bool checkRunning(){
	if (debug && debug_check_running){
		cout << "Entering checkRunning()" << endl;
	}
	//Check if there is a Winner > %50 votes
	for (candidate c : candidates_running){
		if ((double)c.ballots.size()/num_of_ballots > .5){
			cout << c.name << endl;
			return true;
		}
	}

	if (candidates_loosers.size() == 0){
		for (candidate c : candidates_running){
			cout << c.name << endl;
		}
		return true;
	}
	if (debug && debug_check_running){
		cout << "Leaving checkRunning()" << endl;
	}
	return false;
}

// evaluate from the second column onward
// check if we found a winner at the beginning
// only consider those in the losers pool
void eval() {

	if (checkRunning()){
		return;
	}
	for (int i = 0; i < (int)candidates_loosers.size(); i++){
		candidate* looser = &candidates_loosers[i];
		for (deque<int> b : looser->ballots){
			int i = b.front();
			b.pop_front();

			while(!b.empty() && (end(running_ids) != find(begin(running_ids), end(running_ids), i))){
				i = b.front();
				b.pop_front();
			}
			for (candidate runner : candidates_running){
				if (runner.id == i){
					runner.ballots.push_back(b);
				}
				candidates_loosers.erase(candidates_loosers.begin()+i);
			}
		}
	}

}

// ------------
// parse_input
// ------------

void parse_input(istream &input) {

	if(debug && debug_parse_input) {
		cout << "Entering parse_input()" << endl;
	}

	// get number of candidates
	string num_candidates_str;
	getline(input, num_candidates_str);
	int num_candidates = atoi(num_candidates_str.c_str());

	// allocate storage for candidates
	//candidates_running.resize(num_candidates);
	//candidates_loosers.resize(num_candidates);

	// create candidates_running
	for(int i = 0; i < num_candidates; i++) {
		string name;
		getline(input, name);

		candidate curr_cand(name, i+1);
		candidates_running.push_back(curr_cand);
	}
	if (debug && debug_parse_input){
		cout<< "Printing state before inputing ballots" << endl;
		//print_state();
	}
	// read ballots
	string ballot_line;
	while(getline(input,ballot_line) && strcmp(ballot_line.c_str(), "") != 0) {
		deque<int> dq;
		++num_of_ballots;
		string b;
		stringstream stream(ballot_line);
		while (getline(stream, b, ' '))
			dq.push_back(atoi(b.c_str()));

		int index = dq.front()-1;
		/*if (debug && debug_parse_input){
			cout<< "Indexing at : " << index << endl;
			print_state();
		}*/
		dq.pop_front();
		candidates_running[index].ballots.push_back(dq);
	}
	if (debug && debug_parse_input){
		cout<< "Printing state after inputing ballots" << endl;
		//print_state();
	}
	//print_state();
	if(debug && debug_parse_input) {
		cout << "Leaving parse_input()" << endl;
	}
}


void go(istream &input, ostream &o) {
	if(debug && debug_go) {
		cout << "Entering go" << endl;
	}
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
		//print_state_running();
		//print_state_loosing();
		candidates_running.clear();

	}
	if(debug && debug_go) {
		cout << "Leaving go()" << endl;
	}
}