#include "Search.h"

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
  return ((*this->begin).IsValidDate() && (*this->end).IsValidDate());
}
