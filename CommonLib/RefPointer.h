#pragma once
#include "ReferenceCounter.h"
template<typename T>
class RefPointer
{
public:
	RefPointer();
	RefPointer(T* object);
	RefPointer(const RefPointer& r);
	~RefPointer();

	const int RefCount() const { return m_Refs->count(); }

	T* GetData() { return m_Data; }

	RefPointer& operator=(const RefPointer& r);

	T* operator->() { return m_Data; }

private:
	T* m_Data = nullptr;
	ReferenceCounter* m_Refs = nullptr;

};

template<typename T>
RefPointer<T>::RefPointer()
	: m_Refs(new ReferenceCounter)
{
	m_Refs->Increment();
}

template<typename T>
RefPointer<T>::RefPointer(const RefPointer& r)
{
	*this = r;
}

template<typename T>
RefPointer<T>& RefPointer<T>::operator=(const RefPointer& r)
{
	if (this != &r)
	{
		this->m_Data = r.m_Data;
		this->m_Refs = r.m_Refs;
		m_Refs->Increment();
	}
	return *this;
}

template<typename T>
RefPointer<T>::RefPointer(T* object)
	: m_Data(object)
{
	if (!m_Refs)
		m_Refs = new ReferenceCounter;

	m_Refs->Increment();
}

template<typename T>
RefPointer<T>::~RefPointer()
{
	if (m_Refs->Decrement() <= 0)
	{
		delete m_Data;
		m_Data = nullptr;

		delete m_Refs;
		m_Refs = nullptr;

	}
}


//Shitty C version, that i tested for benji
/**
#include <stdio.h>
#include <memory.h>

struct RefCounter
{
	int m_Count;
};

void Increment(int* count)
{
	count++;
}

int Decrement(int* count)
{
	count--;
	return *count;
}

struct RefPointer
{
	struct RefCounter* m_RefCounter;
	void* m_Data;
};

void create_refpointer(struct RefPointer* pRefPointer)
{
	pRefPointer->m_RefCounter = (struct RefCounter*)malloc(4);
	Increment(&pRefPointer->m_RefCounter->m_Count);
	pRefPointer->m_Data = 0;
}

void free_refpointer(struct RefPointer* pRefPointer)
{
	if (Decrement(&pRefPointer->m_RefCounter->m_Count) <= 0)
	{
		free(pRefPointer->m_RefCounter);
		free(pRefPointer->m_Data);
	}
}

int main()
{
	struct RefPointer pointer;
	create_refpointer(&pointer);

	const char* object = "hello world";
	pointer.m_Data = (void*)object;


	free_refpointer(&pointer);

	return 0;
}
*/