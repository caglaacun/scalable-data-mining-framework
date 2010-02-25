// 
// File:   AbstractPTreeSet.h
// Author: serazi
//
// Created on October 24, 2003, 6:25 PM
//

#ifndef _AbstractPTreeSet_H
#define _AbstractPTreeSet_H
#include "PTreeInfo.h"
#include "Exception.h"
#include "BasicPt.h"

/**
* Abstract Ptree set should be implemented by any PtreeSet implementaton
*
* This class will handle the Ptree management (creation, storing, loading) and the Ptree 
* Operations (OLAP type) for counting. This is the heart of the Ptree API
* Good starting point to explore..........
*/

template <typename size_type>
class  AbstractPTreeSet{
public:
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
	* dp=lastDerivedIndex(); 
	* bpt(1,2) & (!bpt(1,3) | bpt(1,4)) & bpt(1)
	* count( bpt(1,2) & (!bpt(1,3) | bpt(1,4))&bpt(dp),true);
	*
	* If the mask is set for the ptree set all ptree operations will 
	* AND the mask ptree.  
	*/         
	virtual size_type count(const BasicPt& bpt, bool store=false) = 0;
	/**
	* For a given pattern this will create a derived ptree, store it, and
	* return the index where the derived ptree has been stored
	*/ 
	virtual size_type derived_ptree(const BasicPt& bpt) = 0;

	/**
	* Given a basic pattern this will return all the tuple indeces that are true   
	* Will return a vector of size_type 
	* Can be used to get each tuple with getTuple();
	* caution : this could be very large. check count before using 
	* 
	*/
	virtual std::vector<size_type> getAllTupleIndices(const BasicPt& bpt) = 0;

	/**
	* Given a basic pattern this will check whether the corresponding derived 
	* ptree has already been generated. If so, retun the index where the
	* ptree has been stored or else return -1 to indicate that required ptree
	* has not been generated yet.
	*/
	virtual int derived_index(const BasicPt& bpt)/*const*/ = 0;

	/**
	* For a given index (spatial) this method will retrieve the corresponding tuple
	* from the ptree set. If the index >= cardinality will throw exception
	* Can be used as a DB retrieval mechanism for accessing records in the 
	* current Ptree set
	*/

	virtual Tuple getTuple(size_type index)throw(IndexOutOfBoundsException) = 0;     
	/**
	* Will return the PtreeSetId used to load/store the current PtreeSet.
	*/ 
	virtual string getPtreeSetId() const = 0;

	/**
	* Will return the PTreeInfo the current PtreeSet. This will incldue all the 
	* inforamation about the attributes.
	*/
	virtual PTreeInfo getPTreeInfo() const = 0;

	/**
	* Will load all ptrees to memory. The unique id given when storing the ptree set
	* will be used to load the ptree set. 
	* An Exception will be thrown if you provide an invalid id. So while calling this
	* method use try{...} catch(InvalidIdException& e){...} blocks.
	*/       
	virtual void load(const string& id) throw(InvalidIdException)= 0;
	/**
	* Store the PtreeSet to the secondary storage with a unique identifier.
	* This enables the storage of striped Ptrees for later loading and use.
	* Will avoid having to generate Ptrees when required.  
	* The Ptree set will be created with the given Idnetifier (directory). 
	* If the identifier already exists it will throw IdExistsException
	* So when calling this method need use
	* try{...} catch(IdExistsException& e){...} blocks.
	*/ 
	virtual void store(const string& id) throw(IdExistsException) = 0;

	/**
	* Return number of ptrees in the current set. This will return the number
	* Bit slices create from the original data. This is not the number of Attributes
	* This does not include the number of temprory derived Ptrees
	*/  
	virtual size_type degree()const = 0;

	/**
	* Will return the index of the last derived Ptree created with a call to 
	* the count method with store=true. The returned index will be valid for the 
	* duration of life of the current Ptree Set.
	*/
	virtual int lastDerivedIndex()const = 0;

	/**
	* Will allow to create a mask Ptree for the current Ptree set. This mask can
	* be used to filter out/in tuples when doing ptree operations.
	* By default (mask_type=false) given indices will be left out of operations
	* If required to include given indices and leave out the rest set mask_type=true
	* NOTE need to explcitly call setMaskPTree to enable the mask
	*/
	virtual void createMaskPTree(const vector<size_type>& indices,bool mask_type=false) = 0;

	/**
	* Will allow to set the mask Ptree for the current Ptree set. This mask can
	* be used to filter out/in tuples when doing ptree operations. 
	* If a maskTree does not exisit all true pure mask will be used for the Ptree
	* operations if it is set (without creating) This will be a waste of computation
	* Can remove the use of the mask with removeMaskPTree(). 
	*/
	virtual void setMaskPTree() = 0;
	/**
	* Will allow to remove the mask Ptree for the current Ptree set. 
	* NOTE will only un-set (!set) the mask. To indicate to the ptree set
	* not to use it for future ptree operations. The last mask created will be
	* retained by the ptree set for future use (with a call to set) if required.
	*/     
	virtual void removeMaskPTree() = 0;

	/**
	* remove all ptrees that belongs to a given attribute
	* @param index of an attribute
	* @exception InvalidException
	*/

	/**
	* Removes all Derived ptrees 
	* WARNING: all previous derived indexes become invalid
	*/
	virtual void removeAllDerivedPTrees() = 0;


	virtual void remove(size_t index) throw(InvalidIndexException) = 0;

	/**
	* remove all ptrees that belongs to a given attribute
	* @param name of an attribute
	* @exception InvalidException
	*/
	virtual void remove(const string& name) throw(InvalidNameException) = 0;

	/**
	* get all ptrees that belongs to a given attribute
	* @param index of an attribute
	* @exception InvalidException
	*/
	virtual vector<PTree> getAttributePTrees(size_t index) throw(InvalidIndexException) = 0;

	/**
	* get all ptrees that belongs to a given attribute
	* @param name of an attribute
	* @exception InvalidNmeException, InvalisdIndexException
	*/
	virtual vector<PTree>  getAttributePTrees(const string& name) throw(InvalidNameException) = 0;


	/**
	* get all ptrees that belongs to a given attribute
	* @param attribute name
	* @param aInfo information about attribute
	* @exception InvalidException
	* Known Issues: not sure about using vector<PTree> since user needs to now about the internals
	* May be vector<boost:dynamic_bitset>
	*/
	virtual void addAttribute(const vector<PTree>& attribute , AttributeInfo & aInfo) throw(InvalidSizeException) = 0;


	/**
	* get all ptrees that belongs to a given attribute
	* @param index of an attribute
	* @param c1=|R1|*...*|Ri-1|
	* @param c2=|Ri+1|*...*|Rn|
	* @exception InvalidIndexException
	*/
	virtual vector<PTree> getReplicatedAttributePTrees(size_t index, size_t c1, size_t c2) throw(InvalidIndexException) = 0;

	/**
	* create a ptree for the given set of indices
	* @param indices set
	*/
	virtual int createDerivedPTree(const vector<size_type>& indices)=0;



};

#endif	/* _AbstractPTreeSet_H */

