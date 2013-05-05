#include "MyDate.h"

using namespace std;

Date::Date() {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  this->day = tm->tm_mday;
  this->month = tm->tm_mon + 1;
  this->year = tm->tm_year + 1900;
}

Date::Date(unsigned int d, unsigned int m, unsigned int y) {
	this->day = d;
	this->month = m;
	this->year = y;
}

Date::Date(time_t tt) {
  if (tt) {
    struct tm *tm = localtime(&tt);
    this->day = tm->tm_mday;
    this->month = tm->tm_mon + 1;
    this->year = tm->tm_year + 1900;
  }
  else {
    this->day = 0;
    this ->month = 0;
    this -> year = 0;
  }
}

Date::~Date() {
}

unsigned int Date::getDay() const {
  return Date::day;
}

unsigned int Date::getMonth() const {
  return Date::month;
}

unsigned int Date::getYear() const {
  return Date::year;
}
