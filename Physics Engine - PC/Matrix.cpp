/*
 *  Vector3.cpp
 *  Physic Engine
 *
 *
 */
#include <string.h>
#include "Matrix.h"

// Constructeur
Matrix44::Matrix44( float _f[16] )
{
	memcpy ( &(this->m), _f, 16 * sizeof(float) );
}

// Multiplication vector3 / matrix
Vector3 operator* (const Matrix44& M,const Vector3& V)
{ 
	float vV4[4] = {V.x, V.y ,V.z, 1};
	float vRes[4];

	vRes[0]	= M.m[0] * vV4[0] + M.m[1] * vV4[1] + M.m[2] * vV4[2]	+ M.m[3] * vV4[3];
	vRes[1]	= M.m[4] * vV4[0] + M.m[5] * vV4[1] + M.m[6] * vV4[2] + M.m[7] * vV4[3];
	vRes[2]	= M.m[8] * vV4[0] + M.m[9] * vV4[1] + M.m[10] * vV4[2] + M.m[11] * vV4[3];
	vRes[3]	= M.m[12] * vV4[0] + M.m[13] * vV4[1] + M.m[14] * vV4[2] + M.m[15] * vV4[3];

	return Vector3(vRes[0],vRes[1],vRes[2]); 
}