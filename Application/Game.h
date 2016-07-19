#pragma once

namespace Snowblind
{
	class CSynchronizer;
};

class CEntityManager;


class CGame
{
public:
	CGame(Snowblind::CSynchronizer* aSynchronizer);
	~CGame();

	void Update(float aDeltaTime);

private:
	Snowblind::CSynchronizer* mySynchronizer;
	CEntityManager* myEntityManager;
};

