#pragma once
#include "Matrix44Decl.h"
typedef CU::Math::Matrix44<float> Matrix44f;
namespace CommonUtilities
{
	namespace Math
	{
#pragma region Constructors
		/****************************\
		|		Constructors	     |
		\****************************/
		template<typename TYPE>
		Matrix44<TYPE>::Matrix44()
		{
			if (myMatrix.Size() > 0)
				myMatrix.RemoveAll();
			//Set everything to 0
			for (unsigned short i = 0; i < 16; i++)
			{
				if (i == 0 || i == 5 || i == 10 || i == 15)
					myMatrix.Add(1);
				else
					myMatrix.Add(0);
			}
			//Then set the ones who are supposed to be 1 to 1;
			/*myMatrix[0] = 1;
			myMatrix[5] = 1;
			myMatrix[10] = 1;
			myMatrix[15] = 1;*/
		}


		template<typename TYPE>
		Matrix44<TYPE>::Matrix44(const Matrix44<TYPE>& aMatrix)
		{
			*this = aMatrix;
		}


#pragma endregion
		template<typename TYPE>
		Matrix44<TYPE>::~Matrix44()
		{
		}
#pragma region Functions
		/****************************\
		|		   Functions	     |
		\****************************/

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::Calculate(RotationType rotation, TYPE someCos, TYPE someSin)
		{
			Matrix44<TYPE> matrix;

			switch (rotation)
			{
			case RotationType::Axis_X:

				matrix.myMatrix[0] = 1;
				matrix.myMatrix[1] = 0;
				matrix.myMatrix[2] = 0;
				matrix.myMatrix[3] = 0;

				matrix.myMatrix[4] = 0;
				matrix.myMatrix[5] = someCos;
				matrix.myMatrix[6] = someSin;
				matrix.myMatrix[7] = 0;

				matrix.myMatrix[8] = 0;
				matrix.myMatrix[9] = -someSin;
				matrix.myMatrix[10] = someCos;
				matrix.myMatrix[11] = 0;

				matrix.myMatrix[12] = 0;
				matrix.myMatrix[13] = 0;
				matrix.myMatrix[14] = 0;
				matrix.myMatrix[15] = 1;
				break;

			case RotationType::Axis_Y:

				matrix.myMatrix[0] = someCos;
				matrix.myMatrix[1] = 0;
				matrix.myMatrix[2] = -someSin;
				matrix.myMatrix[3] = 0;

				matrix.myMatrix[4] = 0;
				matrix.myMatrix[5] = 1;
				matrix.myMatrix[6] = 0;
				matrix.myMatrix[7] = 0;

				matrix.myMatrix[8] = someSin;
				matrix.myMatrix[9] = 0;
				matrix.myMatrix[10] = someCos;
				matrix.myMatrix[11] = 0;

				matrix.myMatrix[12] = 0;
				matrix.myMatrix[13] = 0;
				matrix.myMatrix[14] = 0;
				matrix.myMatrix[15] = 1;

				break;

			case RotationType::Axis_Z:

				matrix.myMatrix[0] = someCos;
				matrix.myMatrix[1] = someSin;
				matrix.myMatrix[2] = 0;
				matrix.myMatrix[3] = 0;

				matrix.myMatrix[4] = -someSin;
				matrix.myMatrix[5] = someCos;
				matrix.myMatrix[6] = 0;
				matrix.myMatrix[7] = 0;

				matrix.myMatrix[8] = 0;
				matrix.myMatrix[9] = 0;
				matrix.myMatrix[10] = 1;
				matrix.myMatrix[11] = 0;

				matrix.myMatrix[12] = 0;
				matrix.myMatrix[13] = 0;
				matrix.myMatrix[14] = 0;
				matrix.myMatrix[15] = 1;
				break;

			}

			return matrix;

		}

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::CreateRotateAroundX(TYPE aAngleInRadians)
		{
			Matrix44<TYPE> matrix;

			TYPE _cos = cos(aAngleInRadians);
			TYPE _sin = sin(aAngleInRadians);

			return matrix.Calculate(RotationType::Axis_X, _cos,_sin);
		}

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::CreateRotateAroundY(TYPE aAngleInRadians)
		{
			Matrix44<TYPE> matrix;

			TYPE _cos = cos(aAngleInRadians);
			TYPE _sin = sin(aAngleInRadians);

			return matrix.Calculate(RotationType::Axis_Y, _cos, _sin);
		}

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::CreateRotateAroundZ(TYPE aAngleInRadians)
		{
			Matrix44<TYPE> matrix;

			TYPE _cos = cos(aAngleInRadians);
			TYPE _sin = sin(aAngleInRadians);

			return matrix.Calculate(RotationType::Axis_Z,_cos, _sin);
		}

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::RotateX(TYPE anAngle)
		{
			Matrix44<TYPE> matrix;

			TYPE toRad = 3.14159f / 180.f;

			TYPE _cos = cos(toRad * anAngle);
			TYPE _sin = sin(toRad * anAngle);

			return matrix.Calculate(RotationType::Axis_X, _cos, _sin);
		}

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::RotateY(TYPE anAngle)
		{
			Matrix44<TYPE> matrix;
			TYPE toRad = 3.14159f /180.f;

			TYPE _cos = cos(toRad * anAngle);
			TYPE _sin = sin(toRad * anAngle);

			return matrix.Calculate(RotationType::Axis_Y, _cos, _sin);
		}

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::RotateZ(TYPE anAngle)
		{
			Matrix44<TYPE> matrix;

			TYPE toRad = 3.14159f / 180.f;

			TYPE _cos = cos(toRad * anAngle);
			TYPE _sin = sin(toRad * anAngle);

			return matrix.Calculate(RotationType::Axis_Z, _cos, _sin);
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetRotation3dX(TYPE aRadian)
		{
			Matrix44<TYPE> rotationMatrix = Matrix44::CreateRotateAroundX(aRadian);
			Matrix44<TYPE> tempMatrix = *this;
			tempMatrix = rotationMatrix * tempMatrix;

			myMatrix[0] = tempMatrix.myMatrix[0];
			myMatrix[1] = tempMatrix.myMatrix[1];
			myMatrix[2] = tempMatrix.myMatrix[2];

			myMatrix[4] = tempMatrix.myMatrix[4];
			myMatrix[5] = tempMatrix.myMatrix[5];
			myMatrix[6] = tempMatrix.myMatrix[6];

			myMatrix[8] = tempMatrix.myMatrix[8];
			myMatrix[9] = tempMatrix.myMatrix[9];
			myMatrix[10] = tempMatrix.myMatrix[10];
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetRotation3dY(TYPE aRadian)
		{
			Matrix44<TYPE> rotationMatrix = Matrix44::CreateRotateAroundY(aRadian);
			Matrix44<TYPE> tempMatrix = *this;
			tempMatrix = rotationMatrix * tempMatrix;

			myMatrix[0] = tempMatrix.myMatrix[0];
			myMatrix[1] = tempMatrix.myMatrix[1];
			myMatrix[2] = tempMatrix.myMatrix[2];

			myMatrix[4] = tempMatrix.myMatrix[4];
			myMatrix[5] = tempMatrix.myMatrix[5];
			myMatrix[6] = tempMatrix.myMatrix[6];

			myMatrix[8] = tempMatrix.myMatrix[8];
			myMatrix[9] = tempMatrix.myMatrix[9];
			myMatrix[10] = tempMatrix.myMatrix[10];
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetRotation3dZ(TYPE aRadian)
		{
			Matrix44<TYPE> rotationMatrix = Matrix44::CreateRotateAroundZ(aRadian);
			Matrix44<TYPE> tempMatrix = *this;
			tempMatrix = rotationMatrix * tempMatrix;


			myMatrix[0] = tempMatrix.myMatrix[0];
			myMatrix[1] = tempMatrix.myMatrix[1];
			myMatrix[2] = tempMatrix.myMatrix[2];

			myMatrix[4] = tempMatrix.myMatrix[4];
			myMatrix[5] = tempMatrix.myMatrix[5];
			myMatrix[6] = tempMatrix.myMatrix[6];

			myMatrix[8] = tempMatrix.myMatrix[8];
			myMatrix[9] = tempMatrix.myMatrix[9];
			myMatrix[10] = tempMatrix.myMatrix[10];
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetTranslation(TYPE anX, TYPE anY, TYPE anZ, TYPE anW)
		{
			myMatrix[12] = anX;
			myMatrix[13] = anY;
			myMatrix[14] = anZ;
			myMatrix[15] = anW;
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetRotationX(TYPE aRotationInDegrees)
		{
			Matrix44<TYPE> rotationMatrix = Matrix44::RotateX(aRotationInDegrees);
			Matrix44<TYPE> tempMatrix = *this;
			tempMatrix = rotationMatrix * tempMatrix;

			myMatrix[0] = tempMatrix.myMatrix[0];
			myMatrix[1] = tempMatrix.myMatrix[1];
			myMatrix[2] = tempMatrix.myMatrix[2];

			myMatrix[4] = tempMatrix.myMatrix[4];
			myMatrix[5] = tempMatrix.myMatrix[5];
			myMatrix[6] = tempMatrix.myMatrix[6];

			myMatrix[8] = tempMatrix.myMatrix[8];
			myMatrix[9] = tempMatrix.myMatrix[9];
			myMatrix[10] = tempMatrix.myMatrix[10];
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetRotationY(TYPE aRotationInDegrees)
		{
			Matrix44<TYPE> rotationMatrix = Matrix44::RotateY(aRotationInDegrees);
			Matrix44<TYPE> tempMatrix = *this;
			tempMatrix = rotationMatrix * tempMatrix;

			myMatrix[0] = tempMatrix.myMatrix[0];
			myMatrix[1] = tempMatrix.myMatrix[1];
			myMatrix[2] = tempMatrix.myMatrix[2];

			myMatrix[4] = tempMatrix.myMatrix[4];
			myMatrix[5] = tempMatrix.myMatrix[5];
			myMatrix[6] = tempMatrix.myMatrix[6];

			myMatrix[8] = tempMatrix.myMatrix[8];
			myMatrix[9] = tempMatrix.myMatrix[9];
			myMatrix[10] = tempMatrix.myMatrix[10];
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetRotationZ(TYPE aRotationInDegrees)
		{
			Matrix44<TYPE> rotationMatrix = Matrix44::RotateZ(aRotationInDegrees);
			Matrix44<TYPE> tempMatrix = *this;
			tempMatrix = rotationMatrix * tempMatrix;


			myMatrix[0] = tempMatrix.myMatrix[0];
			myMatrix[1] = tempMatrix.myMatrix[1];
			myMatrix[2] = tempMatrix.myMatrix[2];

			myMatrix[4] = tempMatrix.myMatrix[4];
			myMatrix[5] = tempMatrix.myMatrix[5];
			myMatrix[6] = tempMatrix.myMatrix[6];

			myMatrix[8] = tempMatrix.myMatrix[8];
			myMatrix[9] = tempMatrix.myMatrix[9];
			myMatrix[10] = tempMatrix.myMatrix[10];
		}


		template<typename TYPE>
		void Matrix44<TYPE>::SetTranslation(const Vector4<TYPE>& aVector)
		{
			myMatrix[12] = aVector.X;
			myMatrix[13] = aVector.Y;
			myMatrix[14] = aVector.Z;
			myMatrix[15] = aVector.W;
		}

		template<typename TYPE>
		Vector4<TYPE> Matrix44<TYPE>::GetTranslation() const
		{
			return Vector4<TYPE>(myMatrix[12], myMatrix[13], myMatrix[14], myMatrix[15]);
		}

		template<typename TYPE>
		Vector3<TYPE> Matrix44<TYPE>::GetPosition() const
		{
			return Vector3<TYPE>(myMatrix[12], myMatrix[13], myMatrix[14]);
		}


		template<typename TYPE>
		Vector4<TYPE> Matrix44<TYPE>::GetRight()
		{
			return Vector4<TYPE>(myMatrix[0], myMatrix[1], myMatrix[2], myMatrix[3]);
		}

		template<typename TYPE>
		Vector4<TYPE> Matrix44<TYPE>::GetUp()
		{
			return Vector4<TYPE>(myMatrix[4], myMatrix[5], myMatrix[6], myMatrix[7]);
		}

		template<typename TYPE>
		Vector4<TYPE> Matrix44<TYPE>::GetForward()
		{
			return Vector4<TYPE>(myMatrix[8], myMatrix[9], myMatrix[10], myMatrix[11]);
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetPosition(const Vector3<TYPE>& aVector)
		{
			myMatrix[12] = aVector.x;
			myMatrix[13] = aVector.y;
			myMatrix[14] = aVector.z;
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetRight(const Vector4<TYPE>& aVector)
		{
			myMatrix[0] = aVector.x;
			myMatrix[1] = aVector.y;
			myMatrix[2] = aVector.z;
			myMatrix[3] = aVector.w;
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetUp(const Vector4<TYPE>& aVector)
		{
			myMatrix[4] = aVector.x;
			myMatrix[5] = aVector.y;
			myMatrix[6] = aVector.z;
			myMatrix[7] = aVector.w;
		}

		template<typename TYPE>
		void Matrix44<TYPE>::SetForward(const Vector4<TYPE>& aVector)
		{
			myMatrix[8] = aVector.x;
			myMatrix[9] = aVector.y;
			myMatrix[10] = aVector.z;
			myMatrix[11] = aVector.w;
		}

		template<typename TYPE>
		Matrix44<TYPE> Inverse(Matrix44<TYPE> &aMatrix)
		{
			Vector4<TYPE> theTranslation;

			theTranslation = aMatrix.GetTranslation();
			theTranslation.x *= -1;
			theTranslation.y *= -1;
			theTranslation.z *= -1;
			theTranslation.w = 1;

			Matrix44<TYPE> theMatrix(aMatrix);

			theMatrix.SetTranslation(0.0f, 0.0f, 0.0f, 1.0f);
			theMatrix = Transpose(theMatrix);

			theTranslation = theTranslation*theMatrix;
			theMatrix.SetTranslation(theTranslation.x, theTranslation.y, theTranslation.z, theTranslation.w);

			return theMatrix;
		}


#pragma endregion

#pragma region Operators
		/****************************\
		|		Normal Operators	 |
		\****************************/

		template<typename TYPE>
		const Matrix44<TYPE> operator+(const Matrix44<TYPE>& aFirstMatrix, const Matrix44<TYPE>& aSecondMatrix)
		{
			Matrix44<TYPE> tempMatrix(aFirstMatrix);
			return tempMatrix += aSecondMatrix;
		}

		template<typename TYPE>
		const Matrix44<TYPE> operator-(const Matrix44<TYPE>& aFirstMatrix, const Matrix44<TYPE>& aSecondMatrix)
		{
			Matrix44<TYPE> tempMatrix(aFirstMatrix);
			return tempMatrix -= aSecondMatrix;
		}

		template<typename TYPE>
		const Matrix44<TYPE> operator*(const Matrix44<TYPE>& aFirstMatrix, const Matrix44<TYPE>& aSecondMatrix)
		{
			Matrix44<TYPE> tempMatrix(aFirstMatrix);
			return tempMatrix *= aSecondMatrix;
		}

		template<typename TYPE>
		const Vector4<TYPE> operator*(const Vector4<TYPE>& aVector, const Matrix44<TYPE>& aMatrix)
		{
			Vector4<TYPE> tempVector(aVector);
			return tempVector *= aMatrix;
		}

		template<typename TYPE>
		Vector3<TYPE> operator*(Vector3<TYPE>& aVector, const Matrix44<TYPE>& aMatrix)
		{
			Vector4<TYPE> vector4(aVector.x, aVector.y, aVector.z, 1);
			vector4 = vector4*aMatrix;
			return Vector3<TYPE>(vector4.x, vector4.y, vector4.z);
		}

		/****************************\
		|		Compound Operators	 |
		\****************************/

		template<typename TYPE>
		Matrix44<TYPE> operator+=(Matrix44<TYPE>& aFirstMatrix, const Matrix44<TYPE>& aSecondMatrix)
		{
			for (int i = 0; i < 16; ++i)
			{
				aFirstMatrix.myMatrix[i] += aSecondMatrix.myMatrix[i];
			}
			return aFirstMatrix;
		}

		template<typename TYPE>
		Matrix44<TYPE> operator-=(Matrix44<TYPE>& aFirstMatrix, const Matrix44<TYPE>& aSecondMatrix)
		{
			for (int i = 0; i < 16; ++i)
			{
				aFirstMatrix.myMatrix[i] -= aSecondMatrix.myMatrix[i];
			}
			return aFirstMatrix;
		}

		template<typename TYPE>
		Matrix44<TYPE> operator*=(Matrix44<TYPE>& aFirstMatrix, const Matrix44<TYPE>& aSecondMatrix)
		{
			Vector4<TYPE> myFirstRow(aFirstMatrix.myMatrix[0], aFirstMatrix.myMatrix[1], aFirstMatrix.myMatrix[2], aFirstMatrix.myMatrix[3]);
			Vector4<TYPE> mySecondRow(aFirstMatrix.myMatrix[4], aFirstMatrix.myMatrix[5], aFirstMatrix.myMatrix[6], aFirstMatrix.myMatrix[7]);
			Vector4<TYPE> myThirdRow(aFirstMatrix.myMatrix[8], aFirstMatrix.myMatrix[9], aFirstMatrix.myMatrix[10], aFirstMatrix.myMatrix[11]);
			Vector4<TYPE> myFourthRow(aFirstMatrix.myMatrix[12], aFirstMatrix.myMatrix[13], aFirstMatrix.myMatrix[14], aFirstMatrix.myMatrix[15]);

			Vector4<TYPE> myFirstCollumn(aSecondMatrix.myMatrix[0], aSecondMatrix.myMatrix[4], aSecondMatrix.myMatrix[8], aSecondMatrix.myMatrix[12]);
			Vector4<TYPE> mySecondCollumn(aSecondMatrix.myMatrix[1], aSecondMatrix.myMatrix[5], aSecondMatrix.myMatrix[9], aSecondMatrix.myMatrix[13]);
			Vector4<TYPE> myThirdCollumn(aSecondMatrix.myMatrix[2], aSecondMatrix.myMatrix[6], aSecondMatrix.myMatrix[10], aSecondMatrix.myMatrix[14]);
			Vector4<TYPE> myFourthCollumn(aSecondMatrix.myMatrix[3], aSecondMatrix.myMatrix[7], aSecondMatrix.myMatrix[11], aSecondMatrix.myMatrix[15]);

			aFirstMatrix.myMatrix[0] = Dot(myFirstRow, myFirstCollumn);
			aFirstMatrix.myMatrix[1] = Dot(myFirstRow, mySecondCollumn);
			aFirstMatrix.myMatrix[2] = Dot(myFirstRow, myThirdCollumn);
			aFirstMatrix.myMatrix[3] = Dot(myFirstRow, myFourthCollumn);

			aFirstMatrix.myMatrix[4] = Dot(mySecondRow, myFirstCollumn);
			aFirstMatrix.myMatrix[5] = Dot(mySecondRow, mySecondCollumn);
			aFirstMatrix.myMatrix[6] = Dot(mySecondRow, myThirdCollumn);
			aFirstMatrix.myMatrix[7] = Dot(mySecondRow, myFourthCollumn);

			aFirstMatrix.myMatrix[8] = Dot(myThirdRow, myFirstCollumn);
			aFirstMatrix.myMatrix[9] = Dot(myThirdRow, mySecondCollumn);
			aFirstMatrix.myMatrix[10] = Dot(myThirdRow, myThirdCollumn);
			aFirstMatrix.myMatrix[11] = Dot(myThirdRow, myFourthCollumn);

			aFirstMatrix.myMatrix[12] = Dot(myFourthRow, myFirstCollumn);
			aFirstMatrix.myMatrix[13] = Dot(myFourthRow, mySecondCollumn);
			aFirstMatrix.myMatrix[14] = Dot(myFourthRow, myThirdCollumn);
			aFirstMatrix.myMatrix[15] = Dot(myFourthRow, myFourthCollumn);

			return aFirstMatrix;
		}


		template<typename TYPE>
		Vector4<TYPE>& operator*=(Vector4<TYPE>& aVector, const Matrix44<TYPE>& aMatrix)
		{

			Vector4<TYPE> myFirstCollumn(aMatrix.myMatrix[0], aMatrix.myMatrix[4], aMatrix.myMatrix[8], aMatrix.myMatrix[12]);
			Vector4<TYPE> mySecondCollumn(aMatrix.myMatrix[1], aMatrix.myMatrix[5], aMatrix.myMatrix[9], aMatrix.myMatrix[13]);
			Vector4<TYPE> myThirdCollumn(aMatrix.myMatrix[2], aMatrix.myMatrix[6], aMatrix.myMatrix[10], aMatrix.myMatrix[14]);
			Vector4<TYPE> myFourthCollumn(aMatrix.myMatrix[3], aMatrix.myMatrix[7], aMatrix.myMatrix[11], aMatrix.myMatrix[15]);
			
			Vector4<TYPE> vector = aVector;
			
			aVector.x = Dot(vector, myFirstCollumn);
			aVector.y = Dot(vector, mySecondCollumn);
			aVector.z = Dot(vector, myThirdCollumn);
			aVector.w = Dot(vector, myFourthCollumn);

			return aVector;
		}


		/****************************\
		|		Other Operations     |
		\****************************/

		template<typename TYPE>
		bool operator==(Matrix44<TYPE>& aFirstMatrix, Matrix44<TYPE>& aSecondMatrix)
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
		Matrix44<TYPE>& Matrix44<TYPE>::operator=(const Matrix44<TYPE>& aMatrix)
		{
			if (myMatrix.Size() > 0)
				myMatrix.RemoveAll();

			for (unsigned short i = 0; i < 16; ++i)
			{
				myMatrix.Add(aMatrix.myMatrix[i]);
			}
			return *this;
		}

		template<typename TYPE>
		Matrix44<TYPE> Transpose(const Matrix44<TYPE>& aMatrix)
		{
			Matrix44<TYPE> tempMatrix;
			for (unsigned short i = 0; i < 4; ++i)
			{
				for (unsigned short j = 0; j < 4; ++j)
				{
					tempMatrix.myMatrix[i + (j * 4)] = aMatrix.myMatrix[j + (i * 4)];
				}
			}
			return tempMatrix;
		}

		template<typename TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::Inverse(Matrix44<TYPE> &aMatrix)
		{
			Vector4<TYPE> theTranslation;
			theTranslation = aMatrix.GetTranslation();
			theTranslation.x *= -1;
			theTranslation.y *= -1;
			theTranslation.z *= -1;
			theTranslation.w = 1;

			//	Matrix44<TYPE> theMatrix(aMatrix);

			aMatrix.SetTranslation(0.0f, 0.0f, 0.0f, 1.0f);
			aMatrix = Transpose(aMatrix);

			theTranslation *= aMatrix;

			aMatrix.SetTranslation(theTranslation.x, theTranslation.y, theTranslation.z, 1);

			return aMatrix;
		}

		template<class TYPE>
		Matrix44<TYPE> CreateReflectionMatrixAboutAxis44(Vector3<TYPE> reflectionVector)
		{
			Matrix44<TYPE> reflectionMatrix;
			reflectionMatrix.myMatrix[0] = 1 - 2 * (reflectionVector.x*reflectionVector.x);
			reflectionMatrix.myMatrix[1] = -2 * (reflectionVector.x*reflectionVector.y);
			reflectionMatrix.myMatrix[2] = -2 * (reflectionVector.x*reflectionVector.z);

			reflectionMatrix.myMatrix[4] = -2 * (reflectionVector.y*reflectionVector.x);
			reflectionMatrix.myMatrix[5] = 1 - 2 * (reflectionVector.y*reflectionVector.y);
			reflectionMatrix.myMatrix[6] = -2 * (reflectionVector.y*reflectionVector.z);

			reflectionMatrix.myMatrix[8] = -2 * (reflectionVector.z*reflectionVector.x);
			reflectionMatrix.myMatrix[9] = -2 * (reflectionVector.z*reflectionVector.y);
			reflectionMatrix.myMatrix[10] = 1 - 2 * (reflectionVector.z*reflectionVector.z);
			return reflectionMatrix;
		};



#pragma endregion
	};
};
namespace CU = CommonUtilities;