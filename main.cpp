#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char** argv){
    ifstream file;  //https://linuxhint.com/read-text-file-2d-array-cpp/
    file.open(argv[1]);
    vector<vector<double>> data;
    string line;
    while(getline(file, line)){
        string entry;
        stringstream ss(line);
        vector<double> v;
        while(ss>>entry){
            v.push_back(stod(entry));
        }
        data.push_back(v);

    }
    file.close();
    
    return 0;
}