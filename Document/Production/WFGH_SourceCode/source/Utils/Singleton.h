#ifndef _SINGLETON_H
#define _SINGLETON_H

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Singleton implementation

template <class T>
class Singleton
{
private:
    static T* s_instance;

public:
    static T* getInstance()
	{
		// doesn't create the instance automatically
		ASSERT(s_instance);
		return s_instance;
	}

    static bool hasInstance()
	{
		return s_instance != NULL;
	}


    static void freeInstance()
	{
		delete s_instance;
		s_instance = NULL;
	}

protected:
    Singleton()
	{
		ASSERT(s_instance == 0);
        s_instance = (T*) this;
	}

    virtual ~Singleton()
	{
		s_instance = NULL;
	}
};



template <class T>
T* Singleton<T>::s_instance = 0;


#endif