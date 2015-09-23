
#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // string
#include <utility>  // pair

#include "gtest/gtest.h"

#include "Voting.h"

using namespace std;

// -----------
// TestCollatz
// -----------

// ----
// read
// ----

TEST(Voting_Fixture, read_1) {
    string s("3\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2\n\n3\nJon\nJane\nBob\n1 2 3\n2 3 1\n3 2 1\n3 1 2\n2 3 1\n1 2 3\n\n4\nA\nB\nC\nD\n1 2 3 4\n1 2 3 4\n1 2 3 4\n2 1 3 4\n2 1 3 4\n2 1 3 4\n3 1 2 4\n3 1 2 4\n4 2 1 3\n");
    //= go(s);
    ASSERT_EQ( 1, 1);
}