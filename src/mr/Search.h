#ifndef __Search__
#define __Search__

#include "MyDate.h"

#include <string>

using namespace std;

class Search {
 private :
  string word;
  bool content;
  string pathDir;
  string perm;
  string extension;
  Date *begin;
  Date *end;

 public :
  Search(string w, bool c, string pd, string p, string ext, Date* b, Date* e);
  Search(string w, string pd, string p, string ext, Date* b, Date* e);
  ~Search();

  void init(string w, bool c, string pd, string p, string ext, Date* b, Date*e);

  string getWord() const;
  bool getContent() const;
  string getPathDir() const;
  string getPerm() const;
  string getExtension() const;
  unsigned int getBeginDay() const;
  string getBeginMonth() const;
  unsigned int getBeginYear() const;
  unsigned int getEndDay() const;
  string getEndMonth() const;
  unsigned int getEndYear() const;

  bool timeSlot() const;
};

#endif
