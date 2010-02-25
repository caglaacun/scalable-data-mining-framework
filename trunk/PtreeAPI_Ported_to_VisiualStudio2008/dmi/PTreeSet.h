// File:   PTreeSet.h
// Author: serazi
//
// Created on October 24, 2003, 6:25 PM
//

#ifndef _PTreeSet_H
#define _PTreeSet_H
#include <vector>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include <dirent.h>    // for DIR
#include <sys/stat.h>  //mkdir
#include <fstream>
#include <stack>
#include <map>
#include <ctime>

#include <memory>

#include "Exception.h"
#include "PException.h"
#include "AbstractPTreeSet.h"
#include "Util.h"
#include "Tuple.h"
#include "PTreeInfo.h"
#include "RelationalDataFeeder.h"
#include "OpIndex.h"

/**
* A Non Distributed implementaton of AbstractPtreeSet
*
* This class will handle the Ptree management (creation, storing, loading) and the Ptree 
* Operations (for OLAP type count outputs) for counting. This is the heart of the Ptree API
* Good starting point to explore the API..........
* @todo This class is very long may need to refactor.....
*/
using namespace std;
class  PTreeSet : public AbstractPTreeSet<size_type>{
public:
	//typedef size_type size_type;
	PTreeSet():use_mask(false){}
	/**
	* Feed data to the PtreeSet for creating a ptree Data set 
	* from other data sources. Depending on the input data, feeder
	* needs a corresponding DataFeeder capable of reading the raw data
	* This method will feed all the available data tuples to to the PtreeSet.
	* i.e. verticaly stripe the original data and create the corresponding 
	* Ptrees
	*/  
	void feed(DataFeeder* const feeder)
		throw (OutOfRangeException, InvalidLengthException, OutOfDomainException)
	{
		try{
			meta_file = feeder->getMetaFileName();
			pi = feeder->getPTreeInfo();
			basic_tree = std::vector<PTree>(pi.num_ptrees());
			size_t cardinal = pi.cardinality();
			for(size_t i= 0; i<basic_tree.size(); i++){
				basic_tree[i] = PTree(cardinal, false);
			}
			cout<<"Number of Ptree: "<<basic_tree.size()<<endl;
			time_t t1, t2;
			for(size_t i= 0; i<cardinal; i++){
				Tuple t = feeder->getTuple(); 

				t1 = clock();
				boost::dynamic_bitset<> bs(pi.encode(t));
				cout<<"Feeding tuple:"<<i<<endl;
				add_data(bs, i);
				t2 = clock();
				double time_required = double(t2-t1)/CLOCKS_PER_SEC;
				cout<<"Feeding tuple:"<<i<<" takes "<<time_required<<" sec"<<endl;
			}
		}
		catch(const exception& oe){
			throw;
		}
	}
	/**
	* Store the PtreeSet to the secondary storage with a unique identifier.
	* This enables the storage of striped Ptrees for later loading and use.
	* Will avoid having to generate Ptrees when required.  
	* The Ptree set will be created with the given Idnetifier (directory). 
	* If the identifier already exists it will throw IdExistsException
	* So when calling this method need use
	* try{...} catch(IdExistsException& e){...} blocks.
	*/ 
	virtual void store(string const& id)throw(IdExistsException){
		set_id_ = id;
		DIR *dir = opendir(id.c_str());
		if(dir)throw IdExistsException("Id already Exist, fail to create dir "+id+". Existing");
		closedir(dir);
		//mkdir(id.c_str(), S_IRWXU | S_IRWXG);
		mkdir(id.c_str(), 0x5fd);
		for(int i=0; i<pi.num_ptrees(); i++){
			string file = id+"/p"+to_string(i)+".pct";
			fstream f(file.c_str(), ios::out|ios::binary);
			cout<<"In ptreeset size of ptree::store: "<<basic_tree[i].size()<<endl;
			serialize(f, basic_tree[i]);
			f.close();
		}
		//now store the xml meta file
		cout<<"Reading Metafile: "<<meta_file<<endl;
		ifstream in(meta_file.c_str());
		ofstream out((id + "/meta.xml").c_str());
		out<< in.rdbuf();
		out.close();
		in.close();
	}

