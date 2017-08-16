#ifndef __CC_HASH_MAP_H__
#define __CC_HASH_MAP_H__

#include "ccTypes_ext.h"
#include "support/data_support/uthash.h"
#include "platform/CCPlatformMacros.h"
#include <string>
#include "ccMacros.h"

NS_CC_BEGIN



template <class T>
class CCHashElement
{
public:
	// The max length of string key.
	#define   MAX_KEY_LEN   256

    /**
     *  Constructor of CCHashElement. It's only for internal usage. CCHashMap is its friend class.
     *  
     *  @param  pszKey    The string key of this element.
     *  @param  pObject   The object of this element.
     */
    CCHashElement(const char* pszKey, T pObject)
	{
		CCAssert(pszKey && strlen(pszKey) > 0, "Invalid key value.");
		m_iKey = 0;
		const char* pStart = pszKey;

		int len = strlen(pszKey);
		if (len > MAX_KEY_LEN )
		{
			char* pEnd = (char*)&pszKey[len-1];
			pStart = pEnd - (MAX_KEY_LEN-1);
		}

		strcpy(m_szKey, pStart);

		m_pObject = pObject;
		memset(&hh, 0, sizeof(hh));
	}

    /**
     *  Constructor of CCHashElement. It's only for internal usage. CCHashMap is its friend class.
     *
     *  @param  iKey    The integer key of this element.
     *  @param  pObject   The object of this element.
     */
    CCHashElement(intptr_t iKey, T pObject)
	{
		m_szKey[0] = '\0';
		m_iKey = iKey;
		m_pObject = pObject;
		memset(&hh, 0, sizeof(hh));
	}
    
public:
    /**
     *  The destructor of CCHashElement.
     */
    ~CCHashElement(){}

    // Inline functions need to be implemented in header file on Android.
    
    /**
     * Get the string key of this element.
     * @note    This method assumes you know the key type in the element. 
     *          If the element's key type is integer, invoking this method will cause an assert.
     *
     * @return  The string key of this element.
     */
    inline const char* getStrKey() const
    {
        CCAssert(m_szKey[0] != '\0', "Should not call this function for integer dictionary");
        return m_szKey;
    }

    /**
     * Get the integer key of this element.
     * @note    This method assumes you know the key type in the element.
     *          If the element's key type is string, invoking this method will cause an assert.
     *
     * @return  The integer key of this element.
     */
    inline intptr_t getIntKey() const
    {
        CCAssert(m_szKey[0] == '\0', "Should not call this function for string dictionary");
        return m_iKey;
    }
    
    /**
     * Get the object of this element.
     *
     * @return  The object of this element.
     */
    inline T getObject() const { return m_pObject; }

public:
    
    // char array is needed for HASH_ADD_STR in UT_HASH.
    // So it's a pain that all elements will allocate 256 bytes for this array.
    char      m_szKey[MAX_KEY_LEN];     // hash key of string type
    intptr_t  m_iKey;       // hash key of integer type
    T m_pObject;    // hash value
public:
    UT_hash_handle hh;      // makes this class hashable
    //friend class CCHashMap; // declare CCHashMap as friend class
};

/** The macro for traversing dictionary
 *  
 *  @note It's faster than getting all keys and traversing keys to get objects by objectForKey.
 *        It's also safe to remove elements while traversing.
 */
