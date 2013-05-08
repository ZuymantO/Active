#ifndef __Result__
#define __Result__

#include <list>
#include <string>

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

  /*
  void setName(string n);
  void setPath(string p);
  void setPerm(string p);
  void setSize(unsigned int s);
  void setLastModif(string lm);
  void setProprio(string p);
  */

  list<Result> SearchResults(string xmlStream);
};


#endif
