#include "Vector.h"


Vector::Vector()
{
	xPos = 0;
	yPos = 0;
	zPos = 0;
}

Vector::Vector(float x, float y, float z)
{
	xPos = x;
	yPos = y;
	zPos = z;
}

float Vector::getXPos() const
{
	return xPos;
}

float Vector::getYPos() const
{
	return yPos;
}

float Vector::getZPos() const
{
	return zPos;
}

float Vector::magnitude() const //length // 
{
	return sqrt(pow(xPos, 2) + pow(yPos, 2) + pow(zPos, 2));
}

Vector &Vector::normalize()
{

	float inverse = 1 / magnitude();
	xPos = xPos * inverse;
	yPos = yPos * inverse;
	zPos = zPos * inverse;

	return *this;
}

Vector Vector::operator-(const Vector & v) const
{
	return Vector(xPos - v.xPos, yPos - v.yPos, zPos - v.zPos);
}

Vector Vector::operator-() const
{
	return Vector(-xPos,-yPos,-zPos);
}

Vector Vector::operator+(const Vector & v) const
{
	return Vector(xPos + v.xPos, yPos + v.yPos, zPos + v.zPos);
}

Vector Vector::operator/(const Vector & v) const
{
	return Vector(xPos / v.xPos, yPos / v.yPos, zPos / v.zPos);
}

Vector Vector::operator*(const Vector & v) const
{
	return Vector(xPos * v.xPos, yPos * v.yPos, zPos * v.zPos);
}

Vector Vector::operator*(const float & t) const
{
	return Vector(xPos*t,yPos*t,zPos*t);
}

ostream & operator<<(ostream & os, const Vector & v)
{
	return os << v.xPos << " " << v.yPos << " " << v.zPos << "\n";
}