	/**
	* Will load all ptrees to memory. The unique id given when storing the ptree set
	* will be used to load the ptree set. 
	* An Exception will be thrown if you provide an invalid id. So while calling this
	* method use try{...} catch(InvalidIdException& e){...} blocks.
	*/ 
	virtual void load(string const& ptreeSetId)throw(InvalidIdException){
		this->set_id_ = ptreeSetId;
		MetaFileParser parser(set_id_ + "/meta.xml");
		pi = parser.getPTreeInfo();
		mask = PTree(pi.cardinality(), false);
		basic_tree = vector<PTree>(pi.num_ptrees());
		for(int i=0; i<pi.num_ptrees(); i++){
			string file = ptreeSetId+"/p"+to_string(i)+".pct";
			fstream f(file.c_str(), ios::in|ios::binary);
			if(!deserialize(f, basic_tree[i]))
				throw  InvalidIdException("Not a Valid Id");
			//cout<<"In ptreeset size of ptree::load: "<<basic_tree[i].size()<<endl;  
			f.close();
		} 
	}

	/**
	* Will return the PtreeSetId used to load/store the current PtreeSet.
	*/ 
	virtual string getPtreeSetId() const{
		return set_id_;
	}


	/**
	* Will return the PTreeInfo the current PtreeSet. This will incldue all the 
	* inforamation about the attributes.
	*/
	virtual PTreeInfo getPTreeInfo() const{
		return pi;
	}

	/**
	* Return number of ptrees in the current set. This will return the number
	* Bit slices create from the original data. This is not the number of Attributes
	* This does not include the number of temprory derived Ptrees. To get the 
	* current count if derived Ptrees use lastDerivedIndex
	*/       
	virtual size_type degree()const{
		return basic_tree.size();
	}

	/**
	* For a given index (spatial) this method will retrieve the corresponding tuple
	* from the ptree set. If the index >= cardinality will throw exception
	* Can be used as a DB retrieval mechanism for accessing records in the 
	* current Ptree set
	* @todo May need to add mechanisms to give different types of indexes. Or
	* do index translations.
	*/ 
	virtual Tuple getTuple(size_type index)throw(IndexOutOfBoundsException){
		if(index >= pi.cardinality())
			throw IndexOutOfBoundsException(pi.cardinality(), index);
		boost::dynamic_bitset<> bs;
		for(size_t i=0; i<degree(); i++){
			bs.push_back(basic_tree[degree() - i -1][index]);
		}
		return pi.decode(bs);  
	}
	/**
	* Will return the count of the corresponding Value Ptree created by the 
	* Given pattern. 
	* i.e will return the numebr of occerences of the given pattern in the 
	* current Ptree set. 
	* 
	* A combination (pattern) of Ptrees can be assembled using 
	* the BasicPt and the required operators AND & OR | NOT !. 
	* eg. 
	* bpt(1,2) & (!bpt(1,3) | bpt(1,4))
	*
	* If Store is set (true) the resultant Ptree will be stored as a derived Ptree
	* This derived Ptree can be used later by it's index. Need to use the 
	* lastDerivedIndex() keep track of all the derived Ptrees. 
	* Derived Ptrees can be used with exisitng Ptrees
	* eg. 
	* count(bpt(1,2) & (!bpt(1,3) | bpt(1,4)) , true);
	* dp=astDerivedIndex(); 
	* bpt(1,2) & (!bpt(1,3) | bpt(1,4)) & bpt(1)
	* count( bpt(1,2) & (!bpt(1,3) | bpt(1,4))&bpt(dp),true);
	*
	* If the mask is set for the ptree set all ptree operations will 
	* AND the mask ptree.  
	*/    

