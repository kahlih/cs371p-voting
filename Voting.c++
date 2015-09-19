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
#include <algorithm>
#include <iterator>
#include "Voting.h"

using namespace std;

vector<candidate> candidates_running;
vector<candidate> candidates_loosers;
vector<int> running_ids;
int num_of_ballots;

bool debug = false;
bool trace = true;
bool debug_parse_input = false;
bool debug_pre_eval = false;
bool debug_go = false;
bool debug_check_running = false;
bool debug_eval = false;

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

// analyze the first column
inline void pre_eval(){
	if (debug && trace){
		cout << "Entering Pre_Eval()" << endl;
	}

	if (debug && debug_pre_eval){
		cout << "1" << endl;
	}

	size_t mn = num_of_ballots;
	size_t mx = 0;
	for (candidate c : candidates_running){
		mn = min(c.ballots.size(),mn);
		mx = max(c.ballots.size(), mx);
	}

	if (debug && debug_pre_eval){
		cout << "2" << endl;
		cout << "max is : " << mx << endl;
		print_state_running(); 
	}

	// Moving candidates with tied for last into the looser pool
	// Max sure they are not all winners (checking with mx!=mn)
	for(size_t i = 0; i <candidates_running.size() && (mx!=mn); i++) {

		if (debug && debug_pre_eval){
			cout << "i is : " << i << endl;
		}

		candidate c = candidates_running[i];
		if (c.ballots.size() == mn){
			candidates_loosers.push_back(c);
			candidates_running.erase(candidates_running.begin()+i);
		}
		else {
			running_ids.push_back(c.id);
		}
	}

	if (debug && trace){
		cout << "Leaving Pre_Eval()" << endl;	
	}
}

bool checkRunning(){
	if (debug && trace){
		cout << "Entering checkRunning()" << endl;
	}

	if(debug && debug_check_running) {
		cout << "state at beginning of checkRunning" << endl;
		print_state_running();
		print_state_loosing();
	}

	size_t mn = num_of_ballots;
	size_t mx = 0;

	// Check if there is a Winner > %50 votes
	for (candidate c : candidates_running) {
		if ((double) c.ballots.size() / num_of_ballots > .5) {
			cout << c.name << endl;

			if(debug && debug_check_running) {
				cout << " found our winner: " << c.name << endl;
				cout << " with " << (double)c.ballots.size() / num_of_ballots << " of the vote" << endl;
			}
			return true;
		}
		mn = min(mn, c.ballots.size());
		mx = max(mx, c.ballots.size());
	}
	// check for tie
	if (mn == mx && candidates_loosers.size() == 0) {
		
		if(debug && debug_check_running) {
			cout << "found our winners: " << endl;
		}

		for (candidate c : candidates_running){
			cout << c.name << endl;
			if(debug && debug_check_running) {
				cout << " with " << (double)c.ballots.size() / num_of_ballots << " of the vote" << endl;
			}
		}

		return true;
	}

	// otherwise move bottom tier to losers
	for(int i = 0; i < (int) candidates_running.size() && candidates_loosers.size()==0; i++) {
		candidate c = candidates_running[i];
		if(c.ballots.size() == mn) {
			// delete from running, add to losers
			candidates_running.erase(candidates_running.begin()+i,candidates_running.begin()+i+1);
			candidates_loosers.push_back(c);
		}
	}
	// cout << "\n\n\nstate after some stuff" << endl;
	// 	print_state_running();
	// 	print_state_loosing();

	if (debug && trace){
		cout << "Leaving checkRunning()" << endl;
	}
	return false;
}

// evaluate from the second column onward
// check if we found a winner at the beginning
// only consider those in the losers pool
void eval() {

	if(debug && trace) {
		cout << "Entering eval()" << endl;
	}

	if(debug && debug_eval) {
		cout << "\n\n\n\n\n\n\n\nlooking for a winner" << endl;
	}

	if (checkRunning()) {
		return;
	}

	for (int i = 0; i < (int)candidates_loosers.size(); i++) {
		
		if(debug && debug_eval)
			cout << "i: " << i << endl;

		candidate* looser = &candidates_loosers[i];
		for (deque<int> b : looser->ballots){

			int value = b.front();
			while(!b.empty()) {

				// can find it
				if(end(running_ids) != find(begin(running_ids), end(running_ids), value)) {
					break;
				}
				value = b.front();
				b.pop_front();
			}

			for(size_t runner_index = 0; runner_index < candidates_running.size(); runner_index++) {

				candidate &runner = candidates_running[runner_index];

				// looking for a vote for a winner
				if (runner.id == value) {
					
					if(debug && debug_eval) {
						cout << "\n\nfound a new vote" << endl;
						cout << "ballot size before pushing: " << runner.ballots.size() << endl;
					}

					runner.ballots.push_back(b);
					
					if(debug && debug_eval) {
						cout << "ballot size after pushing: " << runner.ballots.size() << endl;
						print_state_running();
						print_state_loosing();
					}
					break;
				}
			}
		}
		candidates_loosers.erase(candidates_loosers.begin()+i);
	}

	eval();

	if(debug && trace) {
		cout << "Leaving eval()" << endl;
	}

}

// ------------
// parse_input
// ------------

void parse_input(istream &input) {

	if(debug && trace) {
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
	if(debug && trace) {
		cout << "Leaving parse_input()" << endl;
	}
}


void go(istream &input, ostream &o) {
	if(debug && trace) {
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
		candidates_loosers.clear();
		if (t != tests-1)
			cout << endl;

	}
	if(debug && trace) {
		cout << "Leaving go()" << endl;
	}
}