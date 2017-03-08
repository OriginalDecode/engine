
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

			TYPE GetXRotation();
			TYPE GetYRotation();
			TYPE GetZRotation();
			static Matrix44<TYPE> CreateRotateAroundX(const TYPE& aRadian);
			static Matrix44<TYPE> CreateRotateAroundY(const TYPE& aRadian);
			static Matrix44<TYPE> CreateRotateAroundZ(const TYPE& aRadian);
			static Matrix44<TYPE> CreateProjectionMatrixLH(TYPE aNearZ, TYPE aFarZ, TYPE anAspectRatio, TYPE aFoVAngle);
			static Matrix44<TYPE> CreateOrthogonalMatrixLH(TYPE aWidth, TYPE aHeight, TYPE aNearZ, TYPE aFarZ);
			static Matrix44<TYPE> CreateOrthographicMatrixLH(float width, float height, float near_plane, float far_plane);
			static Matrix44<TYPE> CreateScaleMatrix(const Vector4<TYPE>& scale);
			void RotateAroundPointX(const CU::Vector3f& point, float radian, float dt);
			void RotateAroundPointY(const CU::Vector3f& point, float radian, float dt);
			void RotateAroundPointZ(const CU::Vector3f& point, float radian, float dt);


			void SetPerspectiveFOV(float fov, float aspect_ratio);


			union
			{
				TYPE myMatrix[16];
				Vector4<TYPE> rows[4];
			};


			void ConvertFromCol(const TYPE aColMatrix[16]);
			void InitWithArray(const TYPE aColMatrix[16]);

			Vector3<TYPE> GetRadRotations();
			Vector3<TYPE> GetGradRotations();


			void SetRotation3dX(const TYPE& aRadian);
			void SetRotation3dY(const TYPE& aRadian);
			void SetRotation3dZ(const TYPE& aRadian);

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
		Vector3<TYPE> Matrix44<TYPE>::GetGradRotations()
		{
			Vector3<TYPE> output;
			output.x = (-atan2(myMatrix[9], myMatrix[10]))  * (180.f / 3.1415926535f);
			output.y = (atan2(myMatrix[8], sqrt((myMatrix[9] * myMatrix[9]) + (myMatrix[10] * myMatrix[10]))))  * (180.f / 3.1415926535f);
			output.z = (-atan2(myMatrix[4], myMatrix[0])) * (180.f / 3.1415926535f);
			return output;
		}

		template<typename TYPE>
		Vector3<TYPE> Matrix44<TYPE>::GetRadRotations()
		{
			Vector3<TYPE> output;
			output.x = -atan2(myMatrix[9], myMatrix[10]);
			output.y = atan2(myMatrix[8], myMatrix[0]);
			output.z = -atan2(myMatrix[4], myMatrix[0]);
			return output;
		}

		template<typename TYPE>
		void CommonUtilities::Math::Matrix44<TYPE>::RotateAroundPointZ(const CU::Vector3f& point, float radian, float dt)
		{
			CU::Vector3f original_pos = GetPosition();
			SetPosition(original_pos - point);

			*this = *this * CU::Matrix44f::CreateRotateAroundZ(radian * dt);
			SetPosition(GetPosition() + point);
		}

		template<typename TYPE>
		void CommonUtilities::Math::Matrix44<TYPE>::RotateAroundPointY(const CU::Vector3f& point, float radian, float dt)
		{
			CU::Vector3f original_pos = GetPosition();
			SetPosition(original_pos - point);

			*this = *this * CU::Matrix44f::CreateRotateAroundY(radian * dt);
			SetPosition(GetPosition() + point);
		}

		template<typename TYPE>
		void CommonUtilities::Math::Matrix44<TYPE>::RotateAroundPointX(const CU::Vector3f& point, float radian, float dt)
		{
			CU::Vector3f original_pos = GetPosition();
			SetPosition(original_pos - point);

			*this = *this * CU::Matrix44f::CreateRotateAroundX(radian * dt);
			SetPosition(GetPosition() + point);
		}

		template<typename TYPE>
		Matrix44<TYPE> CommonUtilities::Math::Matrix44<TYPE>::CreateOrthographicMatrixLH(float width, float height, float near_plane, float far_plane)
		{
			Matrix44<TYPE> new_matrix;
			new_matrix[0] = 2.f / width;

			new_matrix[5] = 2.f / height;
			
			new_matrix[10] = 1.f / (far_plane - near_plane);

			new_matrix[14] = near_plane / (near_plane - far_plane);
			new_matrix[15] = 1.f;
			return new_matrix;
		}


		template<typename TYPE>
		void CommonUtilities::Math::Matrix44<TYPE>::InitWithArray(const TYPE aColMatrix[16])
		{
			for (int i = 0; i < 16; i++)
			{
				myMatrix[i] = aColMatrix[i];
			}
		}

		template<typename TYPE>
		void CommonUtilities::Math::Matrix44<TYPE>::ConvertFromCol(const TYPE aColMatrix[16])
		{
			myMatrix[1] = aColMatrix[4];
			myMatrix[4] = aColMatrix[1];

			myMatrix[2] = aColMatrix[8];
			myMatrix[8] = aColMatrix[2];

			myMatrix[3] = aColMatrix[12];
			myMatrix[12] = aColMatrix[3];

			myMatrix[6] = aColMatrix[9];
			myMatrix[9] = aColMatrix[6];

			myMatrix[7] = aColMatrix[13];
			myMatrix[13] = aColMatrix[7];

			myMatrix[11] = aColMatrix[14];
			myMatrix[14] = aColMatrix[11];

		}

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

