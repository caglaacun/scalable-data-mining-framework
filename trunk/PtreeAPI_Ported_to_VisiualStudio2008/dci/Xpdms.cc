#include <string>
#include <libxml/parser.h>
#include "Xpdms.h"
#include "XMLException.h"

#include "AttributeInfo.h"
#include "IntAttributeInfo.h"
#include "SignIntAttributeInfo.h"
#include "UsignIntAttributeInfo.h"
#include "SignDoubleAttributeInfo.h"
#include "UsignDoubleAttributeInfo.h"
#include "SingCatAttributeInfo.h"
#include "MultiCatAttributeInfo.h"
#include "SkipAttributeInfo.h"

#include <iostream>

using namespace std;

Xpdms::Xpdms(string fname) {
  xmlfilename=fname;
  description="";
  delimiter="";
  cat_delimiter="";
}

Xpdms::Xpdms() {
  xmlfilename=string("");
  description="";
  delimiter="";
  cat_delimiter="";
}

void Xpdms::setFileName(string fname) {
  xmlfilename=fname;
}

string Xpdms::getDescription(void) {
  return description;
}


int Xpdms::getCardinality(void) {
  return cardinality;
}

const char* Xpdms::getDelimiter(void) {
  return delimiter;
}

const char* Xpdms::getCatDelimiter(void) {
  return cat_delimiter;
}

vector<AttributeInfo*> Xpdms::getAttributeInfo(void) {
  return attributes;
}

string Xpdms::getAttributeFile(const string& att_name) const {
  map<string,string> m=datafiles;
  return m[att_name];
}

void Xpdms::parse() throw(XMLException) {
  xmlDocPtr doc;
  xmlNodePtr cur;
  doc=xmlParseFile(xmlfilename.c_str());
  if(doc==NULL) {
    throw XMLException("Parse error");
  }
  cur=xmlDocGetRootElement(doc);
  if(cur==NULL) {
    throw XMLException("Empty document");
  }

  if(xmlStrcmp(cur->name,(const xmlChar*) "datasetinfo")) {
    // cout << "Root node==" << cur->name << endl;
    xmlFreeDoc(doc);
    throw XMLException("Document of the wrong type, root node != xpdms");
  }
  // cout << "Root element is datasetinfo" << endl;
  //cout << "datafile:" << xmlGetProp(cur,(const xmlChar*)"datafile") << endl;
  /* going through 
   * 1. description
   * 2. cardinality
   * 3. delimiter
   * 4. data_file
   */
  /* 
  xmlChar *card=xmlGetProp(cur,(const xmlChar*)"cardinality");
  //string scard;
  if(card==NULL) {
    cout << "card==NULL\n";
    scard="";
  } else {
    scard=(char*)card;  //!!! ?
  }
  cout << "cardinality:" << scard << endl;
  cout << "---------- Attributes ------------\n";
  */
  cur=cur->xmlChildrenNode;
  while(cur!=NULL) {
    if(cur->type==XML_ELEMENT_NODE ) {
      //cout << cur->name << endl;
      if(!(xmlStrcmp(cur->name,(const xmlChar*)"description"))) {
	parseDescription(cur);
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"cardinality"))) {
	parseCardinality(cur);
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"delimiter"))) {
	parseDelimiter(cur);
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"multi_cat_delim"))) {
	parseCatDelimiter(cur);
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"data_file"))) {
	parseDataFile(cur);
      } else {
	cout << "wrong tag:" << cur->name << endl;
      }      
    }
    cur=cur->next;
  }
  if(!strcmp(delimiter,"")) {
    throw XMLException("delimeter is not defined");
  }
  if(!strcmp(delimiter,cat_delimiter)) {
    throw XMLException("delimeter is equal to multi_cat_delim");
  }
  xmlFreeDoc(doc);
}

