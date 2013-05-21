#include "Utils.h"

using namespace std;

string Utils::IntToString(int i) {
  ostringstream oss;
  oss << i;
  return oss.str();
}

int Utils::StringToInt(string s) {
  istringstream iss(s);
  int i;
  iss >> i;
  return i;
}

string Utils::GetExtension(string filename) {
  int i = filename.length();
  while (i >= 0) {
    if (filename[i] != '.') {
      i--;
    }
    else {
      break;
    }
  }
  return filename.substr(i+1, filename.length());
}
