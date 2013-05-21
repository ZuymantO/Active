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

namespace acommon {

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
  string user_id;
  string group_id;
  unsigned int   disk_size;
  string       last_acces;
  string      last_modif;
  string       last_stat;
  string       created_on; // file's creation date
  string       insert_date;  // date of insertion on the database

  int GetIdData() const;
  string GetHashGen() const;
  string GetUserFlags() const;
  string GetPath() const;
  string GetName() const;
  string GetMime() const;
  string GetExt() const;
  int GetMask() const;
  short GetHardLink() const;
  string GetUserId() const;
  string GetGroupId() const;
  unsigned int GetDiskSize() const;
  string GetLastAcces() const;
  string GetLastModif() const;
  string GetLastStat() const;
  string GetCreatedOn() const;
  string GetInsertDate() const;
  
  
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
      user_id = irvalue;
      return true;
    }else if(irname == "group_id"){
      group_id = irvalue;
      return true;
    }else if(irname == "disk_size"){
      disk_size = atoi(irvalue.c_str());
      return true;
    }else if(irname == "last_acces"){
      last_acces = irvalue ;
    }else if(irname == "last_modif"){
      last_modif = irvalue;
      return true;
    }else if(irname == "created_on"){
      created_on = irvalue;
      return true;
    }else if(irname == "insert_date"){
      insert_date = irvalue;
      return true;
    }
    
    return false;
  };

};
}

#endif /* defined(__BI__AnyFile__) */
