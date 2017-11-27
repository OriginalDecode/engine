#pragma once
#include "Message.h"
class TreeDweller;
struct OnLeftClick : public Message
{
	OnLeftClick() : Message(eMessageType::COLLIDED){}

	OnLeftClick(float rx, float ry, float rz, float cx, float cy, float cz, void* _offset, TreeDweller* player)
		: Message(eMessageType::ON_LEFT_CLICK)
		, ray_dir_x(rx)
		, ray_dir_y(ry)
		, ray_dir_z(rz)
		, camera_pos_x(cx)
		, camera_pos_y(cy)
		, camera_pos_z(cz)
		, m_Player(player)
		, m_Offset(_offset)
	{
	}


	float ray_dir_x;
	float ray_dir_y;
	float ray_dir_z;

	float camera_pos_x;
	float camera_pos_y;
	float camera_pos_z;

	void* m_Offset = nullptr;

	TreeDweller* m_Player = nullptr;

};