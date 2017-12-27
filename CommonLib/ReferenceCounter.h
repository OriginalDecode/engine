#pragma once
class ReferenceCounter
{
public:
	ReferenceCounter() = default;
	void Increment() { ++m_Count; }
	int count() { return m_Count; }
	int Decrement() { return --m_Count; }
private:
	int m_Count = 0;
};
