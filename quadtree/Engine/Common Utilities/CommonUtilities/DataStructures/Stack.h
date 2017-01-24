#ifndef STACK_H
#define STACK_H

#include "GrowingArray.h"
#include <assert.h>

template<typename T>
class Stack
{

public:
	Stack();
	Stack(int aSize);
	~Stack();


	T Pop();
	T& Top();
	int Size();
	void Push(const T& aValue);

private:

	CU::GrowingArray<T> myStack;
};


template<typename T>
Stack<T>::Stack()
{
	myStack.Init(16, true);
}

template<typename T>
Stack<T>::Stack(int aSize)
{
	myStack.Init(aSize, true);
}

template<typename T>
Stack<T>::~Stack()
{
}

template<typename T>
T Stack<T>::Pop()
{
	assert(myStack.Size() > 0 && "Stack is empty");

	T temp = myStack.GetLast();
	myStack.RemoveCyclicAtIndex(myStack.Size() - 1);


	return temp;
}

template<typename T>
T& Stack<T>::Top()
{
	assert(myStack.Size() > 0 && "Stack is empty");

	return myStack.GetLast();
}

template<typename T>
int Stack<T>::Size()
{
	return myStack.Size();
}

template<typename T>
void Stack<T>::Push(const T& aValue)
{
	myStack.Add(aValue);
}

#endif