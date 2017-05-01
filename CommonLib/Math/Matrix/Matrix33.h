#pragma once
#include "Matrix44.h"
#include "Matrix33Decl.h"


namespace CommonUtilities
{
	typedef Math::Matrix33<float> Matrix33f;
	namespace Math
	{
#pragma region Constructors

		/****************************\
		|		Constructors	     |
		\****************************/
		template<typename TYPE>
		Matrix33<TYPE>::Matrix33()
		{
			if (myMatrix.Size() > 0)
				myMatrix.RemoveAll();
			//Set everything to 0
			for (unsigned short i = 0; i < 9; i++)
			{
				myMatrix.Add(0);
			}
			//Then set the ones who are supposed to be 1 to 1;
			myMatrix[0] = 1;
			myMatrix[4] = 1;
			myMatrix[8] = 1;
		}


		template<typename TYPE>
		Matrix33<TYPE>::Matrix33(const Matrix33<TYPE>& aMatrix)
		{
			*this = aMatrix;
		}


		template<typename TYPE>
		Matrix33<TYPE>::Matrix33(Matrix44<TYPE> aMatrix)
		{
			myMatrix[0] = aMatrix.myMatrix[0];
			myMatrix[1] = aMatrix.myMatrix[1];
			myMatrix[2] = aMatrix.myMatrix[2];
			myMatrix[3] = aMatrix.myMatrix[4];
			myMatrix[4] = aMatrix.myMatrix[5];
			myMatrix[5] = aMatrix.myMatrix[6];
			myMatrix[6] = aMatrix.myMatrix[8];
			myMatrix[7] = aMatrix.myMatrix[9];
			myMatrix[8] = aMatrix.myMatrix[10];
		}

		template<typename TYPE>
		Matrix33<TYPE>::~Matrix33()
		{
		}

#pragma endregion

#pragma region Functions

		/****************************\
		|		   Functions	     |
		\****************************/

		template<typename TYPE>
		Matrix33<TYPE> Matrix33<TYPE>::CreateRotateAroundX(TYPE aAngleInRadians)
		{
			Matrix33<TYPE> matrix;

			matrix.myMatrix[0] = 1;
			matrix.myMatrix[1] = 0;
			matrix.myMatrix[2] = 0;

			matrix.myMatrix[3] = 0;
			matrix.myMatrix[4] = cos(aAngleInRadians);
			matrix.myMatrix[5] = sin(aAngleInRadians);

			matrix.myMatrix[6] = 0;
			matrix.myMatrix[7] = -sin(aAngleInRadians);
			matrix.myMatrix[8] = cos(aAngleInRadians);

			return matrix;
		}

		template<typename TYPE>
		Matrix33<TYPE> Matrix33<TYPE>::CreateRotateAroundY(TYPE aAngleInRadians)
		{
			Matrix33<TYPE> matrix;

			matrix.myMatrix[0] = cos(aAngleInRadians);
			matrix.myMatrix[1] = 0;
			matrix.myMatrix[2] = -sin(aAngleInRadians);

			matrix.myMatrix[3] = 0;
			matrix.myMatrix[4] = 1;
			matrix.myMatrix[5] = 0;

			matrix.myMatrix[6] = sin(aAngleInRadians);
			matrix.myMatrix[7] = 0;
			matrix.myMatrix[8] = cos(aAngleInRadians);

			return matrix;
		}

		template<typename TYPE>
		Matrix33<TYPE> Matrix33<TYPE>::CreateRotateAroundZ(TYPE aAngleInRadians)
		{
			Matrix33<TYPE> matrix;

			matrix.myMatrix[0] = cos(aAngleInRadians);
			matrix.myMatrix[1] = sin(aAngleInRadians);
			matrix.myMatrix[2] = 0;

			matrix.myMatrix[3] = -sin(aAngleInRadians);
			matrix.myMatrix[4] = cos(aAngleInRadians);
			matrix.myMatrix[5] = 0;

			matrix.myMatrix[6] = 0;
			matrix.myMatrix[7] = 0;
			matrix.myMatrix[8] = 1;

			return matrix;
		}


		template<typename TYPE>
		void Matrix33<TYPE>::Rotate2D(TYPE aRadian)
		{
			Matrix33<TYPE> rotateMatrix = Matrix33::CreateRotateAroundZ(aRadian);
			Matrix33<TYPE> tempMatrix = *this;
			tempMatrix *= rotateMatrix;

			myMatrix[0] = tempMatrix.myMatrix[0];
			myMatrix[1] = tempMatrix.myMatrix[1];
			myMatrix[3] = tempMatrix.myMatrix[3];
			myMatrix[4] = tempMatrix.myMatrix[4];
		}
		template<typename TYPE>
		void Matrix33<TYPE>::SetRotation2d(TYPE aRadian)
		{
			myMatrix[0] = cos(aRadian);
			myMatrix[1] = sin(aRadian);
			myMatrix[3] = -sin(aRadian);
			myMatrix[4] = cos(aRadian);
		}

