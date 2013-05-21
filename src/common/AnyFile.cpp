//
//  AnyFile.cpp
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#include "AnyFile.h"
#include <string>

unsing namespace std;

namespace acommon{

int AnyFile::GetIdData() const {
  return this->idData;
}

string AnyFile::GetHashGen() const {
  return this->hash_gen;
}

string AnyFile::GetUserFlags() const {
  return this->user_flags;
}

string AnyFile::GetPaht() const {
  return this-path>;
}

string AnyFile::GetName() const {
  return this->name;
}

string AnyFile::GetMime() const {
  return this->mime;
}

string AnyFile::GetExt() const {
  return this->ext;
}

int AnyFile::GetMask() const {
  return this->mask;
}

short AnyFile::GetHardLink() const {
  return this->hard_link;
}

unsigned int AnyFile::GetUserId() const {
  return this->user_id;
}

unsigned int AnyFile::GetGroupId() const {
  return this->group_id;
}

unsigned int AnyFile::GetDiskSize() const {
  return this->disk_size;
}

time_t* AnyFile::GetLastAccess() const {
  return this->last_acces;
}

time_t* AnyFile::GetLastModif() const {
  return this->last_modif;
}

time_t* AnyFile::GetLastStat() const {
  return this->last_stat;
}

time_t* AnyFile::GetCreatedOn() const {
  return this->created_on;
}

time_t* AnyFile::GetInsertDate() const {
  return this->insert_date;
}

}
