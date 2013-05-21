#ifndef __INDEXATION__
#define __INDEXATION__

#include <string>
#include <list>

using namespace std;

// Classe representant un indexage (un mot et son occurence)
class Indexage {
 private :
  string word;
  int occurrence;
 public :
  Indexage(string s, int o);
  ~Indexage();

  string GetWord() const;
  int GetOccurrence() const;
  void SetOccurrence(int i);
};

// Classe representant un renommage
class Renommage {
 private :
  string oldpath;
  string newpath;
 public :
  Renommage(string o, string n);
  ~Renommage();

  string GetOldPath() const;
  string GetNewPath() const;
};

// Classe representant un fichier pour la modification et la creation
class File {
 protected :
  string path;
  string date;
  unsigned int taille;
  string proprietaire;
  string groupe;
  string perm;
  list<Indexage> index;
 public :
  File(string pa, string d, unsigned int t, string pr, string g, string p, list<Indexage> i);
  File(string pa, string d, unsigned int t, string pr, string g, string p, Indexage i);
  ~File();

  void init(string pa, string d, unsigned int t, string pr, string g, string p, list<Indexage> i);

  string GetPath() const;
  string GetDate() const;
  unsigned int GetTaille() const;
  string GetProprietaire() const;
  string GetGroupe()const;
  string GetPerm() const;
  list<Indexage> GetIndex() const;

  void AddIndex(Indexage i);
};

// Classe representant la modification
class Modification {
 private :
  File* file;
  string newpath;
 public :
  Modification(string pa, string d, int t, string pr, string g, string p, list<Indexage> i, string n);
  ~Modification();

  File* GetFile() const;
  string GetNewpath() const;
};

// Classe representant la creation
class Creation {
 private :
  File* file;
  string format;
 public :
  Creation(string pa, string d, int t, string pr, string g, string p, list<Indexage> i, string f);
  ~Creation();

  File* GetFile() const;
  string GetFormat() const;
};

// Classe representant la suppression
class Suppression {
 private :
  string path;
 public :
  Suppression(string p);
  ~Suppression();

  string GetPath() const;
};

// Classe representant une indexation multiple
class Indexation {
 private :
  list<Renommage> renom;
  list<Modification> modif;
  list<Suppression> supp;
  list<Creation> crea;

 public :
  Indexation(list<Renommage> r, list<Modification> m, list<Suppression> s, list<Creation> c);
  ~Indexation();

  list<Renommage> GetRenom() const;
  list<Modification> GetModif() const;
  list<Suppression> GetSupp() const;
  list<Creation> GetCrea() const;
};

#endif
