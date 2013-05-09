
#include "Search.h"

#include <iostream>

Search::Search(string w, bool c, string pd, string p, string ext, Date* b, Date* e) {
  this->init(w, c, pd, p, ext, b, e);
}

Search::Search(string w, string pd, string p, string ext, Date* b, Date* e) {
  this->init(w, false, pd, p, ext, b, e);
}

Search::~Search() {
  delete begin;
  delete end;
}

void Search::init(string w, bool c, string pd, string p, string ext, Date* b, Date* e) {
  this->word = w;
  this->content = c;
  this->pathDir = pd;
  this->perm = p;
  this->extension = ext;
  this->begin = new Date((*b).getDay(), (*b).getMonth(), (*b).getYear());
  this->end = new Date((*e).getDay(), (*e).getMonth(), (*e).getYear());
}

string Search::getWord() const {
  return this->word;
}

bool Search::getContent() const {
  return this->content;
}

string Search::getPathDir() const {
  return this->pathDir;
}

string Search::getPerm() const {
  return this->perm;
}

string Search::getExtension() const {
  return this->extension;
}

unsigned int Search::getBeginDay() const {
  return (*this->begin).getDay();
}

string Search::getBeginMonth() const {
  return (*this->begin).getMonth();
}

unsigned int Search::getBeginYear() const {
  return (*this->begin).getYear();
}

unsigned int Search::getEndDay() const {
  return (*this->end).getDay();
}

string Search::getEndMonth() const {
  return (*this->end).getMonth();
}

unsigned int Search::getEndYear() const {
  return (*this->end).getYear();
}

bool Search::timeSlot() const {
  //return (Date::isValidDate((*this->begin)) && Date::isValidDate((*this->end)));
  return ((*this->begin).IsValidDate() && (*this->end).IsValidDate());
}

/*
int main() {
  Date *d1, *d2;
  d1 = new Date();
  d2 = new Date(4,"5",2013);

  Search *m = new Search("mot", "pathdir", "perm", "ext", d1, d2);
  cout << "taille : " << (*m).getWord().size() << endl;
  cout << "word : " << (*m).getWord() << endl;
  cout << "content : " << (*m).getContent() << endl;
  cout << "pathdir : " << (*m).getPathDir() << endl;
  cout << "perm : " << (*m).getPerm() << endl;
  cout << "extension : " << (*m).getExtension() << endl;
  cout << "date debut : " << (*m).getBeginDay() << "/" << (*m).getBeginMonth() << "/" << (*m).getBeginYear() << endl;
  cout << "date fin : " << (*m).getEndDay() << "/" << (*m).getEndMonth() << "/" << (*m).getEndYear() << endl;

  return 1;
}
*/
