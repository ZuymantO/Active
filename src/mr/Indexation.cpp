#include "Indexation.h"

using namespace std;

// ----------------------------------------------------------------------------
// Class Indexage
// ----------------------------------------------------------------------------

Indexage::Indexage(string s, int o) {
  this->word = s;
  this->occurrence = o;
}

Indexage::~Indexage() {}

string Indexage::GetWord() const {
  return this->word;
}

int Indexage::GetOccurrence() const {
  return this->occurrence;
}

void Indexage::SetOccurrence(int i) {
  this->occurrence = i;
}

// ----------------------------------------------------------------------------
// Class Renommage
// ----------------------------------------------------------------------------

Renommage::Renommage(string o, string n) {
  this->oldpath = o;
  this->newpath = n;
}

Renommage::~Renommage() {}

string Renommage::GetOldPath() const {
  return this->oldpath;
}

string Renommage::GetNewPath() const {
  return this->newpath;
}

// ----------------------------------------------------------------------------
// Class File
// ----------------------------------------------------------------------------

File::File(string pa, string d, unsigned int t, string pr, string g, string p, list<Indexage> i) {
  this->init(pa, d, t, pr, g, p, i);
}

File::File(string pa, string d, unsigned int t, string pr, string g, string p, Indexage i) {
  list<Indexage> tmp (1,i);
  this->init(pa, d, t, pr, g, p, tmp);
}

File::~File() {}

void File::init(string pa, string d, unsigned int t, string pr, string g, string p, list<Indexage> i) {
  this->path = pa;
  this->date = d;
  this->taille = t;
  this->proprietaire = pr;
  this->groupe = g;
  this->perm = p;
  this->index = i;
}

string File::GetPath() const{
  return this->path;
}

string File::GetDate() const {
  return this->date;
}

unsigned int File::GetTaille() const {
  return this->taille;
}

string File::GetProprietaire() const {
  return this->proprietaire;
}

string File::GetGroupe() const {
  return this->groupe;
}

string File::GetPerm() const {
  return this->perm;
}

list<Indexage> File::GetIndex() const {
  return this->index;
}

void File::AddIndex(Indexage i) {
  this->index.push_front(i);
}

// ----------------------------------------------------------------------------
// Class Modification
// ----------------------------------------------------------------------------

Modification::Modification(string pa, string d, int t, string pr, string g, string p, list<Indexage> i, string o) : File(pa, d, t, pr, g, p, i) {
  this->oldpath = o;
}

Modification::~Modification() {}

string Modification::GetOldpath() const {
  return this->oldpath;
}

// ----------------------------------------------------------------------------
// Class Creation
// ----------------------------------------------------------------------------

Creation::Creation(string pa, string d, int t, string pr, string g, string p, list<Indexage> i, string f) : File(pa, d, t, pr, g, p, i) {
  this->format = f;
}

Creation::~Creation() {}

string Creation::GetFormat() const {
  return this->format;
}

// ----------------------------------------------------------------------------
// Class Suppression
// ----------------------------------------------------------------------------

Suppression::Suppression(string p) {
  this->path = p;
}

Suppression::~Suppression() {}

string Suppression::GetPath() const {
  return this->path;
}

// ----------------------------------------------------------------------------
// Class Indexation
// ----------------------------------------------------------------------------
Indexation::Indexation(list<Renommage> r, list<Modification> m, list<Suppression> s, list<Creation> c) {
  this->renom = r;
  this->modif = m;
  this->supp = s;
  this->crea = c;
}

Indexation::~Indexation() {
}

list<Renommage> Indexation::GetRenom() const {
  return this->renom;
}

list<Modification> Indexation::GetModif() const {
  return this->modif;
}

list<Suppression> Indexation::GetSupp() const {
  return this->supp;
}

list<Creation> Indexation::GetCrea() const {
  return this->crea;
}
