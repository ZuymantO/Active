#ifndef __UTILS__
#define __UTILS__

#include <string>
#include <sstream>

using namespace std;

class Utils {
 public :
  static string IntToString(int i);
  static int StringToInt(string s);
  static string GetExtension(string filename);
};

#endif
