#pragma once
#include "..\..\DataStructures\GrowingArray.h"
#include "..\Vector\Vector.h"
namespace CommonUtilities
{
	namespace Math
	{
		template<typename TYPE>
		class Matrix44
		{
		public:
			Matrix44();
			Matrix44(const Matrix44<TYPE>& aMatrix);
			~Matrix44();

			Matrix44& operator=(const Matrix44& aMatrix);

			static Matrix44<TYPE> CreateRotateAroundX(TYPE aAngleInRadians);
			static Matrix44<TYPE> CreateRotateAroundY(TYPE aAngleInRadians);
			static Matrix44<TYPE> CreateRotateAroundZ(TYPE aAngleInRadians);

			GrowingArray<TYPE> myMatrix;

			void Rotate3DZ(TYPE aRadian);
			void Rotate3DY(TYPE aRadian);
			void Rotate3DX(TYPE aRadian);

			void SetRotation3dX(TYPE aRadian);
			void SetRotation3dY(TYPE aRadian);
			void SetRotation3dZ(TYPE aRadian);

			void SetTranslation(TYPE anX, TYPE anY, TYPE anZ, TYPE anW);
			void SetTranslation(const Vector4<TYPE>& aVector);
			Vector4<TYPE> GetTranslation() const;


			void SetForward(const Vector4<TYPE>& aVector);
			void SetRight(const Vector4<TYPE>& aVector);
			void SetUp(const Vector4<TYPE>& aVector);
			void SetPosition(const Vector3<TYPE>& aVector);
			Vector4<TYPE> GetForward();
			Vector4<TYPE> GetRight();
			Vector4<TYPE> GetUp();
			Vector3<TYPE> GetPosition();

		private:
		};
	};
};
	namespace CU = CommonUtilities;