		template<typename TYPE>
		void Matrix33<TYPE>::SetTranslation(TYPE anX, TYPE anY, TYPE anZ)
		{
			myMatrix[6] = anX;
			myMatrix[7] = anY;
			myMatrix[8] = anZ;
		}

		template<typename TYPE>
		void Matrix33<TYPE>::SetTranslation(const Vector3<TYPE>& aVector3)
		{
			myMatrix[6] = aVector3.x;
			myMatrix[7] = aVector3.y;
			myMatrix[8] = aVector3.z;
		}

		template<typename TYPE>
		Vector3<TYPE> Matrix33<TYPE>::GetTranslation() const
		{
			return Vector3<TYPE>(myMatrix[6], myMatrix[7], myMatrix[8]);
		}


#pragma endregion

#pragma region Operators

#pragma region NormalOperator

		/****************************\
		|		Normal Operators	 |
		\****************************/

		template<typename TYPE>
		const Matrix33<TYPE> operator+(Matrix33<TYPE>& aFirstMatrix, const Matrix33<TYPE>& aSecondMatrix)
		{
			return aFirstMatrix += aSecondMatrix;
		}

		template<typename TYPE>
		const Matrix33<TYPE> operator-(Matrix33<TYPE>& aFirstMatrix, const Matrix33<TYPE>& aSecondMatrix)
		{
			return aFirstMatrix -= aSecondMatrix;
		}

		template<typename TYPE>
		const Matrix33<TYPE> operator*(Matrix33<TYPE>& aFirstMatrix, const Matrix33<TYPE>& aSecondMatrix)
		{
			return aFirstMatrix *= aSecondMatrix;
		}

		template<typename TYPE>
		const Vector3<TYPE> operator*(Vector3<TYPE>& aVector, const Matrix33<TYPE>& aMatrix)
		{
			return aVector *= aMatrix;
		}

		template<typename TYPE>
		Vector2<TYPE> operator*(Vector2<TYPE>& aVector, const Matrix33<TYPE>& aMatrix)
		{
			Vector3<TYPE> vector3(aVector.x, aVector.y, 1);
			vector3 = vector3*aMatrix;
			return Vector2<TYPE>(vector3.x, vector3.y);
		}

#pragma endregion

#pragma region CompoundOperators
		/****************************\
		|		Compound Operators	 |
		\****************************/

		template<typename TYPE>
		Matrix33<TYPE> operator+=(Matrix33<TYPE>& aFirstMatrix, const Matrix33<TYPE>& aSecondMatrix)
		{
			for (int i = 0; i < 9; ++i)
			{
				aFirstMatrix.myMatrix[i] += aSecondMatrix.myMatrix[i];
			}
			return aFirstMatrix;
		}

		template<typename TYPE>
		Matrix33<TYPE> operator-=(Matrix33<TYPE>& aFirstMatrix, const Matrix33<TYPE>& aSecondMatrix)
		{
			for (int i = 0; i < 9; ++i)
			{
				aFirstMatrix.myMatrix[i] -= aSecondMatrix.myMatrix[i];
			}
			return aFirstMatrix;
		}

		template<typename TYPE>
		Matrix33<TYPE> operator*=(Matrix33<TYPE>& aFirstMatrix, const Matrix33<TYPE>& aSecondMatrix)
		{
			Vector3<TYPE> myFirstRow(aFirstMatrix.myMatrix[0], aFirstMatrix.myMatrix[1], aFirstMatrix.myMatrix[2]);
			Vector3<TYPE> mySecondRow(aFirstMatrix.myMatrix[3], aFirstMatrix.myMatrix[4], aFirstMatrix.myMatrix[5]);
			Vector3<TYPE> myThirdRow(aFirstMatrix.myMatrix[6], aFirstMatrix.myMatrix[7], aFirstMatrix.myMatrix[8]);

			Vector3<TYPE> myFirstCollumn(aSecondMatrix.myMatrix[0], aSecondMatrix.myMatrix[3], aSecondMatrix.myMatrix[6]);
			Vector3<TYPE> mySecondCollumn(aSecondMatrix.myMatrix[1], aSecondMatrix.myMatrix[4], aSecondMatrix.myMatrix[7]);
			Vector3<TYPE> myThirdCollumn(aSecondMatrix.myMatrix[2], aSecondMatrix.myMatrix[5], aSecondMatrix.myMatrix[8]);

			aFirstMatrix.myMatrix[0] = Dot(myFirstRow, myFirstCollumn);
			aFirstMatrix.myMatrix[1] = Dot(myFirstRow, mySecondCollumn);
			aFirstMatrix.myMatrix[2] = Dot(myFirstRow, myThirdCollumn);

			aFirstMatrix.myMatrix[3] = Dot(mySecondRow, myFirstCollumn);
			aFirstMatrix.myMatrix[4] = Dot(mySecondRow, mySecondCollumn);
			aFirstMatrix.myMatrix[5] = Dot(mySecondRow, myThirdCollumn);

			aFirstMatrix.myMatrix[6] = Dot(myThirdRow, myFirstCollumn);
			aFirstMatrix.myMatrix[7] = Dot(myThirdRow, mySecondCollumn);
			aFirstMatrix.myMatrix[8] = Dot(myThirdRow, myThirdCollumn);

			return aFirstMatrix;
		}