void Xpdms::parseDescription(xmlNodePtr cur) throw(XMLException) {
  // cout << "\x1b[41mparse description\x1b[0m\n";
  description="";
  cur=cur->children;
  while(cur) {
    if(cur->type==XML_ELEMENT_NODE) {
      //cout << cur->name << endl;
      if(!(xmlStrcmp(cur->name,(const xmlChar*)"title"))) {
	// parse title
	xmlNodePtr title_cur=cur->children;
	while(title_cur) {
	  if(title_cur->type==XML_ELEMENT_NODE) {
	    if(!(xmlStrcmp(title_cur->name,(const xmlChar*)"line"))) {
	      //cout << "Parsing line\n";
	      if(title_cur->children!=NULL) {
		char *line=
		  removeSpaces((const char*)(title_cur->children->content));
		//cout << "Spaces removed\n";
		description+=line;
		description+="\n";
		delete[] line;
	      }
	    }
	  }
	  title_cur=title_cur->next;
	}
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"sources"))) {
	// parse sources
	description+="\nSources:\n";
	int counter=0;
	xmlNodePtr src_cur=cur->children;
	while(src_cur) {
	  if(src_cur->type==XML_ELEMENT_NODE) {
	    if(!(xmlStrcmp(src_cur->name,(const xmlChar*)"source"))) {
	      char scounter[30];
	      sprintf(scounter,"  Source %i:\n",++counter);
	      description+=scounter;
	      xmlNodePtr src2_cur=src_cur->children;
	      char *creator=NULL,
		*donor=NULL,
		*date=NULL;
	      while(src2_cur) {
		if(src2_cur->type==XML_ELEMENT_NODE) {
		  if(!(xmlStrcmp(src2_cur->name,(const xmlChar*)"creator"))) {
		    if(src2_cur->children!=NULL) {
		      creator=
			removeSpaces((const char*)src2_cur->children->content);
		    } else {
		      creator=emptyLine();
		    }
		  } else if(!(xmlStrcmp(src2_cur->name,
					(const xmlChar*)"donor"))) {
		    if(src2_cur->children!=NULL) {
		      donor=
			removeSpaces((const char*)src2_cur->children->content);
		    } else {
		      donor=emptyLine();
		    }
		  } else if(!(xmlStrcmp(src2_cur->name,
					(const xmlChar*)"date"))) {
		    if(src2_cur->children!=NULL) {
		      date=
			removeSpaces((const char*)src2_cur->children->content);
		    } else {
		      date=emptyLine();
		    }
		  }
		}
		src2_cur=src2_cur->next;
	      }
	      if(creator==NULL || donor==NULL || date==NULL) {
		delete[] creator,donor,date;
		throw XMLException("Xpdms::parseDescription:"
				   "'creator', 'donor', and 'date' tags"
				   "expected in the 'source'");
	      }
	      description=description+"    Creator: "+creator+"\n";
	      description=description+"    Donor: "+donor+"\n";
	      description=description+"    Date: "+date+"\n\n";
	      delete[] creator,donor,date;
	    }
	  }
	  src_cur=src_cur->next;
	} 
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"relevant_info"))) {
	// cout << "Relevant info" << endl;
	xmlNodePtr ri_cur=cur->children;
	while(ri_cur) {
	  char *paragraph=NULL;
	  if(ri_cur->type==XML_ELEMENT_NODE) {
	    if(!(xmlStrcmp(ri_cur->name,(const xmlChar*)"p"))) {
	      if(ri_cur->children!=NULL) {
		paragraph=
		  removeInternalSpaces((const char*)ri_cur->children->content);
	      } else {
		paragraph=emptyLine();
	      }
	    }
	  }
	  if(paragraph!=NULL) {
	    description=description+"\n  "+paragraph;
	    delete[] paragraph;
	  }
	  ri_cur=ri_cur->next;
	}
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"url"))) {
	char *url=NULL;
	if(cur->children!=NULL) {
	  url=removeSpaces((const char*)(cur->children->content));
	} else {
	  url=emptyLine();
	}
	description=description+"\n\nURL:\n"+url+"\n";
	delete[] url;
      }
    }
    cur=cur->next;
  }
}

void  Xpdms::parseCardinality(xmlNodePtr cur) throw(XMLException) {
  // cout << "\x1b[41mparse cardinality\x1b[0m\n";
  cur=cur->children;
  if(cur->type==XML_TEXT_NODE) {
    // cout << "cardinality: " << cur->content << endl;
  } else {
    throw XMLException("xpdms::parseCardinality: expected XML_TEXT_NODE\n");
  }
  char *ccard=removeSpaces((const char*)(cur->content));
  cardinality=atoi(ccard);
  delete[] ccard;
}

