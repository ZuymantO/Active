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
