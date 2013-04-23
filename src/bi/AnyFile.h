//
//  AnyFile.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__AnyFile__
#define __BI__AnyFile__

#include <iostream>
#include <time.h>
using namespace std;

class AnyFile {
private:
  
public:
    /* En public pour travailler vite et y avoir acces par exterieur, 
     * il faudra mettre private et creer les setters getters adequates avant soumissions finale
     */
  int idData;
  string        hash_gen;
  string        user_flags;
  string path;
  string name;
  string mime;
  string extension;
  int     mask;
  short   nb_hard_link;
  unsigned int user_id;
  unsigned int group_id;
  unsigned int   disk_size;
  time_t*       last_acces;
  time_t*       last_modif;
  time_t*       last_stat;
  time_t*       created_date;
  time_t*       insert_date;
  
  
  AnyFile();
  ~AnyFile();

  
};
#endif /* defined(__BI__AnyFile__) */
