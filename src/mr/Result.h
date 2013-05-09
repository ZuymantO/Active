#ifndef __Result__
#define __Result__

#include <string>
#include <list>

using namespace std;

class Result {
 private :
  string name;
  string path;
  string perm;
  unsigned int size;
  string lastModif;
  string proprio;

 public :
  Result(string n, string pa, string pe, unsigned int s, string lm, string pr);
  Result(string n, string pa, string pe, unsigned int s);
  ~Result();

  void init(string n, string pa, string pe, unsigned int s, string lm, string pr);

  string getName() const;
  string getPath() const;
  string getPerm() const;
  unsigned int getSize() const;
  string getLastModif() const;
  string getProprio() const;
};

class Results {
 private :
  string id;
  list<Result> res;

 public :
  Results(string i, list<Result> r);
  ~Results();

  string GetID();
  list<Result> GetRes();
};

#endif
