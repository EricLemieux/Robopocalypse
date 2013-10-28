#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "Vector.h"

template<typename _num>
void Vector<_num>::operator=(const Vector<_num> &other){
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

template<typename _num>
Vector<_num> Vector<_num>::operator+(const Vector<_num> &other){
	return(Vector<_num>(other.x + x, other.y + y, other.z + z));
}

template<typename _num>
void Vector<_num>::operator+=(const Vector<_num> &other){
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

template<typename _num>
Vector<_num> Vector<_num>::operator-(const Vector<_num> &other){
	return( Vector<_num>(x - other.x,y - other.y,z - other.z));
}				   
	
template<typename _num>
bool Vector<_num>::operator==(const Vector<_num> &other)const{
	if(this->x == other.x){
		if(this->y == other.y){
			if(this->z == other.z){
				return true;}}}
	return false;
}
template<typename _num>
bool Vector<_num>::operator!=(const Vector<_num> &other)const{
	if(this->x != other.x){
		if(this->y != other.y){
			if(this->z != other.z){
				return true;}}}
	return false;
}
 
//Vector Vector::operator*(const float &s){
//	return(Vector(x*s,y*s,z*s));
//}
template<typename _num>
void Vector<_num>::operator*=(const float &s){
	x*=s; 
	y*=s; 
	z*=s;
}
template<typename _num>
Vector<_num> Vector<_num>::operator/(const float &s){
	return Vector<_num>(x/s,y/s,z/s);
}
template<typename _num>
void Vector<_num>::operator/=(const float &s){
	x/=s; y/=s; z/=s;
}

template<typename _num>
float Vector<_num>::DOT(const Vector<_num> &other){
	return (x*other.x) + (y*other.y) + (z*other.z);
}
template<typename _num>
Vector<_num> Vector<_num>::CROSS(const Vector<_num> &other){
	return(Vector<_num>(y*other.z - other.y*z,
				z*other.x - other.z*x,
				 x*other.y - other.x*y));
}
template<typename _num>
float Vector<_num>::NORM(float l){  

	if(l == 1)
		return(x+y+z);
	float tempx = pow(x, l);
	float tempy = pow(y, l);
	float tempz = pow(z, l);

	float r = 1.0f / l;
	return pow(tempx + tempy + tempz, r);
}

template<typename _num>
Vector<_num> Vector<_num>::Normalize(){
   float len = this->NORM(2);

   if(len == 0) {return Vector();}

   return Vector<_num>(x/len, y/len, z/len);
}

//Rotate about z-axis 2D
template<typename _num>
Vector<_num> Vector<_num>::RotateZ2D(const Vector<_num>, float degree){
	Vector temp<_num>;
	temp.z = 0;

	float radians = degree * (3.14f / 180.0f);

	temp.x = x*cosf(radians) - y*sinf(radians);
	temp.y = x*sinf(radians) + y*cosf(radians);

	return temp;
}

//template <class _num> float Vector<_num>::getX(){
//	return xyz[0];
//}
//template <class _num> float Vector<_num>::getY(){
//	return xyz[1];
//}
//template <class _num> float Vector<_num>::getZ(){
//	return xyz[2];
//}

//template <class _num> void Vector<_num>::setX(_num x){
//	xyz[0] = x;
//}
//template <class _num> void Vector<_num>::setY(_num y){
//	xyz[1] = y;
//}
//template <class _num> void Vector<_num>::setZ(_num z){
//	xyz[2] = z;
//}
//template <class _num> void Vector<_num>::getDimensions(){
//	return dimensions;
//}

#endif VECTOR_H