		template<typename TYPE>
		Vector3<TYPE> operator*=(Vector3<TYPE>& aVector, const Matrix33<TYPE>& aMatrix)
		{

			Vector3<TYPE> myVector;

			Vector3<TYPE> myFirstCollumn(aMatrix.myMatrix[0], aMatrix.myMatrix[3], aMatrix.myMatrix[6]);
			Vector3<TYPE> mySecondCollumn(aMatrix.myMatrix[1], aMatrix.myMatrix[4], aMatrix.myMatrix[7]);
			Vector3<TYPE> myThirdCollumn(aMatrix.myMatrix[2], aMatrix.myMatrix[5], aMatrix.myMatrix[8]);

			myVector.x = Dot(aVector, myFirstCollumn);
			myVector.y = Dot(aVector, mySecondCollumn);
			myVector.z = Dot(aVector, myThirdCollumn);

			return myVector;
		}

		template<typename TYPE>
		Vector2<TYPE> operator*=(Vector2<TYPE>& aVector, const Matrix33<TYPE>& aMatrix)
		{
			Vector2<TYPE> myVector;
			Vector2<TYPE> firstCollumn(aMatrix.myMatrix[0], aMatrix.myMatrix[3]);
			Vector2<TYPE> secondCollumn(aMatrix.myMatrix[1], aMatrix.myMatrix[4]);
			Vector2<TYPE> thirdCollumn(aMatrix.myMatrix[3], aMatrix.myMatrix[5]);

			myVector.x = Dot(aVector, firstCollumn);
			myVector.y = Dot(aVector, secondCollumn);
			//myVector.z = Dot(aVector, thirdCollumn);

			return myVector;
		}

#pragma endregion

#pragma region OtherOperations
		/****************************\
		|		Other Operations     |
		\****************************/

		template<typename TYPE>
		bool operator==(Matrix33<TYPE>& aFirstMatrix, Matrix33<TYPE>& aSecondMatrix)
		{
			for (int i = 0; i < 9; ++i)
			{
				aFirstMatrix.myMatrix[i] = aSecondMatrix.myMatrix[i];
				if (aFirstMatrix.myMatrix[i] != aSecondMatrix.myMatrix[i])
				{
					return false;
				}
			}
			return true;
		}


		template<typename TYPE>
		Matrix33<TYPE>& Matrix33<TYPE>::operator=(const Matrix33<TYPE>& aMatrix)
		{
			myMatrix.Reserve(9);
			for (unsigned short i = 0; i < 9; ++i)
			{
				myMatrix[i] = aMatrix.myMatrix[i];
			}
			return *this;
		}

		template<typename TYPE>
		Matrix33<TYPE> Transpose(const Matrix33<TYPE>& aMatrix)
		{
			Matrix33<TYPE> tempMatrix;
			for (unsigned short i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					tempMatrix.myMatrix[i + (j * 3)] = aMatrix.myMatrix[j + (i * 3)];
				}
			}
			return tempMatrix;
		}

		template<typename TYPE>
		Matrix33<TYPE> Inverse(Matrix33<TYPE> &aMatrix)
		{
			Vector3<TYPE> theTranslation;
			
			theTranslation = aMatrix.GetTranslation();
			theTranslation.x *= -1;
			theTranslation.y *= -1;
			theTranslation.z = 1;

			Matrix33<TYPE> theMatrix(aMatrix);

			theMatrix.SetTranslation(0.0f, 0.0f, 1.0f);
			theMatrix = Transpose(theMatrix);

			theTranslation = theTranslation*theMatrix;
			theMatrix.SetTranslation(theTranslation.x, theTranslation.y, theTranslation.z);

			return theMatrix;
		}

		template<class TYPE>
		Matrix33<TYPE> CreateReflectionMatrixAboutAxis(Vector3<TYPE> reflectionVector)
		{
			Matrix33<TYPE> reflectionMatrix;
			reflectionMatrix.myMatrix[0] = 1 - 2 * (reflectionVector.x*reflectionVector.x);
			reflectionMatrix.myMatrix[1] = -2 * (reflectionVector.x*reflectionVector.y);
			reflectionMatrix.myMatrix[2] = -2 * (reflectionVector.x*reflectionVector.z);

			reflectionMatrix.myMatrix[3] = -2 * (reflectionVector.y*reflectionVector.x);
			reflectionMatrix.myMatrix[4] = 1 - 2 * (reflectionVector.y*reflectionVector.y);
			reflectionMatrix.myMatrix[5] = -2 * (reflectionVector.y*reflectionVector.z);

			reflectionMatrix.myMatrix[6] = -2 * (reflectionVector.z*reflectionVector.x);
			reflectionMatrix.myMatrix[7] = -2 * (reflectionVector.z*reflectionVector.y);
			reflectionMatrix.myMatrix[8] = 1 - 2 * (reflectionVector.z*reflectionVector.z);
			return reflectionMatrix;
		};

#pragma endregion

#pragma endregion

	};
};
namespace CU = CommonUtilities;