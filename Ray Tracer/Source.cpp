#include <iostream>
#include <fstream>
#include <math.h>
#include "Vector.h"

int dotProduct(Vector &a, Vector &b);
Vector crossProduct(Vector &a, Vector &b);

class Ray
{
private:
	Vector origin;
	Vector direction;
public:

	Ray(Vector i, Vector j)
	{
		origin = i;
		direction = j;
	}

	Vector getOrigin()
	{
		return origin;
	}

	Vector getDirection()
	{
		return direction;
	}


};

class Color
{
private:

	float r;
	float g;
	float b;

	/*uint8_t r;
	uint8_t g;
	uint8_t b;*/
public:
	Color()
	{
		r = 0;
		g = 0;
		b = 0;
	}

	Color(float i, float j, float k)
	{
		r = i;
		g = j;
		b = k;
	}

	float getRed()
	{
		return r;
	}
	float getGreen()
	{
		return g;
	}
	float getBlue()
	{
		return b;
	}
};

class Sphere
{
private:
	Vector center;
	float radius;
	Color color;

public:
	Sphere(Vector i, float j, Color col)
	{
		center = i;
		radius = j;
		color = col;

	}
	bool intersect(Ray ray, float *t)
	{
		Vector direction = ray.getDirection();
		Vector l = center - ray.getOrigin(); // vector from the ray origin to the center

		float r2, s, s2, l2,m2,q;

		r2 = radius * radius;
		s = dotProduct(l, direction); //projection of l onto the ray direction
		s2 = s * s;
		l2 = dotProduct(l, l);

		if (s < 0 && l2 > r2) // if s < 0 and ray orgin is outside the sphere, the sphere is behind ray orgin
		{
			return false;
		}
		m2 = l2 - s2; // Pythagorean Theorem
		if (m2 > r2) // the ray will miss if m2 > r2
		{
			return false;
		}
		q = sqrt(r2 - m2); 
		if (l2 > r2)
		{
			*t = s - q; //ray orgin is outside the sphere
		}
		else 
		{
			*t = s + q; //ray orgin is inside the sphere
		}

		return true;

	}

	Vector getCenter()
	{
		return center;
	}

	Color getColor()
	{
		return color;
	}
};

class Plane
{
private:
	Vector normal;//n = 0,0,1
	Vector point;
	Color color;
	float epsilon = numeric_limits<float>::epsilon();


public:

	Plane(Vector p, Vector n, Color col)
	{
		point = p;
		normal = n;
		color = col;

	}

	bool intersect(Ray ray, float *t)
	{

		Vector direction = ray.getDirection();
		Vector origin = ray.getOrigin();

		//dot av normal och ray riktning
		float dotND = (float)dotProduct(normal, direction);


		if (dotND > epsilon) //if the dotproduct of den normal and ray direction then the ray does not hit the plane
		{
			Vector pointMinusOrgin = point - origin;
			*t = dotProduct(pointMinusOrgin, normal) / dotND;


			if (*t >= 0)
			{
				return true;
			}
		}
		return false;
	}


	Color getColor()
	{
		return color;
	}

};

class Box
{
private:
	float tMin = -numeric_limits<float>::infinity();
	float tMax = numeric_limits<float>::infinity();
	float epsilon = numeric_limits<float>::epsilon();

	Vector center;
	Color color;
	float halfWidth = 0;
	float halfHeight = 0;

	float t1;
	float t2;

	float hU = halfHeight, hV = halfWidth, hW = halfHeight;
	float halfLengthArray[3];


	//a u,v,w are normazlsed side directions
	Vector a[3] = { Vector(1, 0, 0).normalize(), Vector(0, 1, 0).normalize(), Vector(0, 0, 1).normalize() };

public:

	Box(Vector c, Vector n0, Vector n1, Vector n2, float w, float h, Color col)
	{
		center = c;
		halfWidth = w / 2;
		halfHeight = h / 2;
		color = col;
		halfLengthArray[0] = h / 2;
		halfLengthArray[1] = w / 2;
		halfLengthArray[2] = h / 2;
		a[0] = n0.normalize();
		a[1] = n1.normalize();
		a[2] = n2.normalize();
	}

	bool  intersect(Ray ray, float *t)
	{
		Vector origin = ray.getOrigin();
		Vector direction = ray.getDirection();

		Vector p = center - origin;

		for (int i = 0; i < 3; i++)
		{

			//cout << halfLengthArray[i] << endl;
			float e = (float)dotProduct(a[i], p);
			float f = (float)dotProduct(a[i], direction);

			if (abs(f) > epsilon) // line 7 checks if the ray direction is not perpendicular to the normal direction of the slab currently tested
			{
				t1 = (e + halfLengthArray[i]) / f;//line 8
				t2 = (e - halfLengthArray[i]) / f;//line 9

				if (t1 > t2) //line 10 ensures that the minimum of t1 and t2 is stored t1 and that the maximum is stored in t2
				{
					float tTemp = t1;
					t1 = t2;
					t2 = tTemp;
				}

				if (t1 > tMin)//line 11 
				{
					tMin = t1;
				}

				if (t2 < tMax)//line 12
				{
					tMax = t2;

				}

				if (tMin > tMax)//line 13 if ray misses
				{
					return false;
				}
				if (tMax < 0) //line 14 behind ray orgin
				{
					return false;
				}
			}
			else if (-e - halfLengthArray[i] > 0 || -e + halfLengthArray[i] < 0) //line 15 is executed if ray is parallel to the slab and so cannot intersect. Checks if the ray is outside the slab.
			{
				return false;
			}

		}
		if (tMin > 0)
		{
			*t = tMin;
			return true;
		}
		else
		{
			*t = tMax;
			return true;
		}

	}