	/*   
	virtual size_type count(const BasicPt& bpt, bool store = false ){
	stack<PTree*> st;
	PTree *pt0, *pt1, *pt2;//,pt3,pt4;
	//cout<<"PTreeSet::count()"<<endl;
	for(size_t i=0; i<bpt.size(); i++){
	OpIndex* op = bpt.getOpIndex(i);
	int t = op->type();
	size_t val;
	switch(t){
	case OpIndex::OPTR:
	val = op->value();
	switch(val){
	case AND:
	cout<<"AND "<<endl;
	pt0 = st.top();
	cout<<"pt0  :"<<pt0<<endl;
	st.pop();
	pt1 = st.top();
	cout<<"pt1  :"<<pt1<<endl;
	st.pop();
	pt2 = &((*pt0) & (*pt1));
	//cout<<"rslt :"<<pt2<<endl;
	st.push(pt2);
	//cout<<"AND ";
	break;
	case OR:
	pt0 = st.top();
	st.pop();
	pt1 = st.top();
	st.pop();
	pt2 = &((*pt0) | (*pt1));
	//cout<<pt2<<endl;
	st.push(pt2);
	cout<<"OR ";
	break;
	case NOT:
	cout<<"NOT "<<endl;
	pt0 = st.top();
	//cout<<"pt1  :"<<pt3<<endl;
	//pt0 = *(st.top());
	//cout<<"pt0  :"<<pt0<<endl;
	st.pop();
	pt1 = &(~(*pt0));
	//cout<<"rslt :"<<pt2<<endl;
	//pt4 = ~pt3;
	//cout<<"rslt :"<<pt4<<endl;
	st.push(pt1);
	//cout<<"NOT ";
	break;
	}
	break;
	case OpIndex::BASIC:
	val = op->value();
	st.push(&basic_tree[val]);
	//cout<<*(st.top())<<endl;
	break;
	case OpIndex::DRVD:
	val = op->value();
	if(val>=derived_tree.size()) break;
	st.push(&derived_tree[val]);
	//cout<<*(st.top())<<endl;
	break;   				
	}
	cout<<"stack: top " <<(*(st.top()))<< " size "<<st.size()<<endl; 
	}
	if(st.size() != 0)	{
	if(use_mask){
	pt0 = *(st.top()) & mask;
	}
	else pt0 = *(st.top());
	}
	else
	pt0 = PTree(pi.cardinality(), false);
	if(store) derived_tree.push_back(pt0);	
	return pt0.count();  
	}
	*/  
	/**
	* @todo handling BasicPt by count needs testing. 
	* My solution for the loss of reference problem on the stack.
	* Original code with a short term fix also available in commented sec
	* If stack grows for more than 2 temp results original code failed
	* New code needs testing !! - amal 
	*/

