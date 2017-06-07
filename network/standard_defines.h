#pragma once

#define TL template<typename T>

//For constructors
#define TD_(_class_) TL _class_<T>

//When the function is part of the template class
#define TD(_class_, funcType) TL funcType _class_<T>

//When the function needs template args returned or as parameters
#define TR(funcType) TL funcType

#define POW(a) a * a

