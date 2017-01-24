#pragma once

#include "..\Math\Matrix\Matrix.h"
#include "..\Math\Vector\Vector.h"
#include <crtdbg.h>
#include "DL_Debug\DL_DEBUG\DL_Debug.h"
/* Typedefs */

typedef CommonUtilities::Math::Vector2<float> Vector2f;
typedef CommonUtilities::Math::Vector3<float> Vector3f;
typedef CommonUtilities::Math::Vector4<float> Vector4f;

typedef CommonUtilities::Math::Matrix33<float> Matrix33f;
typedef CommonUtilities::Math::Matrix44<float> Matrix44f;


/*Macros*/

#define SAFE_DELETE_ARRAY(aPointer)	\
	delete[] (aPointer);			\
	(aPointer) = nullptr;	

#define SAFE_DELETE(a) delete a, a = nullptr; DL_DEBUG("You've deleted")

#define MAX(a,b) a < b ? b : a
#define MIN(a,b) a > b ? b : a

#define CYCLIC_DELETE(i, vector) vector[i] = vector.back(); vector.pop_back();

#define FOR_EACH(i, vector) for (unsigned int i = 0; i < vector.size(); ++i)
#define FOR_EACH_OBJECT(vector)for(unsigned int i = 0; i < vector.size(); ++i)
		
#define ABS(a) (((a) < (0)) ? (a) - (a) - (a) : (a))
#define POW(a) ((a)*(a))
#define PERIM(r) ((r)*((3.14)*(3.14)))

#define RADIANS_TO_DEGREE(aRadians) \
	(float(((aRadians) * (180 / PI))))		

#define DEGREE_TO_RADIAN(aDegree)	\
	(float((aDegree) * (PI / 180)))	



#define PI 3.141592653589793f


#ifdef _DEBUG
#define NEW_DEBUG(function) new(_NORMAL_BLOCK,__FILE__,__LINE__) function; DL_DEBUG("New used.")
#define new(function) NEW_DEBUG(function) 
#endif
#ifndef _DEBUG
#define NEW_RELEASE(function) new function
#define new(function) NEW_RELEASE(function)
#endif


