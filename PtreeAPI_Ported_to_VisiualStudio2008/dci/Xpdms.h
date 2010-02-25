#ifndef XPDMS_HH
#define XPDMS_HH

#include <string>
#include <map>
#include <vector>
#include <libxml\parser.h>
#include "XMLException.h"

#include "AttributeInfo.h"

class Xpdms {
private:
  std::string xmlfilename;

  int cardinality;
  char *delimiter;
  char *cat_delimiter;
  std::vector<AttributeInfo*> attributes;
  std::map<std::string,std::string> datafiles;
  std::string title;
  std::string description;

public:
  Xpdms(std::string fname);
  Xpdms();
  void setFileName(std::string fname);

  void parse(void) throw(XMLException);

  std::string getDescription(void);
  int getCardinality(void);
  const char* getDelimiter(void);
  const char* getCatDelimiter(void);

  std::vector<AttributeInfo*> getAttributeInfo(void);
  std::string getAttributeFile(const std::string& att_name) const;
protected:
  void parseDescription(xmlNodePtr cur) throw(XMLException);

  void parseCardinality(xmlNodePtr cur) throw(XMLException);
  void parseDelimiter(xmlNodePtr cur) throw(XMLException);
  void parseCatDelimiter(xmlNodePtr cur) throw(XMLException);

  void parseDataFile(xmlNodePtr cur) throw(XMLException);
  void parseAttribute(xmlNodePtr cur,char* filename) throw(XMLException);
  void parseAttribute_old(xmlNodePtr cur) throw(XMLException);

private:
  char* removeSpaces(const char* str);
  char* removeInternalSpaces(const char* str);
  char* emptyLine();

};

#endif // XPDMS_HH
