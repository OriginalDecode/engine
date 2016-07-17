#pragma once
class CBaseSystem
{
public:
	CBaseSystem();
	virtual ~CBaseSystem();

	bool HasFinished();
protected:
	bool myHasFinished = false;
	
	

};

