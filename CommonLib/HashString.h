#pragma once
#include "../standard_datatype.hpp"
#include "DataStructures/Hashmap/Hash.h"
#include <assert.h>

namespace cl
{

	template<s32 _SIZE = 30>
	class CHashString
	{
	public:
		CHashString() { };

		CHashString(const CHashString& copy)
		{
			operator=(copy);
		}

		CHashString(const char* literal)
		{
			operator=(literal);
		}

		CHashString(u64 hash)
			:m_HashValue(hash) 
		{ };

		CHashString operator+(const CHashString &aString) const;
		CHashString operator+(const float aFloat) const;
		CHashString operator+(const double aDouble) const;
		CHashString operator+(const int aInt) const;
		CHashString operator+(const char* aLiteralString) const;
		CHashString operator+(const char aChar) const;

		CHashString& operator+=(const CHashString &aString);
		CHashString& operator+=(const float aFloat);
		CHashString& operator+=(const double aDouble);
		CHashString& operator+=(const int aInt);
		CHashString& operator+=(const char* aLiteralString);
		CHashString& operator+=(const char aChar);

		bool operator==(const CHashString &aString) const;
		bool operator!=(const CHashString& string) const;
		void operator=(const CHashString &aString);
		void operator=(const char* aLiteralString); //assert

		char operator[](const s32 index) const;

		s32 find(const char* literal) const;
		s32 find(const CHashString* string) const;
		bool contains(const char* literal) const;

		s32 length() const;
		CHashString substr(const s32 start_index, const s32 len) const;
		const char* c_str() const;
		u64 GetHash() const;

	private:

		s32 Horspool(const char* pattern) const;

		u64 m_HashValue = 0;
		s32 m_Length = 0;
		char m_String[_SIZE + 1];

	};

	template<s32 _SIZE /*= 30*/>
	bool cl::CHashString<_SIZE>::operator!=(const CHashString& string) const
	{
		return m_HashValue != string.GetHash();
	}


