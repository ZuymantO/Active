#ifndef __MY_DATE__
#define __MY_DATE__

#include <ctime>

class Date {
 private:
  unsigned int day;
  unsigned int month;
  unsigned int year;

 public :
  Date();
  Date(unsigned int d, unsigned int m, unsigned int y);
  Date(time_t tt);
  ~Date();

  unsigned int getDay() const;
  unsigned int getMonth() const;
  unsigned int getYear() const;
};

#endif
