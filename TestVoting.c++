
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