	template< s32 _SIZE /*= 30*/>
	u64 CHashString<_SIZE>::GetHash() const
	{
		return m_HashValue;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE> CHashString<_SIZE>::substr(const s32 start_index, const s32 len) const
	{
		assert(start_index + len <= _SIZE && "Function: SubStr(), (aIndex + aLength) exceeded Max Size limit!");
		String newString;

		for (int i = 0; i < len; ++i)
		{
			newString += myData[i + start_index];
		}

		newString.myData[len] = '\0';

		return newString;
	}

	template< s32 _SIZE /*= 30*/>
	s32 CHashString<_SIZE>::find(const CHashString* string) const
	{
		return find(aString->c_str());
	}

	template<s32 _SIZE /*= 30*/>
	bool CHashString<_SIZE>::contains(const char* string) const
	{
		return (Horspool(string) >= 0);
	}

	template< s32 _SIZE /*= 30*/>
	s32 CHashString<_SIZE>::find(const char* literal) const
	{
		return Horspool(literal);
	}

	template< s32 _SIZE /*= 30*/>
	char CHashString<_SIZE>::operator[](const s32 index) const
	{
		assert(index <= _SIZE && "String operator[] : aIndex was larger than string Size!");
		assert(index >= 0 && "String operator[] : aIndex was smaller than 0!");

		return m_String[index];
	}

	template< s32 _SIZE /*= 30*/>
	void CHashString<_SIZE>::operator=(const char* aLiteralString)
	{
		s32 len = strlen(aLiteralString);
		assert(len <= _SIZE && "Tried to copy a string larger than Size of current string!");

		for (s32 i = 0; i < _SIZE; ++i)
		{
			m_String[i] = aLiteralString[i];
		}
		m_Length = strlen(aLiteralString);
		m_HashValue = Hash(aLiteralString);
	}

	template< s32 _SIZE /*= 30*/>
	void CHashString<_SIZE>::operator=(const CHashString &aString)
	{
		assert(aString.length() <= _SIZE && "Tried to copy a string larger than Size of current string!");

		for (int i = 0; i < _SIZE; ++i)
		{
			m_String[i] = aString[i];
		}
		m_HashValue = aString.GetHash();
		m_Length = aString.length();
	}

	template< s32 _SIZE /*= 30*/>
	bool CHashString<_SIZE>::operator==(const CHashString &aString) const
	{
		return m_HashValue == aString.GetHash();
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE>& CHashString<_SIZE>::operator+=(const char* aLiteralString)
	{
		assert(m_Length + strlen(aLiteralString) <= _SIZE && "String length exceeded with += operator");

		int len = strlen(aLiteralString);
		for (int i = m_Length; i < len + m_Length; ++i)
		{
			m_String[i] = aLiteralString[i - m_Length];
		}
		m_Length += strlen(aLiteralString);
		m_String[m_Length] = '\0';
		
		m_HashValue = Hash(m_String);

		return *this;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE>& CHashString<_SIZE>::operator+=(const char aChar)
	{
		assert(m_Length + 1 <= _SIZE && "String length exceeded with += operator");
		m_String[m_Length] = aChar;
		++m_Length;
		m_HashValue = Hash(m_String);
		return *this;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE>& CHashString<_SIZE>::operator+=(const int aInt)
	{
		char buffer[_SIZE];
		sprintf_s(buffer, "%d", aInt);
		operator+=(buffer);

		return *this;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE>& CHashString<_SIZE>::operator+=(const double aDouble)
	{
		char buffer[_SIZE];
		sprintf_s(buffer, "%f", aDouble);
		operator+=(buffer);

		return *this;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE>& CHashString<_SIZE>::operator+=(const float aFloat)
	{
		char buffer[_SIZE];
		sprintf_s(buffer, "%f", aFloat);
		operator+=(buffer);

		return *this;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE>& CHashString<_SIZE>::operator+=(const CHashString &aString)
	{
		s32 len = aString.length();
		for (int i = m_Length; i < len + m_Length; ++i)
		{
			m_String[i] = aString[i - m_Length];
		}
		m_Length += len;
		m_String[m_Length] = '\0';
		m_HashValue = Hash(m_String);
		return *this;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE> CHashString<_SIZE>::operator+(const char aChar) const
	{
		CHashString str(*this);
		str += aChar;
		return str;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE> CHashString<_SIZE>::operator+(const char* aLiteralString) const
	{
		CHashString str(*this);
		str += aLiteralString;
		return str;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE> CHashString<_SIZE>::operator+(const int aInt) const
	{
		CHashString str(*this);
		str += aInt;
		return str;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE> CHashString<_SIZE>::operator+(const double aDouble) const
	{
		CHashString str(*this);
		str += aDouble;
		return str;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE> CHashString<_SIZE>::operator+(const float aFloat) const
	{
		CHashString str(*this);
		str += aFloat;
		return str;
	}

	template< s32 _SIZE /*= 30*/>
	CHashString<_SIZE> CHashString<_SIZE>::operator+(const CHashString &aString) const
	{
		CHashString str(*this);
		str += aString;
		return str;
	}

	template< s32 _SIZE /*= 30*/>
	s32 CHashString<_SIZE>::length() const
	{
		return m_Length;
	}

	template< s32 _SIZE /*= 30*/>
	s32 CHashString<_SIZE>::Horspool(const char* pattern) const
	{
		u64 table[UCHAR_MAX + 1];
		s32 pattern_size = strlen(pattern);
	
		for (s32 i = 0; i <= UCHAR_MAX; ++i)
		{
			table[i] = pattern_size;
		}

		for (int i = 0; i < pattern_size; ++i)
		{
			table[pattern[i]] = pattern_size - i - 1;
		}


		int textSize = strlen(m_String);
		if (pattern_size > textSize)
		{
			return -1;
		}

		int k = pattern_size - 1;

		while (k < textSize)
		{
			int j = pattern_size - 1;
			int i = k;

			while (j >= 0 && m_String[i] == pattern[j])
			{
				j--;
				i--;
			}
			if (j == -1)
			{
				return i + 1;
			}
			k += table[m_String[k]];
		}
		return -1;
	}

	template< s32 _SIZE>
	const char* CHashString<_SIZE>::c_str() const
	{
		return m_String;
	}

	typedef CHashString<128> HashString;
};
