#ifndef __MY_DATE__
#define __MY_DATE__

#include <ctime>
#include <sstream>
#include <string>

#include "Utils.h"

using namespace std;

class Date {
 private:
  unsigned int day;
  string month;
  unsigned int year;
  Utils u;

 public :
  Date();
  Date(unsigned int d, string m, unsigned int y);
  Date(time_t tt);
  ~Date();

  unsigned int getDay() const;
  string getMonth() const;
  unsigned int getYear() const;
  
  bool IsValidDate() const;
};

#endif
