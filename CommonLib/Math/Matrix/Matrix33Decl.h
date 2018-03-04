#pragma once
#include "..\..\DataStructures\GrowingArray.h"
#include "..\Vector\Vector.h"

namespace CommonUtilities
{
	namespace Math
	{
		template <typename TYPE>
		class Matrix33
		{
		public:
			Matrix33();
			Matrix33(const Matrix33<TYPE>& aMatrix);
			Matrix33(Matrix44<TYPE> aMatrix);
			~Matrix33();

			Matrix33& operator=(const Matrix33& aMatrix);

			static Matrix33<TYPE> CreateRotateAroundX(TYPE aAngleInRadians);
			static Matrix33<TYPE> CreateRotateAroundY(TYPE aAngleInRadians);
			static Matrix33<TYPE> CreateRotateAroundZ(TYPE aAngleInRadians);


			TYPE m_Matrix[9];

			void Rotate2D(TYPE aRadian);
			void SetRotation2d(TYPE aRadian);
			void SetTranslation(TYPE anX, TYPE anY, TYPE anZ);
			void SetTranslation(const Vector3<TYPE>& aVector3);
			Vector3<TYPE> GetTranslation() const;
		};
	};
};
namespace CU = CommonUtilities;


