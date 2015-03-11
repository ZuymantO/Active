#include "SearchStruct.h"

SearchStruct::SearchStruct() : text(""), content(false), permissions(-1), beginDate(""), endDate("") {

}

void SearchStruct::setText(std::string cnt){
  this->text = cnt;
}

void SearchStruct::setSourceDir(std::string dir){
  this->srcDir = dir;
}

void SearchStruct::setBeginDate(int day, int month, int year){
  //TODO: transformer les 3 valeurs en une chaine compatible avec sqlite3
}

void SearchStruct::setEndDate(int day, int month, int year){
  //TODO: transformer les 3 valeurs en une chaine compatible avec sqlite3
}

/* Transforme un code xml en une structure de recherche SearchStruct */
SearchStruct SearchStruct::xmlToSearchStruct(std::string xml){
  SearchStruct search;

  //TODO

  return search;
}

/* Indique si la recherche est une recherche par contenu ou par nom */
bool SearchStruct::isContent(){
  return content;
}

std::string SearchStruct::getText(){
  return text;
}

std::string SearchStruct::getSourceDir(){
  return srcDir;
}

std::string SearchStruct::getBeginDate(){
  return beginDate;
}

std::string SearchStruct::getEndDate(){
  return endDate;
}

std::string SearchStruct::toQuery(){

}

SearchStruct::~SearchStruct(){}