void Xpdms::parseDelimiter(xmlNodePtr cur) throw(XMLException) {
  // cout << "\x1b[41mparse delimiter\x1b[0m\n";
  cur=cur->children;
  if(cur->type==XML_TEXT_NODE) {
    //cout << "delimiter: " << cur->content << endl;
  } else {
    throw XMLException("xpdms::parseDelimiter: expected XML_TEXT_NODE\n");
  }
  char *cdelim=removeSpaces(((const char*)(cur->content)));
  if(!strcmp(cdelim,"tab")) {
     delimiter="\t";
  } else if(!strcmp(cdelim,"comma")) {
     delimiter=",";
  } else if(!strcmp(cdelim,"space")) {
     delimiter=" ";
  } else if(!strcmp(cdelim,"verticalbar")) {
     delimiter="|";
  } else {
    delete[] cdelim;
    throw XMLException("ParseDelimiter: oops.. wrong delimiter");
  }
  delete[] cdelim;
}

void Xpdms::parseCatDelimiter(xmlNodePtr cur) throw(XMLException) {
  // cout << "\x1b[41mparse categorical delimiter\x1b[0m\n";
  cur=cur->children;
  if(cur->type==XML_TEXT_NODE) {
    //cout << "categorical delimiter: " << cur->content << endl;
  } else {
    throw XMLException("Xpdms::parseCatDelimiter: expected XML_TEXT_NODE\n");
  }
  char *cdelim=removeSpaces(((const char*)(cur->content)));
  if(!strcmp(cdelim,"tab")) {
     cat_delimiter="\t";
  } else if(!strcmp(cdelim,"comma")) {
      cat_delimiter=",";
  } else if(!strcmp(cdelim,"space")) {
      cat_delimiter=" ";
  } else if(!strcmp(cdelim,"verticalbar")) {
      cat_delimiter="|";
  } else {
    delete[] cdelim;
    throw XMLException("ParseCatDelimiter: oops.. wrong delimiter");
  }
  //cout<<"cat delemiter "<<cat_delimiter<<endl;
  delete[] cdelim;
}

void  Xpdms::parseDataFile(xmlNodePtr cur) throw(XMLException) {
  // cout << "\x1b[41mparse data_file\x1b[0m\n";
  xmlChar *xfilename=xmlGetProp(cur,(xmlChar*)"name");
  if(xfilename==NULL) 
    throw XMLException("Xpdms::parseDataFile: no 'name' attribute'");
  // cout << "filenamename:" << xfilename << endl;
  char *filename=removeSpaces((const char*)xfilename);
  cur=cur->children;
  while(cur) {
    if(cur->type==XML_ELEMENT_NODE) {
      if(!(xmlStrcmp(cur->name,(const xmlChar*)"attribute"))) {
	parseAttribute(cur,filename);
      } else {
	throw XMLException("Xpdms::parseDataFile: expected tag attribute");
      }
    }
    cur=cur->next;
  }
  delete[] filename;
}

