#pragma once

#include "Engine.h"

template<typename _num>
class Vector{
public:
	//Default
	Vector(){}
	//2D
	Vector<_num>(_num _x, _num _y){x = _x; y = _y; z = 0.0f;}
	Vector(const sf::Vector2f s){x = s.x, y = s.y, z=0.0f;}
	//3D
	Vector<_num>(_num _x, _num _y, _num _z){x = _x; y = _y; z = _z;}
	Vector<_num>(const Vector<_num> &other){x = other.x; y = other.y; z = other.z;}
	Vector(const sf::Vector3f s){x = s.x, y = s.y, z=s.z;}
	//4D
	//Vector(float x, float y, float z, float w){}

	//Destructor
	~Vector(){}

//	//2D Constructor
//	Vector<_num>(_num _x, _num _y, _num _z){x = _x;
//											y = _y;
//											dimensions = 2;}
//	//3D Constructor
//	Vector<_num>(_num _x, _num _y, _num _z){x = _x;
//											y = _y;
//											z = _z;
//											dimension = 3;}
//	//4D Constructor
//	Vector<_num>(_num _x, _num _y, _num _z, _num _w){{x = _x;
//													y = _y;
//													z = _z;
//													w = _w;
//													dimension = 4;}
//	

	//Return SFML operators
	sf::Vector2f sfml_2f(Vector<_num> &other){return sf::Vector2f(x,y);}
	sf::Vector3f sfml_3f(Vector<_num> &other){return sf::Vector3f(x,y,z);}  //TODO uncomment when implemented

	//Math Operators
	void operator=(const Vector<_num> &other);
	Vector<_num> operator+(const Vector &other);
	void operator+=(const Vector &other);
	Vector<_num> operator-(const Vector<_num> &other);

	//Scalar Multiplication
	//Vector operator*(const float &s);
	void operator*=(const float &s);
	Vector<_num> operator/(const float &s);
	void operator /=(const float &s);


	//Make Vector inverse
	void negate(){x=-x; y=-y; z=-z;}

	//Comparisons
	bool operator==(const Vector<_num> &other)const;
	bool operator!=(const Vector<_num> &other)const;
	
	//Simple Linear Algebra
	Vector<_num> Normalize();
	float DOT(const Vector<_num> &other);
	Vector<_num> CROSS(const Vector<_num> &other);
	Vector<_num> RotateZ2D(const Vector<_num>, float degrees);

	float NORM(float);


	//float getX();
	//float getY();
	//float getZ();
	//
	//void setX(_num x);
	//void setY(_num y);
	//void setZ(_num z);

	//void getDimensions();

	_num x;
	_num y;
	_num z;
	//_num w;

private:
	//_num xyz[3];
	//unsigned int dimensions;
};

//Scalar Multiplication
template<typename _num>
Vector<_num> operator*(float s, Vector<_num> &other) {
	return(Vector<_num>(s*other.x,s*other.y,s*other.z));
}
template<typename _num>
Vector<_num> operator*(Vector<_num> &other, float s){
	return(Vector<_num>(other.x*s,other.y*s,other.z*s));
}

#include "Vector.cpp"