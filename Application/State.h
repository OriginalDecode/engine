#pragma once


class State
{
public:

	State() = default;

	virtual void Update(float dt) = 0;
	virtual void Render(bool render_through = false) = 0;
	
private:

};