void Xpdms::parseAttribute(xmlNodePtr cur,char* filename) throw(XMLException) {
  // cout << "\x1b[33mparse attribute\x1b[0m\n";
  cur=cur->children;
  xmlChar *xname=NULL,
    *xtype=NULL,
    *xprecision=NULL,
    *xdomainlower=NULL,
    *xdomainupper=NULL;
  vector<string> categorical_domain;
  categorical_domain.clear();
  while(cur) {
    // cout << cur->name << endl;
    if(cur->type==XML_ELEMENT_NODE) {
      if(!(xmlStrcmp(cur->name,(const xmlChar*)"name"))) {
	if(cur->children->type==XML_TEXT_NODE) {
	  // cout << "1\n";
	  xname=cur->children->content;
	  if(cur->children->next) {
	    throw XMLException("Xpdms::parseAttribute: tag: <name>: "
			       "#PCDATA expected");
	  }
	} else {
	  throw XMLException("Xpdms::parseAttribute: tag: <name>: "
			     "#PCDATA expected");
	}
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"type"))) {
	if(cur->children->type==XML_TEXT_NODE) {
	  // cout << "2 (" << cur->children->content << ")\n";
	  xtype=cur->children->content;
	  if(cur->children->next) {
	    throw XMLException("Xpdms::parseAttribute: tag: <type>: "
			       "#PCDATA expected");
	  }
	} else {
	  throw XMLException("Xpdms::parseAttribute: tag: <type>: "
			     "#PCDATA expected");
	}
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"precision"))) {
	if(cur->children->type==XML_TEXT_NODE) {
	  // cout << "3\n";
	  xprecision=cur->children->content;
	  if(cur->children->next) {
	    throw XMLException("Xpdms::parseAttribute: tag: <precision>: "
			       "#PCDATA expected");
	  }
	} else {
	  throw XMLException("Xpdms::parseAttribute: tag: <precision>: "
			     "#PCDATA expected");
	}
      } else if(!(xmlStrcmp(cur->name,(const xmlChar*)"domain"))) {
	//!! doesn't check for #PCDATA in <lower>, <upper> tags, just 
	//   gets first TEXT_NODE.
	xmlNodePtr dcur=cur->children;
	// cout << dcur->name << endl;
	char *type=removeSpaces((const char*)xtype);
	// cout << "type:" << type << endl;
	if(!strcmp(type,"int") || !strcmp(type,"double")) {
	  /*---------- int & double -------------*/
	  while(dcur) {
	    if(dcur->type==XML_ELEMENT_NODE) {
	      // cout << dcur->name << endl;
	      if(!(xmlStrcmp(dcur->name,(const xmlChar*)"lower"))) {
		xdomainlower=dcur->children->content;
		// cout << "4\n";
	      } else if(!(xmlStrcmp(dcur->name,(const xmlChar*)"upper"))) {
		// cout << "5\n";
		xdomainupper=dcur->children->content;
	      }
	    }
	    dcur=dcur->next;
	  } 
	} else if(!strcmp(type,"single valued categorical") 
		  || !strcmp(type,"multi valued categorical")) {
	  /*--------------- Categorical -------------*/
	  if(!strcmp(type,"multi valued categorical") 
	     && !strcmp(cat_delimiter,"")) {
	    throw XMLException("multi_cat_delim is not defined");
	  }
	  while(dcur) {
	    if(dcur->type==XML_ELEMENT_NODE) {
	      //cout << "categorical: " << dcur->name << " : " 
	      //   << dcur->children->content << endl;
	      if(!(xmlStrcmp(dcur->name,(const xmlChar*)"value"))) {
		char* element=
		  removeSpaces((const char*)dcur->children->content);
		categorical_domain.push_back(element);
		delete[] element;
	      }
	    }
	    dcur=dcur->next;
	  }
	} else {
	  //cout << "Error: type:" << type << endl;
	  delete[] type;
	  throw XMLException("Xpdms::parseAttribute: wrong type");
	}
	delete[] type;
      } else {
	throw XMLException("Xpdms::parseAttribute: 'name', 'type', "
			   "'precision', or 'domain' expected'");
      }
    }
    cur=cur->next;
  }
  if(xname==NULL || xtype==NULL) {
    throw XMLException("Xpdms::parseAttribute: 'name' and 'type' expected");
  }
  char *name=NULL,*type=NULL;
  //cout << "! " << xname << ":" << xtype << endl;
  name=removeSpaces((const char*)xname);
  type=removeSpaces((const char*)xtype);
  //cout << "! " << xname << ":" << xtype << endl;
  if(!strcmp(type,"int")) {
    int lower,upper;
    if(xdomainlower==NULL || xdomainupper==NULL) {
      delete[] name;
      delete[] type;
      throw XMLException("Xpdms::parseAttribute::parseDomain: "
			 " 'lower' and 'upper' expected");
    }
    char *sdomainlower,*sdomainupper;
    sdomainlower=removeSpaces((const char*)xdomainlower);
    sdomainupper=removeSpaces((const char*)xdomainupper);
    lower=atoi(sdomainlower);
    upper=atoi(sdomainupper);
    delete[] sdomainlower;
    delete[] sdomainupper;
    if(lower<0) {
      SignIntAttributeInfo *ai=new SignIntAttributeInfo();
      ai->setName(string(name));
      ai->setDomain(lower,upper);
      attributes.push_back((AttributeInfo*)ai);
    } else {
      UsignIntAttributeInfo *ai=new UsignIntAttributeInfo();
      ai->setName(string(name));
      ai->setDomain(lower,upper);
      attributes.push_back((AttributeInfo*)ai);
    }
    datafiles[string(name)]=string(filename);
  } else if(!strcmp(type,"double")) {
    double lower,upper;
    int precision;
    if(xdomainlower==NULL || xdomainupper==NULL || xprecision==NULL) {
      delete[] name;
      delete[] type;
      throw XMLException("Xpdms::parseAttribute::parseDomain: "
			 " 'precision', 'lower', and 'upper' expected");
    }
    char *sdomainlower,*sdomainupper,*sprecision;
    sdomainlower=removeSpaces((const char*)xdomainlower);
    sdomainupper=removeSpaces((const char*)xdomainupper);
    sprecision=removeSpaces((const char*)xprecision);
    lower=atof(sdomainlower);
    upper=atof(sdomainupper);
    precision=atoi(sprecision);
    delete[] sdomainlower;
    delete[] sdomainupper;
    delete[] sprecision;
    if(lower<0) {
      SignDoubleAttributeInfo *ai=new SignDoubleAttributeInfo();
      ai->setName(name);
      ai->setDomain(lower,upper);
      ai->setPrecision(precision);
      attributes.push_back((AttributeInfo*)ai);
    } else {
      UsignDoubleAttributeInfo *ai=new UsignDoubleAttributeInfo();
      ai->setName(string(name));
      ai->setDomain(lower,upper);
      ai->setPrecision(precision);
      attributes.push_back((AttributeInfo*)ai);
    }
    datafiles[string(name)]=string(filename);
  } else if(!strcmp(type,"single valued categorical")) {
    SingCatAttributeInfo *ai=new SingCatAttributeInfo();
    ai->setName(name);
    ai->setDomain(categorical_domain);
    attributes.push_back((AttributeInfo*)ai);
    datafiles[string(name)]=string(filename);
  } else if(!strcmp(type,"multi valued categorical")) {
    MultiCatAttributeInfo *ai=new MultiCatAttributeInfo();
    ai->setName(name);
    ai->setDomain(categorical_domain);
    attributes.push_back((AttributeInfo*)ai);
    datafiles[string(name)]=string(filename);
  } else if(!strcmp(type,"skip")) {
    SkipAttributeInfo *ai=new SkipAttributeInfo();
    ai->setName(name);
    attributes.push_back((AttributeInfo*)ai);
    datafiles[string(name)]=string(filename);
  } else {
    string msg("Unknown type: ");
    msg+=type;
    delete[] name;
    delete[] type;
    throw XMLException(msg);
  }
  delete[] name;
  delete[] type;
  /*
  if(xname==NULL || xtype==NULL || xdomainlower==NULL
     || xdomainupper==NULL) {
    throw XMLException("Xpdms::parseAttribute: 'name', 'type', "
		       "'precision', and 'domain' expected'");
  }
  cout << "name: " << xname << endl
       << "type: " << xtype << endl
       << "precision: " << xprecision << endl
       << "lower: " << xdomainlower << endl
       << "upper: " << xdomainupper << endl;
  */
  return;
}

