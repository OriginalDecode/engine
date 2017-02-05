#pragma once
struct BaseComponent
{
	virtual ~BaseComponent() = 0;
	bool m_Updated = false;
};

