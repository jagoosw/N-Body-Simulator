#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <typeinfo>
//Borrowed tle to vector algorithm from https://github.com/dh4gan/orbital-calculations/ but converted from python to C++
using namespace std;

vector <long double> rotate_z(vector <long double> origional, long double angle){
	vector <long double> final;
	for (int i = 1; i <= 3; i++) 
		final.push_back(0); 
	final.at(0) = origional[0]*cos(angle)-origional[1]*sin(angle);
	final.at(1) = origional[0]*sin(angle)+origional[1]*cos(angle);
	final.at(2) = origional[2];
	return final;
} 

vector <long double> rotate_x(vector <long double> origional, long double angle){
	vector <long double> final;
	for (int i = 1; i <= 3; i++) 
		final.push_back(0); 
	final.at(0) = origional[0];
	final.at(1) = origional[1]*cos(angle)-origional[2]*sin(angle);
	final.at(2) = origional[1]*sin(angle)+origional[2]*cos(angle);
	return final;
} 

//https://www.tutorialspoint.com/how-to-print-out-the-contents-of-a-vector-in-cplusplus
void print(vector <long double> const &a) {
   cout << "The vector elements are : ";
   
   for(int i=0; i < a.size(); i++)
	  cout << a.at(i) << ' ';
}

int main(){
	string infile_base = "/Users/jago/Documents/Uni/Other/CV/N-Body-Simulator/starlink_data/";
	float gm = 3.986004418e14;
	float pi = 3.1415926536;
	ifstream name_file("starlink_satcat_ids.txt");
	string write_file;
	write_file += "ID,magP,magV,r_x,r_y,r_z,v_x,v_y,v_z";
	if (name_file.is_open()) {
		string line;
		while (getline(name_file, line)) {
			ifstream tle_file(infile_base+line);
			string tle_line;
			int line_no=0;
			string tle_line_2;
			while (getline(tle_file, tle_line))
			{
				//cout<<"data:\n"+tle_line+"\n*** no:"+to_string(line_no)+"\n";
				if(line_no == 15){
					tle_line_2 = tle_line;
				}
				line_no++;
			}
			float mean_motion = ::atof(tle_line_2.substr(52,11).c_str());

			float t = 86400/mean_motion;
			float a = pow((gm*t*t/(4*pi*pi)),0.3333333);

			//They use two different conventions for "decimal point assumed" in the same files, grr
			float e = ::atof(("."+tle_line_2.substr(26,6)).c_str());
			float m = ::atof(tle_line_2.substr(43,7).c_str());

			//taylor series from https://en.wikipedia.org/wiki/Mean_anomaly
			float v_ = m + (2*e-0.25*e*e*e)*sin(m)+1.25*e*e*sin(2*m)+(13/12)*e*e*e*sin(3*m);

			float w_ = ::atof(tle_line_2.substr(34,7).c_str());
			float o_ = ::atof(tle_line_2.substr(17,7).c_str());
			float i = ::atof(tle_line_2.substr(8,7).c_str());

			float p = a*(1-e*e);
			float modR = p/(1-e*cos(v_));
			float modV;
			if(p!=0){
				modV = pow(gm/p,0.5);
			}
			else{
				modV = pow(2*gm/modR,0.5);
			}

			vector<long double> position;
			//This is a messy way to define the vectors but others weren't working
			position.push_back(modR*cos(v_));
			position.push_back(modR*sin(v_));
			position.push_back(0);
			vector<long double> velocity;
			velocity.push_back(-modV*sin(v_));
			velocity.push_back(-modV*(cos(v_)+e));
			velocity.push_back(0);

			position = rotate_z(rotate_x(rotate_z(position,w_),i),o_);
			velocity = rotate_z(rotate_x(rotate_z(velocity,w_),i),o_);
			write_file+="\n"+line+","+to_string(modR)+","+to_string(modV)+","+to_string(position[0])+","+to_string(position[1])+","+to_string(position[2])+","+to_string(velocity[0])+","+to_string(velocity[1])+","+to_string(velocity[2]);

			tle_file.close();
		}
		name_file.close();
		ofstream out_file;
		out_file.open("starlink_state_vectors.csv");
		out_file<<write_file;
		out_file.close();
	}
}