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

int num_of_ballots;

bool debug = false;
bool trace = false;
bool debug_parse_input = false;
bool debug_pre_eval = false;
bool debug_go = false;
bool debug_check_running = true;
bool debug_eval = true;

void print_state_running(vector<candidate> &candidates_running) {
	cout << "candidates_running: " << endl;
	for (candidate c : candidates_running) {
		c.print_candidate();
	}
}
void print_state_loosing(vector<candidate> &candidates_loosers) {
	cout << "candidates_loosers: " << endl;
	for (candidate c : candidates_loosers) {
		c.print_candidate();
	}
}

// analyze the first column
inline void pre_eval(vector<candidate> &candidates_running, vector<candidate> &candidates_loosers, vector<int> &running_ids) {
	if (debug && trace) {
		cout << "Entering Pre_Eval()" << endl;
	}

	size_t mn = num_of_ballots;
	size_t mx = 0;
	for (candidate c : candidates_running) {
		mn = min(c.ballots.size(),mn);
		mx = max(c.ballots.size(), mx);
	}

	if (debug && debug_pre_eval) {
		cout << "State upon Entering: " << endl;
		print_state_running(candidates_running); 
		print_state_loosing(candidates_loosers);
		cout << "\n\nNow entinring for loop: " << endl;
	}

	// Moving candidates with tied for last into the looser pool
	// Max sure they are not all winners (checking with mx!=mn)
	for(size_t i = 0; i <candidates_running.size() && (mx!=mn); i++) {

		candidate c = candidates_running[i];

		if (debug && debug_pre_eval) {
			cout << "Viewing Candidate at " << i << " position: " << endl;
			c.print_candidate();			
		}

		if (c.ballots.size() == mn) {
			if (debug && debug_pre_eval) {
				cout << "Before Removal State: " << endl;
				print_state_running(candidates_running);
				print_state_loosing(candidates_loosers);
				cout << "\n" << endl;
				cout << "Removing candidate at " << i << " position: " << endl;
				c.print_candidate();			
			}
			candidates_loosers.push_back(c);
			candidates_running.erase(candidates_running.begin()+i);
			i--;
			if (debug && debug_pre_eval){
				cout << "After Removal, new state: " << endl;
				print_state_running(candidates_running);
				print_state_loosing(candidates_loosers);			
			}
		}
		else {
			running_ids.push_back(c.id);
		}
	}
	if (debug && debug_pre_eval) {
		print_state_running(candidates_running);
		print_state_loosing(candidates_loosers);
	}
	if (debug && trace){
		cout << "Leaving Pre_Eval()" << endl;	
	}
}

bool check_running(ostream &o, vector<candidate> &candidates_running, vector<candidate> &candidates_loosers, vector<int> &running_ids) {
	if (debug && trace){
		cout << "Entering checkRunning()" << endl;
	}

	if(debug && debug_check_running) {
		cout << "state at beginning of checkRunning" << endl;
		print_state_running(candidates_running);
		print_state_loosing(candidates_loosers);
	}

	size_t mn = num_of_ballots;
	size_t mx = 0;

	// Check if there is a Winner > %50 votes
	for (candidate c : candidates_running) {
		if ((double) c.ballots.size() / num_of_ballots > .5) {
			o << c.name << endl;
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

		for (candidate c : candidates_running) {
			o << c.name << endl;
			if(debug && debug_check_running) {
				cout << " with " << (double)c.ballots.size() / num_of_ballots << " of the vote" << endl;
			}
		}

		return true;
	}

	// otherwise move bottom tier to losers
	if (candidates_loosers.size()==0) {
		for(int i = 0; i < (int) candidates_running.size(); i++) {
			candidate c = candidates_running[i];
			if(c.ballots.size() == mn) {
				// delete from running, add to losers

				candidates_running.erase(candidates_running.begin()+i);
				candidates_loosers.push_back(c);
				running_ids.erase(running_ids.begin()+i);
				i--;
			}
		}
	}

	if (debug && trace) {
		cout << "Leaving checkRunning()" << endl;
	}
	return false;
}

// evaluate from the second column onward
// check if we found a winner at the beginning
// only consider those in the losers pool
void eval(ostream &o, vector<candidate> &candidates_running, vector<candidate> &candidates_loosers, vector<int> &running_ids) {

	if(debug && trace) {
		cout << "Entering eval()" << endl;
	}

	if(debug && debug_eval) {
		cout << "\n\n\n\nlooking for a winner\n\n" << endl;
	}

	if (check_running(o,candidates_running,candidates_loosers,running_ids)) {
		return;
	}

	for (int i = 0; i < (int)candidates_loosers.size(); i++) {
		
		candidate* looser = &candidates_loosers[i];
		for (deque<int> b : looser->ballots){

			int value;
			while(!b.empty()) {
				// can find it
				value = b.front();
				b.pop_front();
				if(end(running_ids) != find(begin(running_ids), end(running_ids), value)) {
					break;
				}
			}
			for(size_t runner_index = 0; runner_index < candidates_running.size(); runner_index++) {

				candidate &runner = candidates_running[runner_index];
				// looking for a vote for a winner
				if (runner.id == value) {
					runner.ballots.push_back(b);
					
					if(debug && debug_eval) {
						cout << "ballot size after pushing: " << runner.ballots.size() << endl;
						print_state_running(candidates_running);
						print_state_loosing(candidates_loosers);
					}
					break;
				}
			}
		}
		candidates_loosers.erase(candidates_loosers.begin()+i);
		i--;
	}

	eval(o, candidates_running, candidates_loosers, running_ids);

	if(debug && trace) {
		cout << "Leaving eval()" << endl;
	}

}

// ------------
// parse_input
// ------------

void parse_input(istream &input, vector<candidate> &candidates_running) {

	if(debug && trace) {
		cout << "Entering parse_input()" << endl;
	}

	// get number of candidates
	string num_candidates_str;
	getline(input, num_candidates_str);
	int num_candidates = atoi(num_candidates_str.c_str());

	// create candidates_running
	for(int i = 0; i < num_candidates; i++) {
		string name;
		getline(input, name);

		candidate curr_cand(name, i+1);
		candidates_running.push_back(curr_cand);
	}
	if (debug && debug_parse_input){
		cout<< "Printing state before inputing ballots" << endl;
		print_state_running(candidates_running);
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
		dq.pop_front();
		candidates_running[index].ballots.push_back(dq);
	}
	if (debug && debug_parse_input) {
		cout<< "Printing state after inputing ballots" << endl;
		print_state_running(candidates_running);
	}
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
		vector<candidate> candidates_running;
		vector<candidate> candidates_loosers;
		vector<int> running_ids;
		parse_input(input,candidates_running);
		pre_eval(candidates_running, candidates_loosers,running_ids);
		eval(o,candidates_running, candidates_loosers, running_ids);
		candidates_running.clear();
		candidates_loosers.clear();
		running_ids.clear();
		if (t != tests-1)
			o << endl;

	}
	if(debug && trace) {
		cout << "Leaving go()" << endl;
	}
}