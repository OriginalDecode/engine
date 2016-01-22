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

			const TYPE& operator[](const int& anIndex) const;
			TYPE& operator[](const int& anIndex);

			static Matrix44<TYPE> CreateRotateAroundX(TYPE aAngleInRadians);
			static Matrix44<TYPE> CreateRotateAroundY(TYPE aAngleInRadians);
			static Matrix44<TYPE> CreateRotateAroundZ(TYPE aAngleInRadians);

			static Matrix44<TYPE> RotateX(TYPE anAngle);
			static Matrix44<TYPE> RotateY(TYPE anAngle);
			static Matrix44<TYPE> RotateZ(TYPE anAngle);


			GrowingArray<TYPE> myMatrix;

			

			void SetRotation3dX(TYPE aRadian);
			void SetRotation3dY(TYPE aRadian);
			void SetRotation3dZ(TYPE aRadian);

			void SetRotationX(TYPE aRotationInDegrees);
			void SetRotationY(TYPE aRotationInDegrees);
			void SetRotationZ(TYPE aRotationInDegrees);

			void SetTranslation(TYPE anX, TYPE anY, TYPE anZ, TYPE anW);
			void SetTranslation(const Vector4<TYPE>& aVector);
			Vector4<TYPE> GetTranslation() const;
			Vector3<TYPE> GetPosition() const;

			void SetForward(const Vector4<TYPE>& aVector);
			void SetRight(const Vector4<TYPE>& aVector);
			void SetUp(const Vector4<TYPE>& aVector);
			void SetPosition(const Vector3<TYPE>& aVector);
			Vector4<TYPE> GetForward();
			Vector4<TYPE> GetRight();
			Vector4<TYPE> GetUp();
			Matrix44<TYPE> Inverse(Matrix44<TYPE>& aMatrix);


			void Init(TYPE* aMatrix)
			{
				myMatrix[0] = aMatrix[0];
				myMatrix[1] = aMatrix[1];
				myMatrix[2] = aMatrix[2];
				myMatrix[3] = aMatrix[3];

				myMatrix[4] = aMatrix[4];
				myMatrix[5] = aMatrix[5];
				myMatrix[6] = aMatrix[6];
				myMatrix[7] = aMatrix[7];

				myMatrix[8] = aMatrix[8];
				myMatrix[9] = aMatrix[9];
				myMatrix[10] = aMatrix[10];
				myMatrix[11] = aMatrix[11];

				myMatrix[12] = aMatrix[12];
				myMatrix[13] = aMatrix[13];
				myMatrix[14] = aMatrix[14];
				myMatrix[15] = aMatrix[15];
			}




		private:
			
			enum class RotationType
			{
				Axis_X,
				Axis_Y,
				Axis_Z
			};

			Matrix44<TYPE> Calculate(RotationType rotation, TYPE cos, TYPE sin);

		};

		template<typename TYPE>
		inline const TYPE& CommonUtilities::Math::Matrix44<TYPE>::operator[](const int& anIndex) const
		{
			return myMatrix[anIndex];
		}

		template<typename TYPE>
		inline TYPE& CommonUtilities::Math::Matrix44<TYPE>::operator[](const int& anIndex)
		{
			return myMatrix[anIndex];
		}

	};
};
	namespace CU = CommonUtilities;