char* Xpdms::removeSpaces(const char* str) {
  //cout << "in\n";
  // cout << "rsps: " << str << endl;
  // cout << "rspl: " << strlen(str) << endl;
  if(str==NULL) 
    return NULL;
  if(!strcmp(str,"")) {
    //cout << "Empty line\n";
    char *buf=new char[1];
    buf[0]=0;
    return buf;
  }
  char *buf=new char[strlen(str)+1];
  int start;
  for(start=0;start<strlen(str);start++) {
    if(!isspace(str[start]))
      break;
  }
  for(int i2=0,i=start;buf[i2]=str[i];i++,i2++);
  for(int i=strlen(buf)-1;i>=0;--i) {
    // cout << ":" << i << endl;
    if(!isspace(buf[i])) {
      buf[i+1]=0;
      break;
    }
  }
  //cout << "out\n";
  return buf;
}

char* Xpdms::removeInternalSpaces(const char* str) {
  //cout << "in\n";
  if(str==NULL) 
    return NULL;
  char *buf=new char[strlen(str)+1];
  int i,j;
  for(i=0,j=0;i<strlen(str);i++) {
    if(isspace(str[i])) {
      for(;isspace(str[i]) && i<strlen(str);) {
	i++;
      }
      buf[j++]=' ';
    }
    buf[j++]=str[i];
  }
  buf[i]='\0';
  return buf;
}

