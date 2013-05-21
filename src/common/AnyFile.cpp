//
//  AnyFile.cpp
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#include "AnyFile.h"
#include <string>

namespace acommon {

using namespace std;

int AnyFile::GetIdData() const {
  return this->idData;
}

string AnyFile::GetHashGen() const {
  return this->hash_gen;
}

string AnyFile::GetUserFlags() const {
  return this->user_flags;
}

string AnyFile::GetPath() const {
  return this->path;
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

string AnyFile::GetUserId() const {
  return this->user_id;
}

string AnyFile::GetGroupId() const {
  return this->group_id;
}

unsigned int AnyFile::GetDiskSize() const {
  return this->disk_size;
}

string AnyFile::GetLastAcces() const {
  return this->last_acces;
}

string AnyFile::GetLastModif() const {
  return this->last_modif;
}

string AnyFile::GetLastStat() const {
  return this->last_stat;
}

string AnyFile::GetCreatedOn() const {
  return this->created_on;
}

string AnyFile::GetInsertDate() const {
  return this->insert_date;
}

}
