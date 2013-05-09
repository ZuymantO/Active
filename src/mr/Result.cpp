#include "Result.h"

Result::Result(string n, string pa, string pe, unsigned int s, string lm, string pr) {
  this->init(n, pa, pe, s, lm, pr);
}

Result::Result(string n, string pa, string pe, unsigned int s) {
  this->init(n, pa, pe, s, NULL, NULL);
}

Result::~Result() {}

void Result::init(string n, string pa, string pe, unsigned int s, string lm, string pr) {
  this->name = n;
  this->path = pa;
  this-> perm = pe;
  this->size = s;
  this->lastModif = lm;
  this->proprio = pr;
}

string Result::getName() const {
  return this->name;
}

string Result::getPath() const {
  return this->path;
}

string Result::getPerm() const {
  return this->perm;
}

unsigned int Result::getSize() const {
  return this->size;
}

string Result::getLastModif() const {
  return this->lastModif;
}

string Result::getProprio() const {
  return this->proprio;
}