#define CCHASH_FOREACH(_T_, __dict__, __el__) \
    CCHashElement<_T_>* pTmp##__dict__##__el__ = NULL; \
    if (__dict__) \
    HASH_ITER(hh, (__dict__)->m_pElements, __el__, pTmp##__dict__##__el__)



template <class T>
class CCHashMap 
{
public:
    /** 
     * The constructor of CCHashMap.
     * @lua NA
     */
    CCHashMap()
	: m_pElements(NULL)
	, m_eDictType(kCCDictUnknown)
	{

	}

    /**
     * The destructor of CCHashMap
     * @lua NA
     */
    ~CCHashMap()
	{
		removeAllObjects();
	}

    /**
     *  Get the count of elements in CCHashMap.
     *
     *  @return  The count of elements.
     */
    unsigned int count() const
	{
		return HASH_COUNT(m_pElements);
	}

    /**
     *  Return all keys of elements.
     *
     *  @return  The array contains all keys of elements. It's an autorelease object yet.
     */
    //CCArray* allKeys();

    /** 
     *  Get all keys according to the specified object.
     *  @warning  We use '==' to compare two objects
     *  @return   The array contains all keys for the specified object. It's an autorelease object yet.
     */
    //CCArray* allKeysForObject(CCObject* object);

    /**
     *  Get the object according to the specified string key.
     *
     *  @note The dictionary needs to use string as key. If integer is passed, an assert will appear.
     *  @param key  The string key for searching.
     *  @return The object matches the key. You need to force convert it to the type you know.
     *  @code
     *     // Assume that the elements are CCString* pointers. Convert it by following code.
     *     CCString* pStr = (CCString*)pDict->objectForKey("key1");
     *     // Do something about pStr.
     *     // If you don't know the object type, properly you need to use dynamic_cast<SomeType*> to check it.
     *     CCString* pStr2 = dynamic_cast<CCString*>(pDict->objectForKey("key1"));
     *     if (pStr2 != NULL) {
     *          // Do something about pStr2
     *     }
     *  @endcode
     *  @see objectForKey(intptr_t)
     */
    T objectForKey(const std::string& key)
	{
		// if dictionary wasn't initialized, return NULL directly.
		if (m_eDictType == kCCDictUnknown) return NULL;
		// CCDictionary only supports one kind of key, string or integer.
		// This method uses string as key, therefore we should make sure that the key type of this CCDictionary is string.
		CCAssert(m_eDictType == kCCDictStr, "this dictionary does not use string as key.");

		T pRetObject = NULL;
		CCHashElement<T> *pElement = NULL;
		HASH_FIND_STR(m_pElements, key.c_str(), pElement);
		if (pElement != NULL)
		{
			pRetObject = pElement->m_pObject;
		}
		return pRetObject;
	}
    
    /**
     *  Get the object according to the specified integer key.
     *
     *  @note The dictionary needs to use integer as key. If string is passed, an assert will appear.
     *  @param key  The integer key for searching.
     *  @return The object matches the key.
     *  @see objectForKey(const std::string&)
     */
    T objectForKey(intptr_t key)
	{
		// if dictionary wasn't initialized, return NULL directly.
		if (m_eDictType == kCCDictUnknown) return NULL;
		// CCDictionary only supports one kind of key, string or integer.
		// This method uses integer as key, therefore we should make sure that the key type of this CCDictionary is integer.
		CCAssert(m_eDictType == kCCDictInt, "this dictionary does not use integer as key.");

		T pRetObject = NULL;
		CCHashElement<T> *pElement = NULL;
		HASH_FIND_PTR(m_pElements, &key, pElement);
		if (pElement != NULL)
		{
			pRetObject = pElement->m_pObject;
		}
		return pRetObject;
	}
    
    /** Get the value according to the specified string key.
     *
     *  @note Be careful to use this function since it assumes the objects in the dictionary are CCString pointer.
     *  @param key  The string key for searching
     *  @return An instance of CCString.
     *          It will return an empty string if the objects aren't CCString pointer or the key wasn't found.
     *  @see valueForKey(intptr_t)
     */
    //const CCString* valueForKey(const std::string& key);
    
    /** Get the value according to the specified integer key.
     *
     *  @note Be careful to use this function since it assumes the objects in the dictionary are CCString pointer.
     *  @param key  The string key for searching.
     *  @return An instance of CCString.
     *          It will return an empty string if the objects aren't CCString pointer or the key wasn't found.
     *  @see valueForKey(intptr_t)
     */
    //const CCString* valueForKey(intptr_t key);

    /** Insert an object to dictionary, and match it with the specified string key.
     *
     *  @note Whe the first time this method is invoked, the key type will be set to string.
     *        After that you can't setObject with an integer key.
     *        If the dictionary contains the key you passed, the object matching the key will be released and removed from dictionary.
     *        Then the new object will be inserted after that.
     *
     *  @param pObject  The Object to be inserted.
     *  @param key      The string key for searching.
     *  @see setObject(CCObject*, intptr_t)
     */
    void setObject(T pObject, const std::string& key)
	{
		CCAssert(key.length() > 0 && pObject != NULL, "Invalid Argument!");
		if (m_eDictType == kCCDictUnknown)
		{
			m_eDictType = kCCDictStr;
		}

		CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as key.");

		CCHashElement<T> *pElement = NULL;
		HASH_FIND_STR(m_pElements, key.c_str(), pElement);
		if (pElement == NULL)
		{
			setObjectUnSafe(pObject, key);
		}
		else if (pElement->m_pObject != pObject)
		{
			T pTmpObj = pElement->m_pObject;
			//pTmpObj->retain();
			removeObjectForElememt(pElement);
			setObjectUnSafe(pObject, key);
			//pTmpObj->release();
		}
	}
    
    /** Insert an object to dictionary, and match it with the specified string key.
     *
     *  @note Then the first time this method is invoked, the key type will be set to string.
     *        After that you can't setObject with an integer key.
     *        If the dictionary contains the key you passed, the object matching the key will be released and removed from dictionary.
     *        Then the new object will be inserted after that.
     *  @param pObject  The Object to be inserted.
     *  @param key      The string key for searching.
     *  @see setObject(CCObject*, const std::string&)
     */
    void setObject(T pObject, intptr_t key)
	{
		CCAssert(pObject != NULL, "Invalid Argument!");
		if (m_eDictType == kCCDictUnknown)
		{
			m_eDictType = kCCDictInt;
		}

		CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as key.");

		CCHashElement<T> *pElement = NULL;
		HASH_FIND_PTR(m_pElements, &key, pElement);
		if (pElement == NULL)
		{
			setObjectUnSafe(pObject, key);
		}
		else if (pElement->m_pObject != pObject)
		{
			CCObject* pTmpObj = pElement->m_pObject;
			pTmpObj->retain();
			removeObjectForElememt(pElement);
			setObjectUnSafe(pObject, key);
			pTmpObj->release();
		}
	}

    /** 
     *  Remove an object by the specified string key.
     *
     *  @param key  The string key for searching.
     *  @see removeObjectForKey(intptr_t), removeObjectsForKeys(CCArray*),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
    void removeObjectForKey(const std::string& key)
	{
		if (m_eDictType == kCCDictUnknown)
		{
			return;
		}

		CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as its key");
		CCAssert(key.length() > 0, "Invalid Argument!");
		CCHashElement<T> *pElement = NULL;
		HASH_FIND_STR(m_pElements, key.c_str(), pElement);
		removeObjectForElememt(pElement);
	}
    
    /**
     *  Remove an object by the specified integer key.
     *
     *  @param key  The integer key for searching.
     *  @see removeObjectForKey(const std::string&), removeObjectsForKeys(CCArray*),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
    void removeObjectForKey(intptr_t key)
	{
		if (m_eDictType == kCCDictUnknown)
		{
			return;
		}

		CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as its key");
		CCHashElement<T> *pElement = NULL;
		HASH_FIND_PTR(m_pElements, &key, pElement);
		removeObjectForElememt(pElement);
	}
    
    /**
     *  Remove objects by an array of keys.
     *
     *  @param pKeyArray  The array contains keys to be removed.
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
    //void removeObjectsForKeys(CCArray* pKeyArray);
    
    /**
     *  Remove an object by an element.
     *
     *  @param pElement  The element need to be removed.
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectsForKeys(CCArray*), removeAllObjects().
     *  @lua NA
     */
    void removeObjectForElememt(CCHashElement<T>* pElement)
	{
		if (pElement != NULL)
		{
			HASH_DEL(m_pElements, pElement);
			CC_SAFE_DELETE(pElement);
		}
	}
    
    /**
     *  Remove all objects in the dictionary.
     *
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectsForKeys(CCArray*), removeObjectForElememt(CCDictElement*).
     */
    void removeAllObjects()
	{
		CCHashElement<T> *pElement, *tmp;
		HASH_ITER(hh, m_pElements, pElement, tmp) 
		{
			HASH_DEL(m_pElements, pElement);
			CC_SAFE_DELETE(pElement);
		}
	}

    /// @{
    /// @name Function override
    /**
     *  This function is used for deepcopy elements from source dictionary to destination dictionary.
     *  You shouldn't invoke this function manually since it's called by CCObject::copy.
     *  @lua NA
     *  @js  NA
     */
    //virtual CCObject* copyWithZone(CCZone* pZone);
    /// @}
    
    /**
     *  Return a random object in the dictionary.
     *
     *  @return The random object. 
     *  @see objectForKey(intptr_t), objectForKey(const std::string&)
     *  @lua NA
     */
    //T* randomObject();
    
    /**
     *  Create a dictionary.
     *  @return A dictionary which is an autorelease object.
     *  @see createWithDictionary(CCHashMap*), createWithContentsOfFile(const char*), createWithContentsOfFileThreadSafe(const char*).
     */
    static CCHashMap<T>* create()
	{
		return new CCHashMap<T>();
	}

    /**
     *  Create a dictionary with an existing dictionary.
     *
     *  @param srcDict The exist dictionary.
     *  @return A dictionary which is an autorelease object.
     *  @see create(), createWithContentsOfFile(const char*), createWithContentsOfFileThreadSafe(const char*).
     */
    //static CCHashMap* createWithDictionary(CCHashMap* srcDict);
    
    /**
     *  Create a dictionary with a plist file.
     *  @param  pFileName  The name of the plist file.
     *  @return A dictionary which is an autorelease object.
     *  @see create(), createWithDictionary(CCHashMap*), createWithContentsOfFileThreadSafe(const char*).
     */
    //static CCHashMap* createWithContentsOfFile(const char *pFileName);
    
    /**
     *  Write a dictionary to a plist file.
     *  @param fullPath The full path of the plist file. You can get writeable path by getWritablePath()
     *  @return true if successed, false if failed
     *  @lua NA
     */
    //bool writeToFile(const char *fullPath);
     
    /**
     *  Create a dictionary with a plist file.
     *  
     *  @note the return object isn't an autorelease object.
     *        This can make sure not using autorelease pool in a new thread.
     *        Therefore, you need to manage the lifecycle of the return object.
     *        It means that when you don't need it, CC_SAFE_RELEASE needs to be invoked.
     *
     *  @param  pFileName  The name of the plist file.
     *  @return A dictionary which isn't an autorelease object.
     *  @lua NA
     */
    //static CCHashMap* createWithContentsOfFileThreadSafe(const char *pFileName);

    /* override functions 
     * @lua NA
     */
    //virtual void acceptVisitor(CCDataVisitor &visitor);

private:
    /** 
     *  For internal usage, invoked by setObject.
     */
    void setObjectUnSafe(T pObject, const std::string& key)
	{
		CCHashElement<T>* pElement = new CCHashElement<T>(key.c_str(), pObject);
		HASH_ADD_STR(m_pElements, m_szKey, pElement);
	}

    void setObjectUnSafe(T pObject, const intptr_t key)
	{
		CCHashElement<T>* pElement = new CCHashElement<T>(key, pObject);
		HASH_ADD_PTR(m_pElements, m_iKey, pElement);
	}
    
public:
    /**
     *  All the elements in dictionary.
     * 
     *  @note For internal usage, we need to declare this member variable as public since it's used in UT_HASH.
     */
    CCHashElement<T>* m_pElements;
private:
    
    /** The support type of dictionary, it's confirmed when setObject is invoked. */
    enum CCDictType
    {
        kCCDictUnknown = 0,
        kCCDictStr,
        kCCDictInt
    };
    
    /** 
     *  The type of dictionary, it's assigned to kCCDictUnknown by default.
     */
    CCDictType m_eDictType;
};


NS_CC_END

#endif