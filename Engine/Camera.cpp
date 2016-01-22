#include "Camera.h"
#include <xnamath.h>
namespace Snowblind
{
	CCamera::CCamera(float aWidth, float aHeight, Vector3f aPosition)
	{
		XMMATRIX projection;
		projection = XMMatrixPerspectiveFovLH(XM_PI*0.3f, aWidth / FLOAT(aHeight), 0.1f, 100.f);

		XMFLOAT4X4 projMiddleHand;
		XMStoreFloat4x4(&projMiddleHand, projection);
		myProjectionMatrix.Init(reinterpret_cast<float*>(projMiddleHand.m));

		myOrientation.myMatrix[3] = aPosition.x;
		myOrientation.myMatrix[7] = aPosition.y;
		myOrientation.myMatrix[11] = aPosition.z;
	}

	CCamera::~CCamera()
	{
	}

	void CCamera::SetPosition(const Vector3f& aPosition)
	{
		myOrientation.SetPosition(aPosition);
	}
}