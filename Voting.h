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
#include <vector>


using namespace std;


/**
 With each candidate comes a name, an ID, and a set of ballots whose
 size indicates the number of people who voted for him
 */
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