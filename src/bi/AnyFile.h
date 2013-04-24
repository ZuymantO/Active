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
#include <cstdlib>
#include <cstdio>
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
  string ext ;  // file extension
  int     mask;
  short   hard_link;  // no hard link
  unsigned int user_id;
  unsigned int group_id;
  unsigned int   disk_size;
  time_t*       last_acces;
  time_t*       last_modif;
  time_t*       last_stat;
  time_t*       created_on; // file's creation date
  time_t*       insert_date;  // date of insertion on the database
  
  
  AnyFile(){};
  ~AnyFile(){};

  bool majField(const string& irname, const string& irvalue){

    if (irname == "path") {
      path = irvalue;
      return true;
    }else if(irname == "name"){
      name = irvalue;
      return true;
    }else if(irname == "mime"){
      mime = irvalue;
      return true;
    }else if(irname == "ext"){
      ext = irvalue;
      return true;
    }else if(irname == "mask"){
      mask = atoi(irvalue.c_str());
      return true;
    }else if(irname == "hard_link"){
      hard_link = atoi(irvalue.c_str());
      return true;
    }else if(irname == "hash_gen"){
      hash_gen = irvalue;
      return true;
    }else if(irname == "user_flags"){
      user_flags = irvalue;
      return true;
    }else if(irname == "id"){
      idData = atoi(irvalue.c_str());
      return true;
    }else if(irname == "user_id"){
      user_id = atoi(irvalue.c_str());
      return true;
    }else if(irname == "group_id"){
      group_id = atoi(irvalue.c_str());
      return true;
    }else if(irname == "disk_size"){
      disk_size = atoi(irvalue.c_str());
      return true;
    }else if(irname == "last_acces"){
      name = "" ;
        // TODO convertir les str en dates system pour stocker dans la classe (faire des tests pour voir le format de sqlite)
    }else if(irname == "last_modif"){
      name = irvalue;
      return true;
    }else if(irname == "created_on"){
      name = irvalue;
      return true;
    }else if(irname == "insert_date"){
      name = irvalue;
      return true;
    }
    
    return false;
  };

};
#endif /* defined(__BI__AnyFile__) */