	virtual size_type count(const BasicPt& bpt, bool store=false){
		//cout<<"=============mask is "<<use_mask<<endl;
		//bool dbug=1; // expensive comment out when done!
		stack<PTree*> st;
		stack<PTree*> tempPtr;
		PTree pt0,pt1,pt2;
		PTree *ptTmp;
		//cout<<"PTreeSet::count()"<<endl;
		//if(bpt.size() == 0) pt0 = PTree(pi.cardinality(), true);
		for(size_t i=0; i<bpt.size(); i++){
			PTree *tmpt;//,pt1,pt2,pt3,pt4;
			OpIndex* op = bpt.getOpIndex(i);
			int t = op->type();
			size_t val;
			switch(t){
				case OpIndex::OPTR:
					val = op->value();
					switch(val){
						case AND:
							// if (dbug) cout<<"AND "<<endl;
							pt0 = *(st.top());
							//if (dbug) cout<<"pt0  :"<<pt0<<endl;
							st.pop();
							pt1 = *(st.top());
							//if (dbug) cout<<"pt1  :"<<pt1<<endl;
							st.pop();
							//pt2 = PTree(pt0 & pt1);
							tmpt = new PTree(pt0 & pt1);
							//if (dbug) cout<<"rslt :"<<*tmpt<<endl;
							st.push(tmpt/*new PTree(pt0 & pt1)*/ );
							tempPtr.push(tmpt);
							//tempPtr.push(true); 
							//cout<<"tmp size: "<<tmpt->size()<<endl; 
							//cout<<"tmp count: "<<tmpt->count()<<endl;   					
							break;
						case OR:
							// cout<<"OR ";
							pt0 = *(st.top());
							st.pop();
							pt1 = *(st.top());
							st.pop();
							tmpt = new PTree(pt0 | pt1);
							//if (dbug) cout<<"rslt :"<<*tmpt<<endl;
							st.push(tmpt/*new PTree(pt0 & pt1)*/ );
							tempPtr.push(tmpt);
							break;
						case NOT:
							// if (dbug) cout<<"NOT "<<endl;
							pt0 = *(st.top());
							//if (dbug) cout<<"pt0  :"<<pt0<<endl;
							st.pop();
							tmpt = new PTree(~pt0);
							st.push(tmpt/*new PTree(pt0 & pt1)*/ );
							//if (dbug) cout<<"rslt :"<<*tmpt<<endl;
							tempPtr.push(tmpt);
							break;
					}
					break;
				case OpIndex::BASIC:
					val = op->value();
					st.push(&basic_tree[val]);
					//cout<<*(st.top())<<endl;
					break;
				case OpIndex::DRVD:
					val = op->value();
					//if(val>=derived_tree.size()) break;
					st.push(&derived_trees[val]);
					//st.push(&derived_trees);
					//cout<<*(st.top())<<endl;
					break;   				
			}
			//if (dbug) cout<<"stack: top " <<(*(st.top()))<< " size "<<st.size()<<endl; 
		}
		if(st.size() != 0)	{
			if(use_mask){
				pt0 = *(st.top()) & mask;
			}
			else pt0 = PTree(*(st.top()));
		}
		else{
			//pt0 = PTree(pi.cardinality(), true);
			pt0.resize(pi.cardinality(), true);
			//cout<<pt0<<endl;
		}	
		if(store){ 
			derived_trees.push_back(pt0);
			derived_indices[bpt.toString()] = derived_trees.size() - 1;
		}

		//release the tmpPtr memory
		int tmp_size = tempPtr.size();        
		for(size_t i=0; i<tmp_size; i++){
			delete tempPtr.top();
			tempPtr.pop();
		}	
		return pt0.count();  
	}
	/**
	* For a given pattern this will create a derived ptree, store it, and
	* return the index where the derived ptree has been stored
	*/
	virtual size_type derived_ptree(const BasicPt& bpt){
		//bool dbug=1; // expensive comment out when done!
		stack<PTree*> st;
		stack<PTree*> tempPtr;
		PTree pt0,pt1,pt2;
		PTree *ptTmp;
		//cout<<"PTreeSet::count()"<<endl;
		//if(bpt.size() == 0) pt0 = PTree(pi.cardinality(), true);
		for(size_t i=0; i<bpt.size(); i++){
			PTree *tmpt;//,pt1,pt2,pt3,pt4;
			OpIndex* op = bpt.getOpIndex(i);
			int t = op->type();
			size_t val;
			switch(t){
				case OpIndex::OPTR:
					val = op->value();
					switch(val){
				case AND:
					// if (dbug) cout<<"AND "<<endl;
					pt0 = *(st.top());
					//if (dbug) cout<<"pt0  :"<<pt0<<endl;
					st.pop();
					pt1 = *(st.top());
					//if (dbug) cout<<"pt1  :"<<pt1<<endl;
					st.pop();
					//pt2 = PTree(pt0 & pt1);
					tmpt = new PTree(pt0 & pt1);
					//if (dbug) cout<<"rslt :"<<*tmpt<<endl;
					st.push(tmpt/*new PTree(pt0 & pt1)*/ );
					tempPtr.push(tmpt);
					//tempPtr.push(true); 
					//cout<<"tmp size: "<<tmpt->size()<<endl; 
					//cout<<"tmp count: "<<tmpt->count()<<endl;   						
					break;
				case OR:
					// cout<<"OR ";
					pt0 = *(st.top());
					st.pop();
					pt1 = *(st.top());
					st.pop();
					tmpt = new PTree(pt0 | pt1);
					//if (dbug) cout<<"rslt :"<<*tmpt<<endl;
					st.push(tmpt/*new PTree(pt0 & pt1)*/ );
					tempPtr.push(tmpt);
					break;
				case NOT:
					// if (dbug) cout<<"NOT "<<endl;
					pt0 = *(st.top());
					//if (dbug) cout<<"pt0  :"<<pt0<<endl;
					st.pop();
					tmpt = new PTree(~pt0);
					st.push(tmpt/*new PTree(pt0 & pt1)*/ );
					//if (dbug) cout<<"rslt :"<<*tmpt<<endl;
					tempPtr.push(tmpt);
					break;
					}
					break;
				case OpIndex::BASIC:
					val = op->value();
					st.push(&basic_tree[val]);
					//cout<<*(st.top())<<endl;
					break;
				case OpIndex::DRVD:
					val = op->value();
					//if(val>=derived_tree.size()) break;
					st.push(&derived_trees[val]);
					//st.push(&derived_trees);
					//cout<<*(st.top())<<endl;
					break;   				
			}
			//if (dbug) cout<<"stack: top " <<(*(st.top()))<< " size "<<st.size()<<endl; 
		}
		if(st.size() != 0)	{
			if(use_mask){
				pt0 = *(st.top()) & mask;
			}
			else pt0 = PTree(*(st.top()));
		}
		else{
			//pt0 = PTree(pi.cardinality(), true);
			pt0.resize(pi.cardinality(), true);
			//cout<<pt0<<endl;
		}	
		derived_trees.push_back(pt0);
		derived_indices[bpt.toString()] = derived_trees.size() - 1;
		//release the tmpPtr memory
		int tmp_size = tempPtr.size();        
		for(size_t i=0; i<tmp_size; i++){
			delete tempPtr.top();
			tempPtr.pop();
		}
		//since for this moment we only allow one derived ptree to be stored
		//it will always return 0, we might change it later 
		return derived_trees.size() - 1;  
	}

