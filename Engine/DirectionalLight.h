#pragma once

namespace Snowblind
{
	class CDirectionalLight
	{
	public:

		CDirectionalLight();
		~CDirectionalLight();

		void Initiate(const CU::Vector3f& aDirection, const CU::Vector3f& aPosition, const CU::Vector4f& aColor);

		void SetColor(const CU::Vector4f& aColor);
		const CU::Vector4f& GetColor();

		void SetDirection(const CU::Vector3f& aDirection);
		const CU::Vector3f& GetDirection();
		const CU::Vector3f& GetOriginalDirection();

		void SetPosition(const CU::Vector3f& aPosition);
		const CU::Vector3f& GetPosition();
		const CU::Vector3f& GetOriginalPosition();

		void Update();

	private:

		CU::Matrix44f myOrientation;
		CU::Vector4f myColor;
		CU::Vector3f myDirection;
		CU::Vector3f myOriginalDirection;
		CU::Vector3f myOriginalPosition;
	};

	__forceinline const CU::Vector3f& CDirectionalLight::GetDirection()
	{
		return myDirection;
	}

	__forceinline const CU::Vector3f& CDirectionalLight::GetOriginalDirection()
	{
		return myOriginalDirection;
	}

	__forceinline const CU::Vector4f& CDirectionalLight::GetColor()
	{
		return myColor;
	}

	__forceinline const CU::Vector3f& CDirectionalLight::GetPosition()
	{
		return myOrientation.GetPosition();
	}

	__forceinline const CU::Vector3f& CDirectionalLight::GetOriginalPosition()
	{
		return myOriginalPosition;
	}

};