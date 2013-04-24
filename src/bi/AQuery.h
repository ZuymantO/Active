//
//  AQuery.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__AQuery__
#define __BI__AQuery__

#include <iostream>
#include "AnyFile.h"
#include "TextFile.h"
#include "VideoFile.h"
#include "ImageFile.h"
#include "ArchiveFile.h"
#include "AudioFile.h"
#include "ObjectFile.h"
#include <map>
#include <vector>

typedef enum _aqueryType{
  UNKNOWT = 0,
  INSERT, // insert  // peut avoir une liste de resultat (qui sont des elements a inserer)
  UPDATE,  // update
  DELETE,  // delete
  SELECT // search ou S comme Select
}AQueryType;


typedef enum _aqueryObject{
  ANY = 0,
  TEXT, //
  IMAGE,
  AUDIO,  //
  VIDEO,  //
  OBJECT, //
  ARCHIVE
}AQueryObject;

  // Un objet de cette classe est genere par le module d'analyse de requete xml
  // Une fois l'AQuery génere on fait appel a getSQLQuery() qui generer la requetes sql correspondante
  // Une fois cette requete recupere, on peut alors interrogere la bdd
  //
  // Cette classe permet aussi la communication inverse. C'est à dire une fois une requetes sql effectué,
  // Recuperer les resultats dans un objet SQLQuery, de cette objet on génere un AQuery.
  // Et ainsi de l'AQuery l'on pourra generer une reauete XML.
class SQLQuery;
class AQuery {
private:
  bool  hasResult;          // Si la requete contient des resultats on l'indique
  AQueryType aqt;
  AQueryObject aobj;
protected:
  void initAQuery(){
    AQResult = NULL;
    hasResult = false;
    aqt = UNKNOWT;
    aobj = ANY;
  };
  
public:
  vector<AnyFile>*               AQResult;  // Si la requete contient des resultats on les stock ici

  AQuery(){
    initAQuery();
  };
  
  AQuery(AQueryType ietype, AQueryObject ieobj): aqt(ietype), aobj(ieobj){
    AQResult = NULL;
    hasResult = NULL;
  };
  void addFile(AnyFile* ipnewFile);
  void addTextFile(TextFile* ipnewFile);
  void addObjectFile(ObjectFile* ipnewFile);
  void addVideoFile(VideoFile* ipnewFile);
  void addImageFile(ImageFile* ipnewFile);
  void addAudioFile(AudioFile* ipnewFile);
  void addArchiveFile(ArchiveFile* ipnewFile);
    // asc = ascendant
  void sortByTypeName(bool iasc);
  void sortBySize(bool iasc);
  void sortByUser(bool iasc);
  void sortByName(bool iasc);
  void sortByCreationDate(bool iasc);
  void sortByAccesDate(bool iasc);
  void sortByModificationDate(bool iasc);
  
  vector<AnyFile>* results();
  
  SQLQuery* getSQLquery();
  bool hasNewResult(){
    return hasResult;
  }
  ~AQuery(){};
};

#endif /* defined(__BI__AQuery__) */
