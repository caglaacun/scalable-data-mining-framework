#pragma once
#include "modelselection.h"
#include "WrapDataSource.h"
#include "BitStreamInfo.h"
#include "datasource.h"

class ClassifierTree
{
public:
	
	_declspec(dllexport) ClassifierTree(void);
	
	_declspec(dllexport) virtual ~ClassifierTree(void);
	
	_declspec(dllexport) ClassifierTree(ModelSelection * toSelectLocModel);
	
	/**
	* Method for building a classifier tree.
	*	
	*/
	_declspec(dllexport) virtual void buildTree(DataSource * data, BitStreamInfo * _existence_map,boolean keepData);
	
	_declspec(dllexport) virtual void buildClassifier(DataSource * data,BitStreamInfo * _existence_bitmap);
	
	int SonsLength() const { return m_sonsLength; }
	
	void SonsLength(int val) { m_sonsLength = val; }

	/**
	* Returns a newly created tree.	
	*/
	ClassifierTree * getNewTree(DataSource * _data, BitStreamInfo * _existence_map);
	
private:
	/** The model selection method. */  
	ModelSelection * m_toSelectModel;     

	/** Local model at node. */
	ClassifierSplitModel * m_localModel;  

	/** References to sons. */
	ClassifierTree ** m_sons; 

	vector<ClassifierTree *> m_sons_vect;

	/** Length of the m_sons array. */
	int m_sonsLength;
	
	/** True if node is leaf. */
	bool m_isLeaf;                   

	/** True if node is empty. */
	bool m_isEmpty;                  

	/** The training instances. */
	DataSource * m_train;                  

	/** Existence Bitmap corresponding to this dataset. */
	BitStreamInfo * m_exitec;                  

	/** The pruning instances. */
	Distribution * m_test;     

	/** The id for the node. */
	int m_id;	
};