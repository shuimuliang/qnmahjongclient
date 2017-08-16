#ifndef __SINGLETON__
#define __SINGLETON__


template <class T>
class Singleton
{
public:
	static T* getInstance(){
		if(_instance == NULL)
		{
			_instance = new T;
		}
		return _instance;
	}

	static void Release()
	{
		if(_instance != NULL)
		{
			delete _instance;
			_instance = NULL;
		}
	}
protected:
	Singleton(void){}   

	virtual ~Singleton(void){}

	static T* _instance;
};
template <class T> T* Singleton<T>::_instance = NULL;
#endif  //__SINGLETON__