	/**
	* Given a basic pattern this will return all the tuple indeces that are true   
	* Will return a vector of size_type 
	* Can be used to get each tuple with getTuple();
	* caution : this could be very large. check count before using 
	* 
	*/
	virtual std::vector<size_type> getAllTupleIndices(const BasicPt& bpt){
		size_type derivedIndex = derived_ptree(bpt);
		std::vector<size_type> tmp = vector<size_type>();
		for(size_type i=0;i<derived_trees[derivedIndex].size();i++){
			if (derived_trees[derivedIndex][i]) tmp.push_back(i);
		}
		//std::vector<PTree>::iterator it = derived_trees.begin();
		derived_trees.erase(derived_trees.begin()+derivedIndex);
		derived_indices.erase(bpt.toString());
		return tmp;
	}


	/**
	* Given a basic pattern this will check whether the corresponding derived 
	* ptree has already been generated. If so, retun the index where the
	* ptree has been stored or else return -1 to indicate that required ptree
	* has not been generated yet.
	* WARNING:If you ever used a mask be carefull about reusing from derived Pt.
	* 
	*/
	virtual int derived_index(const BasicPt& bpt)/*const*/{
		const string s = bpt.toString();
		return derived_indices[s];
	}
	/**
	* Will return the index of the last derived Ptree created with a call to 
	* the count method sith store=true. The returned index will be valid for the 
	* duration of life of the current Ptree Set.
	*/
	virtual int lastDerivedIndex()const{
		if(derived_trees.size()==0) return -1;
		else	return derived_trees.size() - 1;
	}
	/**
	* Will allow to create a mask Ptree for the current Ptree set. This mask can
	* be used to filter out/in tuples when doing ptree operations.
	* By default (mask_type=false) given indices will be left out of operations
	* If required to include given indices and leave out the rest set mask_type=true
	* NOTE need to explcitly call setMaskPTree to enable the mask, After you call
	* setMaskPtree all the Ptree operations will use it untill you call removeMaskPTree
	* @param indices to be set
	* @param mask_type to be used to set the indexes.
	*/
	virtual void createMaskPTree(const vector<size_type>& indices,bool mask_type=false){
		//if ( mask.empty() ) mask = PTree(pi.cardinality()); 
		//above not available in boost_1_29_0 can change later

		//Following needs to be done since cardinality not known at PTree construction time
		if ( mask.size()==0) mask = PTree(pi.cardinality()); 

		//using set and reset to avoid init with 1 prob identified by Taufik
		if (mask_type) mask.reset(); else mask.set();
		//cout<<"\n bef set "<<endl;cout<<"mask_type="<<mask_type<<endl;
		//cout<<mask<<endl; 
		vector<size_type>::const_iterator it=indices.begin();
		for( ;it!=indices.end(); ++it){
			if(*it >= mask.size()); //don't do anything, possibly can throw
			else{
				mask[*it] = mask_type;
			}
		}
		//cout<<"\n mask_type="<<mask_type<<endl;
		//cout<<mask<<endl;
	}    
	/**
	* To create a derived Ptree and add to the current Ptree set. This Ptree can
	* be used as any other derived Ptree. 
	* NOTE similar to mask, in this case you have multiple masks. You need to 
	* explicitly identify the derived Ptree in BasicPt used for Ptree operations
	* @param indices to be set true or '1'
	*/
	virtual int createDerivedPTree(const vector<size_type>& indices){
		PTree pt=PTree(pi.cardinality());           
		vector<size_type>::const_iterator it=indices.begin();
		for( ; it!=indices.end(); it++){
			if(*it >= pt.size()); //don't do anything, possibly can throw
			else{
				pt[*it] = true;
				//cout<<"pt for "<<*it<<endl;
			}
		}
		//cout<<pt<<endl;
		derived_trees.push_back(pt);
		return derived_trees.size() - 1;
	}



