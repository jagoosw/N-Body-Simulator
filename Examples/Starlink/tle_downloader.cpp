#include <iostream>
#include <fstream>
using namespace std;
int main(){
	string url_base = "https://celestrak.com/satcat/tle.php?CATNR=";
	string outfile_base = "/Users/jago/Documents/Uni/Other/CV/N-Body-Simulator/starlink_data/";

	std::ifstream file("starlink_satcat_ids.txt");
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			string command = "wget -O "+outfile_base+line+" '"+url_base+line+"'";
			const char *command_i = command.c_str(); 
			system(command_i);
		}
		file.close();
	}
}