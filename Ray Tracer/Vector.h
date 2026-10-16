#pragma once
#include <iostream>


using namespace std;

class Vector
{
private:
	float xPos, yPos, zPos;

public:

	Vector();

	Vector(float x, float y, float z);

	float getXPos() const;
	

	float getYPos() const;

	float getZPos() const;

	float magnitude() const; //length // 

	Vector & normalize();

	Vector operator - (const Vector &v) const;

	Vector operator - () const;

	Vector operator + (const Vector &v) const;

	Vector operator / (const Vector &v) const;

	Vector operator * (const Vector &v) const;

	Vector operator * (const float &t) const; //scalar mult

	friend ostream& operator << (ostream& os, const Vector &v);
		

};