char* Xpdms::emptyLine() {
  char *buf=new char[1];
  buf[0]='\0';
  return buf;
}

/*
void Xpdms::parseAttribute_old(xmlNodePtr cur) throw(XMLException) {
  cout << "\x1b[33mparse attribute\x1b[0m\n";
  xmlChar *datafile=xmlGetProp(cur,(const xmlChar*)"datafile");
  xmlChar *name=xmlGetProp(cur,(const xmlChar*)"name");
  xmlChar *type=xmlGetProp(cur,(const xmlChar*)"type");
  const xmlChar *smin=NULL,
    *smax=NULL,
    *el=NULL;
  // xmlChar *domain=xmlGetProp(cur,(const xmlChar*)"domain");
  // xmlChar *binarylength=xmlGetProp(cur,(const xmlChar*)"binarylength");

  cout << "\x1b[33mfrom attributes\x1b[0m:" << endl;
  if(name!=NULL)
    cout << name << endl;
  if(type!=NULL)
    cout << type << endl;
  /*
 *if(domain!=NULL) 
 *  cout <<  domain << endl;
 *if(binarylength!=NULL)
 *  cout << binarylength << endl; 
// 

  // parsing inside of <attribute>
  xmlNodePtr attr=cur->xmlChildrenNode;
  while(attr!=NULL) {
    if(attr->type==XML_ELEMENT_NODE) {
      xmlNodePtr value=attr->children;
      if(value->type==XML_TEXT_NODE) {
	//xmlChar *str=xmlNodeGetContent(value);
	//cout << ">>>" << str << endl;
	//xmlFree(str);
	if(!(xmlStrcmp(attr->name,(const xmlChar*)"name")))
	  name=value->content;
	if(!(xmlStrcmp(attr->name,(const xmlChar*)"type")))
	  type=value->content;
        if(!(xmlStrcmp(attr->name,(const xmlChar*)"domain"))) {
	  cout << "parsing domain\n";
	  while(value!=NULL) {
	    // cout << "name:" << value->name << endl;
	    if(value->type==XML_ELEMENT_NODE 
	       && !(xmlStrcmp(value->name,(const xmlChar*)"min"))) {
	      smin=value->children->content;
	    }
	    if(value->type==XML_ELEMENT_NODE 
	       && !(xmlStrcmp(value->name,(const xmlChar*)"max"))) {
	      smax=value->children->content;
	    }
	    if(value->type==XML_ELEMENT_NODE 
	       && !(xmlStrcmp(value->name,(const xmlChar*)"el"))) {
	      el=value->children->content;
	      cout << "el:" << el << endl;
	    }
	    value=value->next;
	  }
	//  domain=value->content;
	}
	//if(!(xmlStrcmp(attr->name,(const xmlChar*)"binarylength")))
	//  binarylength=value->content;
      }
    }
    attr=attr->next;
  }

  cout << "\x1b[33mcomplete\x1b[0m:" << endl;
  if(name!=NULL)
    cout << name << endl;
  if(type!=NULL)
    cout << type << endl;
  if(!(xmlStrcmp(type,(const xmlChar*)"integer"))) {
    cout << "min:" << smin << endl;
    cout << "max:" << smax << endl;
  }
  //if(domain!=NULL) 
  //  cout <<  domain << endl;
  //if(binarylength!=NULL)
  //  cout << binarylength << endl;

  cout << "\x1b[33mcreating objects\x1b[0m:" << endl;
  if(!(xmlStrcmp(type,(const xmlChar*)"integer"))) {
    int min=atoi((const char*)smin),
      max=atoi((const char*)smax);
    if(max<min) 
      throw XMLException("max<min");
    cout << "for integer type ("<< min << "," << max << ") --> ";
    if(min<0) {
      cout << "signed integer\n";
    } else {
      cout << "unsigned integer\n";
      UsignIntAttributeInfo attr;
      string sname((const char*)name);
      attr.setName(sname);
      attr.setDomain(min,max);
    }
  }
  cout << "__________________\n";
}
*/
