//-----------------------------------------------------------
// File: templates.h
// Description: template classes for some reuse. Contains a singleton
//				class to make singleton creation standerdized.
//-----------------------------------------------------------

#ifndef TEMPLATES_H
#define TEMPLATES_H

// Template for singletons. Classes are intended to inherit from singleton
// and must be of the form described below:

/*
class ClassName : public Singleton<ClassName>
{
public: 
	friend Singleton<ClassName>;

protected:
	ClassName();
	~ClassName();
};

// in .cpp file
template<> ClassName* Singleton<ClassName>::instance = NULL;
*/
template< typename T > class Singleton
{
protected:
	Singleton() { }
	static T* instance;

public:

	// Provides access to the class instance
	static T& GetInstance()
	{
		ASSERT( instance != NULL );
		return *instance;
	}

	// Instantiated the instance of the class and calls the 
	// protected class constructor
	static void Create()
	{
		instance = new T();
	}

	// Destroyed the class invoking the protected destructor
	static void Destroy()
	{
		delete instance;
		instance = 0;
	}

private:

	// Disabled operations
	Singleton( Singleton< T > const & );
	Singleton& operator=( const Singleton< T > & );
};

#endif