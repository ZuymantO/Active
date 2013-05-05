#ifndef __MR__
#define __MR__

#include "MyDate.h"

#include <string>
#include <time.h>

using namespace std;

class MR {
 private :
  string word;
  bool content;
  string pathDir;
  string perm;
  string extension;
  Date *begin;
  Date *end;

 public :
  MR(string w, bool c, string pd, string p, string ext, Date* b, Date* e);
  MR(string w, string pd, string p, string ext, Date* b, Date* e);
  ~MR();

  void init(string w, bool c, string pd, string p, string ext, Date* b, Date*e);

  string getWord() const;
  bool getContent() const;
  string getPathDir() const;
  string getPerm() const;
  string getExtension() const;
  unsigned int getBeginDay() const;
  unsigned int getBeginMonth() const;
  unsigned int getBeginYear() const;
  unsigned int getEndDay() const;
  unsigned int getEndMonth() const;
  unsigned int getEndYear() const;

  bool isValidDate(Date d) const;
  bool timeSlot() const;
};

#endif
