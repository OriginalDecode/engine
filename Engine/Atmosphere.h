#pragma once

class Atmosphere
{
public:
	Atmosphere() = default;
	~Atmosphere();
	void Initiate(float inner_radius, float outer_radius, const CU::Vector3f& position );
	void UpdateBuffer(const graphics::RenderContext& rc, Camera* active_camere);

private:
	struct cbPixel
	{
		CU::Vector4f view_dir;
		CU::Vector4f view_pos;
		CU::Vector4f light_dir;
		CU::Vector4f x;
	} m_PixelStruct;
	IBuffer* m_PixelBuffer = nullptr;
};

