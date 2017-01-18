#pragma once
#include "Message.h"
struct OnLeftClick : public Message
{
	OnLeftClick() : Message(eMessageType::COLLIDED){}

	OnLeftClick(float rx, float ry, float rz, float cx, float cy, float cz)
		: Message(eMessageType::ON_LEFT_CLICK)
		, ray_dir_x(rx)
		, ray_dir_y(ry)
		, ray_dir_z(rz)
		, camera_pos_x(cx)
		, camera_pos_y(cy)
		, camera_pos_z(cz)
	{
	}


	float ray_dir_x;
	float ray_dir_y;
	float ray_dir_z;

	float camera_pos_x;
	float camera_pos_y;
	float camera_pos_z;

};