	/**
	* Will allow to set the mask Ptree for the current Ptree set. This mask can
	* be used to filter out/in tuples when doing ptree operations. 
	* If a maskTree does not exisit all true pure mask will be used for the Ptree
	* operations if it is set (without creating) This will be a waste of computation
	* Can remove the use of the mask with removeMaskPTree(). 
	*/
	virtual void setMaskPTree(){
		use_mask = true; 
		//cout<<"in_setMask::===============================mask is "<<use_mask<<endl;
	}

	/**
	* Will allow to remove the mask Ptree for the current Ptree set. 
	* NOTE will only un-set (!set) the mask. To indicate to the ptree set
	* not to use it for future ptree operations. The last mask created will be
	* retained by the ptree set for future use (with a call to set) if required.
	*/
	virtual void removeMaskPTree(){
		use_mask = false;
	}


	/**
	* Removes all Derived ptrees 
	* @warning WARNING all previous derived indexes become invalid.
	*
	* @todo May need to add a remove for given index similar to RemoveAll.
	*
	*/
	virtual void removeAllDerivedPTrees(){ 
		derived_trees.clear();
		derived_indices.clear();
	}


	/**
	* remove all ptrees that belongs to a given attribute
	* @param index of an attribute
	* @exception InvalIndexException
	*/
	virtual void remove(size_t index) throw(InvalidIndexException){
		if(index >= pi.degree()) 
			throw InvalidIndexException("ERROR: Given index is not a valid index");
		else{
			// get number of ptree in the given attribute
			size_t num_ptree = pi[index].binaryLength();
			std::vector<PTree>::iterator it = basic_tree.begin();
			size_t startIndex=pi.getAttributeStartIndex(index);

			basic_tree.erase(it + startIndex, it + startIndex + num_ptree);
			pi.removeAttributeInfo(index);
		}    

	}

	/**
	* remove all ptrees that belongs to a given attribute
	* @param name of an attribute
	* @exception InvalNameException
	*/
	virtual void remove(const string& name) throw(InvalidNameException){
		try{
			size_t index = pi.getAttributeIndex(name);
			if(index>=pi.degree())
				throw InvalidIndexException("ERROR: Given name is not a valid attribute name"); 
			else{
				remove(index);
			}
		}
		catch(const exception& e){
			throw InvalidNameException("ERROR:PTreeSet: Given name is not a valid attribute name");
		}

	}