	Color getColor()
	{
		return color;
	}
};

class Triangle
{
private:
	Vector p0;
	Vector p1;
	Vector p2;
	Color color;

	float tMin = -numeric_limits<float>::infinity();
	float tMax = numeric_limits<float>::infinity();
	float epsilon = numeric_limits<float>::epsilon();

public:
	Triangle(Vector a, Vector b, Vector c, Color col)
	{
		p0 = a;
		p1 = b;
		p2 = c;
		color = col;
	}

	Color getColor()
	{
		return color;
	}


	bool intersect(Ray ray, float *t)
	{
		float u ,v ,a ,f;
		Vector e1 ,e2 ,q ,s ,r ;

		Vector direction = ray.getDirection();
		Vector origin = ray.getOrigin();

		e1 = p1 - p0; //Edge1
		e2 = p2 - p0; //Edge2

		q = crossProduct(direction, e2);
		a = (float)dotProduct(e1,q); // determinant of matrix M
		if (a > -epsilon && a < epsilon)
		{
			return false;
		}

		f = 1 / a; //inverse determinant
		s = origin - p0;
		u = f * dotProduct(s, q); 
		if (u < 0.0f || u > 1.0f)
		{
			return false;
		}
		r = crossProduct(s, e1);
		v = f * dotProduct(direction, r);
		if (v < 0.0f || (u + v) > 1.0f)
		{
			return false;
		}
		*t = f * dotProduct(e2, r);

		if (*t < 0)
		{
			return false;
		}

		return true;

	}

};


int main()
{
	ofstream image;
	const int width = 256;
	const int height = 256;


	Color pixel_Col[height][width];
	Color red(255, 0, 0);
	Color blue(0, 0, 255);
	Color neonPurple(194, 14, 213);
	Color gray(220, 220, 220);

	Color testColor(255, 255, 0);//yellow

	float triCod = 500;



	Plane plane(Vector(0,0,2000), Vector(0, 0, 1),gray);
	Sphere sphere(Vector(width / 2,height / 2,100),55,blue);
	Box box(Vector(0.0f, 0.0f, 300), Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), width, height, red);
	Triangle triangle(Vector(256, 0,triCod), Vector(100, 128, triCod), Vector(100, 50, triCod), neonPurple);


	image.open("image.ppm");

	image << "P3\n" << width << " " << height << "\n255\n";

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++) {
			Ray ray(Vector(j, i, 0.0f), Vector(0.0f, 0.0f, 1.0f));

			float t = 30000;
			float tMin = 200000;

			if (plane.intersect(ray, &t) == true)
			{
				if (t < tMin)
				{
					pixel_Col[i][j] = plane.getColor();
					tMin = t;

				}
			}

			if (sphere.intersect(ray, &t) == true)
			{

				if ( t < tMin)
				{
					pixel_Col[i][j] = sphere.getColor();
					tMin = t;

				}

			}
			if (triangle.intersect(ray, &t) == true)
			{
				if (t < tMin)
				{
					pixel_Col[i][j] = triangle.getColor();
					tMin = t;

				}
			}
			if (box.intersect(ray, &t) == true)
			{
				if (t < tMin)
				{
					pixel_Col[i][j] = box.getColor();
					tMin = t;

				}
			}
				
			image << pixel_Col[i][j].getRed() << endl;
			image << pixel_Col[i][j].getGreen() << endl;
			image << pixel_Col[i][j].getBlue() << endl;
		}
	}


	std::cout << "Done press enter" << endl;
	image.close();
	getchar();
	return 0;
}




int dotProduct(Vector & a, Vector & b)
{
	return  (int)(a.getXPos() * b.getXPos()) + (a.getYPos() * b.getYPos()) + (a.getZPos() * b.getZPos());
}

Vector crossProduct(Vector & a, Vector & b)
{
	float x = ((a.getYPos() * b.getZPos()) - (a.getZPos() * b.getYPos()));
	float y = ((a.getZPos() * b.getXPos()) - (a.getXPos() * b.getZPos()));
	float z = ((a.getXPos() * b.getYPos()) - (a.getYPos() * b.getXPos()));
	//gives new vector(normal) n (x,y,z)
	return Vector(x,y,z);

	

}




