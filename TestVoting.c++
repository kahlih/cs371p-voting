
#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // string
#include <utility>  // pair

#include "gtest/gtest.h"

#include "Voting.h"

using namespace std;



/* ************ */
/* pre_eval */
/* ************ */

TEST(Voting_Fixture_pre_eval, pre_eval_1){
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

}



/* ************ */
/* check_running */
/* ************ */

TEST(Voting_Fixture_check_running, check_running_1){
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

}

/* ************ */
/* eval */
/* ************ */

TEST(Voting_Fixture_eval, eval_1){
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

}

/* ************ */
/* parse_input */
/* ************ */

TEST(Voting_Fixture_parse, parse_input_1){
    istringstream r("3\nDave Grohl\nPatty Griffin\nDave Matthews\n1 2 3\n1 2 3\n2 3 1\n");
    vector<candidate> running_candidates;
    parse_input(r,running_candidates);
    string names[3] = {"Dave Grohl", "Patty Griffin", "Dave Matthews"};
    int counter = 0;
    for (candidate c : running_candidates){
        ASSERT_EQ(c.name, names[counter]);
        ASSERT_EQ(c.id, counter+1);
        ++counter;
    }
}

TEST(Voting_Fixture_parse, parse_input_2){
    istringstream r("1\nBob Marley\n1\n1\n1\n");
    vector<candidate> running_candidates;
    deque<int> d(1, 1);
    parse_input(r,running_candidates);
    for (candidate c : running_candidates){
        ASSERT_EQ(c.name, "Bob Marley");
        ASSERT_EQ(c.id, 1);
    }
}

TEST(Voting_Fixture_parse, parse_input_3){
    istringstream r("2\nMad Max\nTim\n1 2\n2 1\n2 1\n");
    vector<candidate> running_candidates;
    string names[2] = {"Mad Max", "Tim"};
    parse_input(r,running_candidates);
    int counter = 0;
    for (candidate c : running_candidates){
        ASSERT_EQ(c.name, names[counter]);
        ASSERT_EQ(c.id, counter+1);
        ++counter;
    }
    // Test Ballots of Specific Candidate
    counter = 2;
    for (deque<int> b : running_candidates[0].ballots){
        for (int i : b){
            ASSERT_EQ(i,counter);
            --counter;
        }
    }
}

TEST(Voting_Fixture_parse, parse_input_4){
    istringstream r("3\nDave Grohl\nPatty Griffin\nDave Matthews\n1 2 3\n1 2 3\n2 3 1\n");
    vector<candidate> running_candidates;
    parse_input(r,running_candidates);

    for (candidate c : running_candidates){
        if (strcmp(c.name.c_str(),"Dave Grohl")==0){
            ASSERT_EQ(c.ballots.size(), 2);
        }
        else if (strcmp(c.name.c_str(),"Patty Griffin") == 0){
            ASSERT_EQ(c.ballots.size(), 1);
        }
    }

}

/* ************ */
/* go */
/* ************ */

/* Multiple Tests */
TEST(Voting_Fixture_go, go_1) {
    istringstream r("3\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2\n\n3\nJon\nJane\nBob\n1 2 3\n2 3 1\n3 2 1\n3 1 2\n2 3 1\n1 2 3\n\n4\nA\nB\nC\nD\n1 2 3 4\n1 2 3 4\n1 2 3 4\n2 1 3 4\n2 1 3 4\n2 1 3 4\n3 1 2 4\n3 1 2 4\n4 2 1 3\n");
    ostringstream w;
    go(r,w);
    ASSERT_EQ("John Doe\n\nJon\nJane\nBob\n\nA\n" , w.str());
}

/* Overall Winner */
TEST(Voting_Fixture_go, go_2) {
    istringstream r("1\n\n3\nDave Grohl\nPatty Griffin\nDave Matthews\n1 2 3\n1 2 3\n2 3 1\n");
    ostringstream w;
    go(r,w);
    ASSERT_EQ("Dave Grohl\n", w.str());
}

/* Tie */
TEST(Voting_Fixture_go, go_3){
    istringstream r("1\n\n2\nBob Marley\nElvis Presley\n1 2\n2 1\n1 2\n2 1\n");
    ostringstream w;
    go(r,w);
    ASSERT_EQ("Bob Marley\nElvis Presley\n", w.str());
}

/* One Runner */
TEST(Voting_Fixture_go, go_4) {
    istringstream r("1\n\n1\nObama\n1\n");
    ostringstream w;
    go(r,w);
    ASSERT_EQ("Obama\n" , w.str());
}
