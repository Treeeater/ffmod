#ifndef Yuchen
#define Yuchen

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

namespace yuchen{
	void inline record(string fileName, string resource, string stack, string additional){
		istringstream iss(stack);
		vector<string> tokens{ istream_iterator < string > {iss}, istream_iterator < string > {} };
		string processedStack = "";
		vector<string> domains;
		string domain = "";
		for (auto curCxt : tokens){
			unsigned at = curCxt.find("@");
			if (at == string::npos) continue;		//ignore contexts w/o @ sign
			curCxt = curCxt.substr(at + 1);
			if (curCxt.substr(0, 7) == "http://") {
				domain = curCxt.substr(7);
			}
			else if (curCxt.substr(0, 8) == "https://"){
				domain = curCxt.substr(8);
			}
			else continue;		//ignore non http/https protocols.
			domain = domain.substr(0, domain.find("/"));
			if (find(domains.begin(), domains.end(), domain) == domains.end()){
				domains.push_back(domain);
				processedStack += (domain + "\n");
			}
		}
		if (processedStack == "") return;		//no valid stack information.
		ofstream myfile;
		myfile.open(fileName, std::ios::out | std::ios::app);
		myfile << resource << endl;
		myfile << processedStack;
		myfile << additional << endl << "----" << endl;
		myfile.close();
	};
}

#endif