	/**
	* get all ptrees that belongs to a given attribute
	* @param index of an attribute
	* @exception InvalidException
	*/
	virtual vector<PTree> getAttributePTrees(size_t index) throw(InvalidIndexException){
		if(index >= pi.degree()) 
			throw InvalidIndexException("ERROR:PTreeSet: Given index is not a valid index");
		else{
			size_t num_ptrees = pi[index].binaryLength();
			std::vector<PTree> tmp = vector<PTree>(num_ptrees);
			size_t startIndex=pi.getAttributeStartIndex(index);  
			for(size_t i = 0; i<num_ptrees; i++){
				tmp[i]=basic_tree[startIndex+num_ptrees-1-i];
			}   
			return tmp;
		}
	}

	/**
	* get all ptrees that belongs to a given attribute
	* @param name of an attribute
	* @exception InvalidNameException
	*/
	virtual vector<PTree> getAttributePTrees(const string& name) throw(InvalidNameException){
		try{
			size_t index = pi.getAttributeIndex(name);
			if(index >= pi.degree())
				throw InvalidIndexException("ERROR::PTreeSet: Given name is not a valid attribute name"); 
			else{
				return getAttributePTrees(index);
			}
		}
		catch(const exception& e){
			throw InvalidNameException("ERROR::PTreeSet: Given name is not a valid attribute name");
		}
	}

	/**
	* get all ptrees that belongs to a given attribute
	* @param attribute name
	* @param aInfo information about attribute
	* @exception InvalidException
	* Known Issues: not sure about using vector<PTree> since user needs to now about the internals
	* May be vector<boost:dynamic_bitset>
	*/   
	virtual void addAttribute(const vector<PTree>& attribute,AttributeInfo & aInfo) throw (InvalidSizeException)      
	{ 
		if (pi.cardinality()==0)
			pi.setCardinality(attribute[0].size());
		for(int i=0;i<attribute.size();i++) {
			if( attribute[i].size() != pi.cardinality() ) 
				throw InvalidSizeException("ERROR:PTreeSet: Given PTree(s) have size missmatch");
		}
		//add  the Attribute Info
		pi.addAttributeInfo(&aInfo);
		//add  the new Attribute Bits
		for(size_t i = 0; i<attribute.size(); i++)
			basic_tree.push_back(attribute[i]);   
	}


	/**
	* get all ptrees that belongs to a given attribute
	* @param index of an attribute
	* @param c1=|R1|*...*|Ri-1|
	* @param c2=|Ri+1|*...*|Rn|
	* @exception InvalidIndexException
	*/
	virtual vector<PTree> getReplicatedAttributePTrees(size_t index, size_t c1, size_t c2) throw(InvalidIndexException){
		if(index >= pi.degree()) 
			throw InvalidIndexException("ERROR:PTreeSet: Given index is not a valid index");
		else{
			size_t num_ptrees = pi[index].binaryLength();
			std::vector<PTree> tmp = getAttributePTrees(index);
			size_t c = pi.cardinality();
			std::vector<PTree> vmv;
			for (size_t i=0; i<num_ptrees; i++){
				PTree ptree = PTree(c1*c*c2);
				size_t count=0;
				for (size_t j=0; j<c2; j++)
					for (size_t k=0; k<c; k++)
						for (size_t m=0; m<c1; m++)
							ptree[count++] = tmp[i][k];
				vmv.push_back(ptree);
			}
			return vmv;
		}
	}

private:
	void add_data(const boost::dynamic_bitset<>& bs,size_t index){
		if(bs.size() != basic_tree.size()) throw 
			InvalidLengthException("ERROR: Size of the bitset is not same as total number of ptree");
		for(size_t i = 0; i<degree(); i++){
			//basic_tree[i].push_back(bs[degree() - i -1]);
			basic_tree[i][index] = bs[degree() - i -1];
		}		
	}
	std::vector<PTree> basic_tree;
	//std::vector<PTree> derived_tree;
	vector<PTree> derived_trees;
	map<string, int> derived_indices;
	PTree mask;
	bool use_mask;
	string set_id_;
	PTreeInfo pi;
	string meta_file;
};



#endif	/* _PTreeSet_H */

