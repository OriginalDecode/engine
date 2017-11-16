#include "reflector.h"
Reflector* Reflector::m_Instance = nullptr;
Reflector::Reflector()
{
	registerType<bool>("bool").constructable<bool>().copyable<bool>().compareable<bool>();
	registerType<char>("char").constructable<char>().copyable<char>().compareable<char>();
	registerType<int>("int").constructable<int>().copyable<int>().compareable<int>();
	registerType<unsigned>("unsigned").constructable<unsigned>().copyable<unsigned>().compareable<unsigned>();
	registerType<float>("float").constructable<float>().copyable<float>().compareable<float>();
	registerType<double>("double").constructable<double>().copyable<double>().compareable<double>();
}

