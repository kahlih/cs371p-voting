// auto test generation
	int numCases = 100;
	cout << numCases << endl << endl;

	for(int i = 0; i < numCases; i++) {

		int numCandidates = (rand() % 20) +1;
		cout << numCandidates << endl;
		vector<string> candidateNames;

		for(int a = 0; a < numCandidates; a++) {
			char c = a + 65;
			stringstream ss;
			string s;
			ss << c;
			ss >> s;
			candidateNames.push_back(s);
		}

		// alternate random name
		// for(int j = 0; j < numCandidates; j++) {

		// 	int nameLength = rand() % 80;
		// 	string currName;
		// 	for(int k = 0; k < nameLength; k++) {
		// 		char c = rand() % 256;
		// 		currName[k] = c;
		// 	}
		// 	candidateNames.push_back(currName);
		// }

		for(string s : candidateNames) {
			cout << s << endl;
		}

		// ballots
		int numBallots = (rand() % 999) + 1;
		for(int j = 0; j < numBallots; j++) {

			// get list of ballots yet to be voted for
			vector<int> currBallots;
			for(int k = 1; k <= numCandidates; k++) {
				currBallots.push_back(k);
			}

			// vote for each candidate once
			while(currBallots.size() > 0) {
				int someVote = (rand() % numCandidates) + 1;

				// make sure someVote hasn't already been used
				if(find(currBallots.begin(), currBallots.end(), someVote) != currBallots.end()) {
					currBallots.erase(find(currBallots.begin(), currBallots.end(), someVote));
					
					if(currBallots.size() != 0)
						cout << someVote <<  " ";
					else 
						cout << someVote;
				}
			}
			cout << endl;
		}
		cout << endl;
	}
	cout << endl;