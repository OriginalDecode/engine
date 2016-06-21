
#pragma once
#include <assert.h>
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

			static Matrix44<TYPE> CreateRotateAroundX(const TYPE& aRadian);
			static Matrix44<TYPE> CreateRotateAroundY(const TYPE& aRadian);
			static Matrix44<TYPE> CreateRotateAroundZ(const TYPE& aRadian);
			static Matrix44<TYPE> CreateProjectionMatrixLH(TYPE aNearZ, TYPE aFarZ, TYPE anAspectRatio, TYPE aFoVAngle);
			static Matrix44<TYPE> CreateOrthogonalMatrixLH(TYPE aWidth, TYPE aHeight, TYPE aNearZ, TYPE aFarZ);
			TYPE myMatrix[16];

			void SetRotation3dX(const TYPE& aRadian);
			void SetRotation3dY(const TYPE& aRadian);
			void SetRotation3dZ(const TYPE& aRadian);

			void SetRotationX(const TYPE& aRotationInDegrees);
			void SetRotationY(const TYPE& aRotationInDegrees);
			void SetRotationZ(const TYPE& aRotationInDegrees);

			void SetTranslation(const TYPE& anX, const TYPE& anY, const TYPE& anZ, const TYPE& anW);
			void SetTranslation(const Vector4<TYPE>& aVector);
			const Vector4<TYPE> GetTranslation() const;
			const Vector3<TYPE> GetPosition() const;

			void SetForward(const Vector4<TYPE>& aVector);
			void SetRight(const Vector4<TYPE>& aVector);
			void SetUp(const Vector4<TYPE>& aVector);
			void SetPosition(const Vector3<TYPE>& aVector);
			void SetPosition(const Vector4<TYPE>& aVector);

			const Vector4<TYPE> GetForward() const;
			const Vector4<TYPE> GetRight() const;
			const Vector4<TYPE> GetUp() const;
			const Matrix44<TYPE> Inverse(Matrix44<TYPE>& aMatrix);
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
			const Matrix44<TYPE> Calculate(const RotationType& rotation, const TYPE& cos, const TYPE& sin);
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

