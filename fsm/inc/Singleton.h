//--------------------------------------------------------------------
// Name: Shyam M Guthikonda
// Date: 12.11.04
// File: Singleton.h
// Desc: Singleton is used as a base class throughout the project as a
//		 guideline for all objects in which there should only be 1 instance
//		 of at any time. Eliminates the need to re-implement the same
//		 code each time. Idea taken from Game Programming Gems I.
//--------------------------------------------------------------------

#ifndef _Singleton_h_
#define _Singleton_h_

#include <cstdlib>
#include <cassert>

/* To use this class, derive a class, B, from it. To create an object B,
do 'new B'. Calling 'new B' again will create a run-time error. A pointer
to B does not need to be saved. To delete it, call 'delete B::getSingletonPtr()'.
To access B's members, call B::getSingleton().method() or B::getSingleton().data.
*/

template< typename T >
class Singleton
{
public:
	Singleton() {
		assert( !m_singleton && "(constructor) Singleton already exists!" );

		/* Given that the 'this' pointer in Singleton::Singleton() will *always*
		point to the 'Singleton' part of an object - regardless of whether the
		Singleton is not at the beginning of it - we need to adjust the 'this'
		pointer - usually by a negative amount, because the object will start before
		the Singleton part of it does.

		"(T*)1" sets up an expression which points to an imaginary object of type T
		at 0x00000001.

		Casting that expression of type T* to Singleton* will adjust the value
		to point to the singleton portion of it. So, if there were 4 bytes in the
		object before the Singleton part of it started, casting (T*)0x00000001
		to (Singleton*) would be (Singleton*)0x00000005.

		Converting both values back to ints and subtracting the second from the first
		would, in this example, give -4 (0x00000001 - 0x00000005) - the number of
		bytes the 'this' pointer must be adjusted by to make it point to the beginning
		of the ENTIRE object, not just the singleton part.
		*/

		// Don't display the annoying warning generated by the following lines of code.
		#pragma warning( push )
		#pragma warning( disable : 4312 )
		#pragma warning( disable : 4311 )

		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		m_singleton = (T*)((int)this + offset);

		#pragma warning( pop )
	}

	~Singleton() {
		assert( m_singleton && "(destructor) Singleton doesn't exist" );
		m_singleton = 0;
	}
	
	static inline T& getSingleton() {
		assert( m_singleton && "(getSingleton) Singleton doesn't exist" );
		return *m_singleton;
	}
	
	static inline T* getSingletonPtr() {
		if ( !m_singleton )
			return NULL;
		else
			return m_singleton;
	}

private:
	static T* m_singleton;
};

template <typename T> T* Singleton <T>::m_singleton = 0;

#endif // _Singleton_h_