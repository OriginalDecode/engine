
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
			Matrix44(TYPE mat[16])
				: myMatrix(mat)
			{
			}
			~Matrix44();

			Matrix44& operator=(const Matrix44& aMatrix);

			const TYPE& operator[](const int& anIndex) const;
			TYPE& operator[](const int& anIndex);

			float GetXRotation() const;
			float GetYRotation() const;
			float GetZRotation() const;
			static Matrix44<TYPE> CreateRotateAroundX(const TYPE& aRadian);
			static Matrix44<TYPE> CreateRotateAroundY(const TYPE& aRadian);
			static Matrix44<TYPE> CreateRotateAroundZ(const TYPE& aRadian);
			static Matrix44<TYPE> CreateProjectionMatrixLH(TYPE aNearZ, TYPE aFarZ, TYPE anAspectRatio, TYPE aFoVAngle);

			static Matrix44<TYPE> CreateOrthogonalMatrixLH(TYPE aWidth, TYPE aHeight, TYPE aNearZ, TYPE aFarZ);
			static Matrix44<TYPE> CreateOrthographicMatrixLH(float width, float height, float near_plane, float far_plane);

			static Matrix44<TYPE> CreateScaleMatrix(const Vector4<TYPE>& scale);
			static Matrix44<TYPE> CreateScaleMatrix(const float x, const float y, const float z, const float w);
			void RotateAroundPointX(const CU::Vector3f& point, float radian, float dt);
			void RotateAroundPointY(const CU::Vector3f& point, float radian, float dt);
			void RotateAroundPointZ(const CU::Vector3f& point, float radian, float dt);
			void RotateAroundPointX(const CU::Vector3f& point, float radian);
			void RotateAroundPointY(const CU::Vector3f& point, float radian);
			void RotateAroundPointZ(const CU::Vector3f& point, float radian);

			void SetPerspectiveFOV(float fov, float aspect_ratio);

			void SetOrthographicProjection(float width, float height, float near_plane, float far_plane);

			//Matrix44<TYPE>& operator*=(const Matrix44<TYPE>& rhs);


			union
			{
				__declspec(align(16))TYPE myMatrix[16];
				TYPE mat[4][4];
				Vector4<TYPE> rows[4];
			};


			void ConvertFromCol(const TYPE aColMatrix[16]);
			void InitWithArray(const TYPE aColMatrix[16]);
			static Matrix44<TYPE> Transpose(const Matrix44<TYPE>& mat);

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
			const Vector4<TYPE> GetScale() const;

			void LookAt(const Vector3<TYPE>& eye, const Vector3<TYPE>& target, const Vector3<TYPE>& up);


			void SetX(const Vector3<TYPE>& v);
			void SetY(const Vector3<TYPE>& v);
			void SetZ(const Vector3<TYPE>& v);

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

			bool operator==(const Matrix44<TYPE>& m0);

			const Vector4<TYPE> GetColumn(int index) const;
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
		void Matrix44<TYPE>::SetOrthographicProjection(float width, float height, float near_plane, float far_plane)
		{
			myMatrix[0] = 2.f / width;

			myMatrix[5] = 2.f / height;

			myMatrix[10] = 1.f / (far_plane - near_plane);

			myMatrix[14] = near_plane / (near_plane - far_plane);
			myMatrix[15] = 1.f;
		}

		template<typename TYPE>
		void Matrix44<TYPE>::LookAt(const Vector3<TYPE>& eye, const Vector3<TYPE>& target, const Vector3<TYPE>& up)
		{
			Vector3<TYPE> z = GetNormalized(target - eye);
			Vector3<TYPE> x = GetNormalized(Cross(up, z));
			Vector3<TYPE> y = Cross(z, x);

			mat[0][0] = x.x; mat[0][1] = y.x; mat[0][2] = z.x;
			mat[1][0] = x.y; mat[1][1] = y.y; mat[1][2] = z.y;
			mat[2][0] = x.z; mat[2][1] = y.z; mat[2][2] = z.z;

			mat[3][0] = -Dot(x, eye); 
			mat[3][1] = -Dot(y, eye); 
			mat[3][2] = -Dot(z, eye); 
			mat[3][3] = 1.f;
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetX(const Vector3<TYPE>& v)
		{
		
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetY(const Vector3<TYPE>& v)
		{
			
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetZ(const Vector3<TYPE>& v)
		{
		
		}

		template<typename TYPE>
		const Vector4<TYPE> Matrix44<TYPE>::GetColumn(int index) const
		{
			assert(index < 4 && index >= 0 && "invalid case!");
			Vector4<TYPE> out;
			switch (index)
			{
			case 0:
				return Vector4<TYPE>(myMatrix[0], myMatrix[4], myMatrix[8], myMatrix[12]);
			case 1:
				return Vector4<TYPE>(myMatrix[1], myMatrix[5], myMatrix[9], myMatrix[13]);
			case 2:
				return Vector4<TYPE>(myMatrix[2], myMatrix[6], myMatrix[10], myMatrix[14]);
			case 3:
				return Vector4<TYPE>(myMatrix[3], myMatrix[7], myMatrix[11], myMatrix[15]);
			}
			return Vector4<TYPE>();
		}

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::Transpose(const Matrix44<TYPE>& mat)
		{
			Matrix44<TYPE> result = mat;
			std::swap(result.myMatrix[1], result.myMatrix[4]);
			std::swap(result.myMatrix[2], result.myMatrix[8]);
			std::swap(result.myMatrix[3], result.myMatrix[12]);
			std::swap(result.myMatrix[6], result.myMatrix[9]);
			std::swap(result.myMatrix[7], result.myMatrix[13]);
			std::swap(result.myMatrix[11], result.myMatrix[14]);
			return result;
		}

		template<typename TYPE>
		bool Matrix44<TYPE>::operator==(const Matrix44<TYPE>& m0)
		{
			for (unsigned int i = 0; i < 16; i++)
			{
				if (m0[i] != myMatrix[i])
					return false;
			}

			return true;
		}

		

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
		void CommonUtilities::Math::Matrix44<TYPE>::RotateAroundPointZ(const CU::Vector3f& point, float radian)
		{
			CU::Vector3f original_pos = GetPosition();
			SetPosition(original_pos - point);

			*this = *this * CU::Matrix44f::CreateRotateAroundZ(radian);
			SetPosition(GetPosition() + point);
		}

		template<typename TYPE>
		void CommonUtilities::Math::Matrix44<TYPE>::RotateAroundPointY(const CU::Vector3f& point, float radian)
		{
			CU::Vector3f original_pos = GetPosition();
			SetPosition(original_pos - point);

			*this = *this * CU::Matrix44f::CreateRotateAroundY(radian);
			SetPosition(GetPosition() + point);
		}

		template<typename TYPE>
		void CommonUtilities::Math::Matrix44<TYPE>::RotateAroundPointX(const CU::Vector3f& point, float radian)
		{
			CU::Vector3f original_pos = GetPosition();
			SetPosition(original_pos - point);

			*this = *this * CU::Matrix44f::CreateRotateAroundX(radian);
			SetPosition(GetPosition() + point);
		}

		template<typename TYPE>
		Matrix44<TYPE> CommonUtilities::Math::Matrix44<TYPE>::CreateOrthographicMatrixLH(float width, float height, float near_plane, float far_plane)
		{
 			Matrix44<TYPE> new_matrix;
			new_matrix.SetOrthographicProjection(width, height, near_plane, far_plane);
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

