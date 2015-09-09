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
#include <deque>

using namespace std;


class candidate {

	public:
		candidate(string pname, int pid) {
			name = pname;
			id = pid;
		}
		//candidate() {}
		string name;
		int id ;
		deque<deque<int>> ballots;

		candidate operator=(candidate& rhs) {
			
			this->name = rhs.name;
			this->ballots = rhs.ballots;

			return *this;
		}

		const candidate& operator=(const candidate &c) {
			this->name = c.name;
			this->ballots = c.ballots;
			this->id = c.id;
			return *this;
		}

		void print_candidate() {
			cout << this->name << endl;
			cout << "ID: " << this->id << endl;
			cout << "Ballots: " <<endl;
			for (deque<int> b : this->ballots) {
				for (int s : b) {
					cout << s << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
};

void print_state_loosing(vector<candidate> &candidates_loosing);
void print_state_running(vector<candidate> &candidates_running);
void pre_eval(vector<candidate> &candidates_running, vector<candidate> &candidates_loosing, vector<int> &running_ids);
bool check_running(ostream &o, vector<candidate> &candidates_running, vector<candidate> &candidates_loosing, vector<int> &running_ids);
void eval(ostream &o, vector<candidate> &candidates_running, vector<candidate> &candidates_loosing, vector<int> &running_ids);
void parse_input(istream &input, vector<candidate> &candidates_running);
void go(istream &input, ostream &o);

int num_of_ballots;

// analyze the first column
inline void pre_eval(vector<candidate> &candidates_running, vector<candidate> &candidates_loosers, vector<int> &running_ids) {

	//Testing purposes
	if (num_of_ballots == 0){
		num_of_ballots = 3;
	}
	size_t mn = num_of_ballots;
	size_t mx = 0;
	for (candidate c : candidates_running) {
		mn = min(c.ballots.size(),mn);
		mx = max(c.ballots.size(), mx);
	}

	// Moving candidates with tied for last into the looser pool
	// Max sure they are not all winners (checking with mx!=mn)
	for(size_t i = 0; i <candidates_running.size() && (mx!=mn); i++) {

		candidate c = candidates_running[i];
		if (c.ballots.size() == mn) {
			candidates_loosers.push_back(c);
			candidates_running.erase(candidates_running.begin()+i);
			i--;
		}
		else running_ids.push_back(c.id);
	}
}

bool check_running(ostream &o, vector<candidate> &candidates_running, vector<candidate> &candidates_loosers, vector<int> &running_ids) {
	size_t mn = num_of_ballots;
	size_t mx = 0;

	// Testing Purposes
	if (num_of_ballots==0){
		num_of_ballots = 3;
	}

	// Check if there is a Winner > %50 votes
	for (candidate c : candidates_running) {
		if ((double) c.ballots.size() / num_of_ballots > .5) {
			o << c.name << endl;
			return true;
		}
		mn = min(mn, c.ballots.size());
		mx = max(mx, c.ballots.size());
	}
	// check for tie
	if (mn == mx && candidates_loosers.size() == 0) {

		for (candidate c : candidates_running) {
			o << c.name << endl;
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

	return false;
}

// evaluate from the second column onward
// check if we found a winner at the beginning
// only consider those in the losers pool
void eval(ostream &o, vector<candidate> &candidates_running, vector<candidate> &candidates_loosers, vector<int> &running_ids) {

	// Testing purposes
	if (num_of_ballots==0){
		num_of_ballots=5;
	}
	if (check_running(o,candidates_running,candidates_loosers,running_ids)) {
		return;
	}

	for (int i = 0; i < (int)candidates_loosers.size(); i++) {
		
		candidate* looser = &candidates_loosers[i];
		for (deque<int> b : looser->ballots) {

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
					
					break;
				}
			}
		}
		candidates_loosers.erase(candidates_loosers.begin()+i);
		i--;
	}

	eval(o, candidates_running, candidates_loosers, running_ids);

}

// ------------
// parse_input
// ------------

void parse_input(istream &input, vector<candidate> &candidates_running) {


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
}

void go(istream &input, ostream &o) {

	string num_tests;
	getline(input, num_tests); // 2

	string dummy;
	getline(input, dummy);

	int tests = atoi(num_tests.c_str());
	// loop for each test
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
}

int main(){
	go(cin,cout);
	return 0;
}