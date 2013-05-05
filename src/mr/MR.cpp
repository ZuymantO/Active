#include "MR.h"

#include <iostream>

MR::MR(string w, bool c, string pd, string p, string ext, Date* b, Date* e) {
  this->init(w, c, pd, p, ext, b, e);
}

MR::MR(string w, string pd, string p, string ext, Date* b, Date* e) {
  this->init(w, false, pd, p, ext, b, e);
}

MR::~MR() {
  delete begin;
  delete end;
}

void MR::init(string w, bool c, string pd, string p, string ext, Date* b, Date* e) {
  this->word = w;
  this->content = c;
  this->pathDir = pd;
  this->perm = p;
  this->extension = ext;
  cout << "content " << c << endl;
  this->begin = new Date((*b).getDay(), (*b).getMonth(), (*b).getYear());
  this->end = new Date((*e).getDay(), (*e).getMonth(), (*e).getYear());
}

string MR::getWord() const {
  return this->word;
}

bool MR::getContent() const {
  return this->content;
}

string MR::getPathDir() const {
  return this->pathDir;
}

string MR::getPerm() const {
  return this->perm;
}

string MR::getExtension() const {
  return this->extension;
}

unsigned int MR::getBeginDay() const {
  return (*this->begin).getDay();
}

unsigned int MR::getBeginMonth() const {
  return (*this->begin).getMonth();
}

unsigned int MR::getBeginYear() const {
  return (*this->begin).getYear();
}

unsigned int MR::getEndDay() const {
  return (*this->end).getDay();
}

unsigned int MR::getEndMonth() const {
  return (*this->end).getMonth();
}

unsigned int MR::getEndYear() const {
  return (*this->end).getYear();
}

bool MR::isValidDate(Date d) const {
  return (d.getDay() != 0 && d.getMonth() != 0 && d.getYear() != 0) ? true : false;
}

bool MR::timeSlot() const {
  return (MR::isValidDate((*this->begin)) && MR::isValidDate((*this->end)));
}

int main() {
  Date *d1, *d2;
  d1 = new Date();
  d2 = new Date(4,5,2013);

  MR *m = new MR("mot", "pathdir", "perm", "ext", d1, d2);
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
