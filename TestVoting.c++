
#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // string
#include <utility>  // pair

#include "gtest/gtest.h"

#include "Voting.h"

using namespace std;


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


/* ************ */
/* pre_eval */
/* ************ */


/* Tie */
TEST(Voting_Fixture_pre_eval, pre_eval_1){
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

    deque<int> d1;
    candidate c1("Han Solo",1);
    c1.ballots.push_back(d1);
    candidates_running.push_back(c1);
    candidate c2("Darth Vader",2);
    c2.ballots.push_back(d1);
    candidates_running.push_back(c2);
    candidate c3("Yoda",3);
    c3.ballots.push_back(d1);
    candidates_running.push_back(c3);

    pre_eval(candidates_running,candidates_loosers,running_ids);

    ASSERT_EQ(candidates_running.size(),3);
    ASSERT_EQ(candidates_loosers.size(),0);
}

/* One Winner */
TEST(Voting_Fixture_pre_eval, pre_eval_2){
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

    deque<int> d1;
    candidate c1("Han Solo",1);
    c1.ballots.push_back(d1);
    c1.ballots.push_back(d1);
    candidates_running.push_back(c1);
    candidate c2("Darth Vader",2);
    c2.ballots.push_back(d1);
    candidates_running.push_back(c2);
    candidate c3("Yoda",3);
    c3.ballots.push_back(d1);
    candidates_running.push_back(c3);

    pre_eval(candidates_running,candidates_loosers,running_ids);

    ASSERT_EQ(candidates_running.size(),1);
    ASSERT_EQ(candidates_loosers.size(),2);
}

/* Two Candidates Tied for First */
TEST(Voting_Fixture_pre_eval, pre_eval_3){
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

    deque<int> d1;
    candidate c1("Han Solo",1);
    c1.ballots.push_back(d1);
    c1.ballots.push_back(d1);
    candidates_running.push_back(c1);
    candidate c2("Darth Vader",2);
    c2.ballots.push_back(d1);
    c2.ballots.push_back(d1);
    candidates_running.push_back(c2);
    candidate c3("Yoda",3);
    c3.ballots.push_back(d1);
    candidates_running.push_back(c3);

    pre_eval(candidates_running,candidates_loosers,running_ids);

    ASSERT_EQ(candidates_running.size(),2);
    ASSERT_EQ(candidates_loosers.size(),1);
}



/* ************ */
/* check_running */
/* ************ */

/* Overall Winner */
TEST(Voting_Fixture_check_running, check_running_1){
    ostringstream w;
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

    deque<int> d1;
    candidate c("Han Solo",1);
    c.ballots.push_back(d1);
    c.ballots.push_back(d1);
    candidates_running.push_back(c);

    check_running(w,candidates_running,candidates_loosers,running_ids);
    ASSERT_EQ(w.str(),"Han Solo\n");
}


/* Tie */
TEST(Voting_Fixture_check_running, check_running_2){
    ostringstream w;
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

    deque<int> d1;
    candidate c1("Han Solo",1);
    c1.ballots.push_back(d1);
    candidates_running.push_back(c1);
    candidate c2("Darth Vader",2);
    c2.ballots.push_back(d1);
    candidates_running.push_back(c2);
    candidate c3("Yoda",3);
    c3.ballots.push_back(d1);
    candidates_running.push_back(c3);

    check_running(w,candidates_running,candidates_loosers,running_ids);
    ASSERT_EQ(w.str(),"Han Solo\nDarth Vader\nYoda\n");
}

/* ************ */
/* eval */
/* ************ */

TEST(Voting_Fixture_eval, eval_1){
    ostringstream w;
    vector<candidate> candidates_running;
    vector<candidate> candidates_loosers;
    vector<int> running_ids;

}
