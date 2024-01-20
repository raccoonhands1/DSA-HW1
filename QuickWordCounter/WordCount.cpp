#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
  ifstream infile("EngHW.txt");

  int count = 0;

  if (!infile.is_open()) {
    cout << "Error opening file" << endl;
    return 1;
  }

  string word;
  while (infile >> word) {
    count++;
  }

  cout << count;

  infile.close();

  return 0;
}
