#ifndef __CC_VECTOR_H__
#define __CC_VECTOR_H__

#include "ccTypes_ext.h"
#include "support/data_support/uthash.h"
#include "platform/CCPlatformMacros.h"
#include <string>
#include <vector>
#include "ccMacros.h"

NS_CC_BEGIN

template <class T>
class CCVector
{
public:
	~CCVector()
	{
		_data.clear();
	}

    /** Create an array */
    static CCVector* create()
	{
		return new CCVector<T>();
	}
    /** Create an array with some objects 
     *  @lua NA
     */
    //static CCVector* create(T pObject, ...);
    /** Create an array with one object */
    //static CCVector* createWithObject(T pObject);
    /** Create an array with capacity */
    //static CCVector* createWithCapacity(unsigned int capacity);
    /** Create an array with an existing array */
    //static CCVector* createWithArray(CCVector* otherArray);
    /**
     @brief   Generate a CCVector pointer by file
     @param   pFileName  The file name of *.plist file
     @return  The CCVector pointer generated from the file
     */
    //static CCVector* createWithContentsOfFile(const char* pFileName);
    
    /*
     @brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
     invoker should call release().
     @lua NA
     */
    //static CCVector* createWithContentsOfFileThreadSafe(const char* pFileName);

    /** Initializes an array 
     *  @lua NA
     */
    bool init()
	{
		return initWithCapacity(1);
	}
    /** Initializes an array with one object
     *  @lua NA
     */
    //bool initWithObject(T pObject);
    /** Initializes an array with some objects 
     *  @lua NA
     */
    //bool initWithObjects(T pObject, ...);
    /** Initializes an array with capacity 
     *  @lua NA
     */
    bool initWithCapacity(unsigned int capacity)
	{
		_data.reserve(capacity);
		return true;
	}
    /** Initializes an array with an existing array 
     *  @lua NA
     */
    //bool initWithArray(CCVector* otherArray);

    // Querying an Array

    /** Returns element count of the array */
    unsigned int count() const
	{
		return _data.size();
	}
    /** Returns capacity of the array */
    unsigned int capacity() const
	{
		return _data.capacity();
	}
    /** Returns index of a certain object, return UINT_MAX if doesn't contain the object */
    unsigned int indexOfObject(T object) const
	{
		for(unsigned int i=0; i<_data.size(); ++i)
		{
			if(_data.at(i) == object) return i;
		}
		return CC_INVALID_INDEX;
	}

    /** Returns an element with a certain index */
    T objectAtIndex(unsigned int index)
	{
		CCAssert(index < _data.size(), "index out of range in objectAtIndex()");
		return _data[index];
	}

    /** Returns last element */
    T lastObject()
	{
		CCAssert(_data.size() > 0, "index out of range in lastObject()");
		return _data[_data.size()-1];
	}

    /** Returns a random element */
    //T randomObject();
    /** Returns a Boolean value that indicates whether object is present in array. */
    bool containsObject(T object) const
	{
		return indexOfObject(object) != CC_INVALID_INDEX;
	}

    /** @since 1.1 */
    //bool isEqualToArray(CCVector* pOtherArray);
    // Adding Objects

    /** Add a certain object */
    void addObject(T object)
	{
		_data.push_back(object);
	}

    /** Add all elements of an existing array */
    //void addObjectsFromArray(CCVector* otherArray);
    /** Insert a certain object at a certain index */
    void insertObject(T object, unsigned int index)
	{
		if(index >= _data.size())
		{
			_data.push_back(object);
		}
		else
		{
			_data.insert(index, object);
		}
	}

    // Removing Objects

    /** Remove last object */
    void removeLastObject(bool bReleaseObj = true)
	{
		typename std::vector<T>::iterator it = _data.rend();
		_data.erase(it);
	}

    /** Remove a certain object */
    void removeObject(T object, bool bReleaseObj = true)
	{
		typename std::vector<T>::iterator it = _data.begin();
		for(it; it!=_data.end(); ++it)
		{
			if(*it != object) continue;
			_data.erase(it);
		}
	}

    /** Remove an element with a certain index */
    void removeObjectAtIndex(unsigned int index, bool bReleaseObj = true)
	{
		if(index > _data.size()) return;
		typename std::vector<T>::iterator it = _data.begin() + index;
		_data.erase(it);
	}
    /** Remove all elements */
    //void removeObjectsInArray(CCVector* otherArray);
    /** Remove all objects */
    void removeAllObjects()
	{
		_data.clear();
	}
    /** Fast way to remove a certain object */
    void fastRemoveObject(T object)
	{
		removeObject(object);
	}

    /** Fast way to remove an element with a certain index */
    void fastRemoveObjectAtIndex(unsigned int index)
	{
		removeObjectAtIndex(index);
	}

    // Rearranging Content

    /** Swap two elements */
    void exchangeObject(T object1, T object2)
	{
		unsigned int src = indexOfObject(object1); if(src == CC_INVALID_INDEX) return;
		unsigned int des = indexOfObject(object2); if(des == CC_INVALID_INDEX) return;
		exchangeObjectAtIndex(src,des);
	}

    /** Swap two elements with certain indexes */
    void exchangeObjectAtIndex(unsigned int index1, unsigned int index2)
	{
		if(index1 >= _data.size() || index2 >= _data.size()) return;
		typename std::vector<T>::iterator s_it = _data.begin() + index1;
		typename std::vector<T>::iterator d_it = _data.begin() + index2;
		_data.swap(s_it, d_it);
	}

    /** Replace object at index with another object. */
    void replaceObjectAtIndex(unsigned int uIndex, T pObject, bool bReleaseObject = true)
	{
		unsigned int index = objectAtIndex(uIndex);
		if(index == CC_INVALID_INDEX) return;
		_data[index] = pObject;
	}

    /** Revers the array */
    void reverseObjects()
	{
		int count = (int) floorf(_data.size()/2.f); 
		unsigned int maxIndex = _data.size() - 1;

		for (int i = 0; i < count ; i++)
		{
			exchangeObjectAtIndex(_data, i, maxIndex);
			maxIndex--;
		}
	}

    /* Shrinks the array so the memory footprint corresponds with the number of items */
    //void reduceMemoryFootprint();
  
    /** override functions 
     *  @js NA
     *  @lua NA
     */
    //virtual T copyWithZone(CCZone* pZone);

    /* override functions */
    //virtual void acceptVisitor(CCDataVisitor &visitor);

public:
    /**
     *  @lua NA
     */
    CCVector()
	{

	}
    /**
     *  @lua NA
     */
    CCVector(unsigned int capacity)
	{
		initWithCapacity(capacity);
	}

public:
	std::vector<T>& getData(){return _data;}

protected:
	std::vector<T> _data;
};

#define CCVECTOR_FOREACH(_T_, __array__, __object__) \
	std::vector<_T_>& _v_data_ = (__array__)->getData(); \
	for(unsigned int _i_ = 0; _i_ < (_v_data_).size() && (__object__ = _v_data_[_i_]); ++_i_)
	

#define CCVECTOR_FOREACH_REVERSE(__array__, __object__)                                                                  \
	if ((__array__) && (__array__)->data->num > 0)                                                                      \
	for(CCObject** __arr__ = (__array__)->data->arr + (__array__)->data->num-1, **__end__ = (__array__)->data->arr;     \
	__arr__ >= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);                                              \
	__arr__--)


NS_CC_END

#endif