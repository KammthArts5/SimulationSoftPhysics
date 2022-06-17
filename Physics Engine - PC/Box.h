/*
 *  Box.h
 *  PhysicEngine
 *
 *  Created by Julien Peyre and Gregory Corgie on 10/01/07.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 
 *
 */

#ifndef __BOX_h__ 

#define __BOX_h__

#include "Object.h"
#include "Matrix.h"

class Box : public Object 
{

public:
	
	Box(Vector3 Centre, float dim1, float dim2, float dim3, Vector3 Orientation1, Vector3 Orientation2, float Mass);

	// ---------------------------------------------------------------
	// Accessors
	// ---------------------------------------------------------------

	void SetR1(Vector3 _R1)		{ m_vR1 = _R1;}
	void SetR2(Vector3 _R2)		{ m_vR2 = _R2;}
	void SetR3(Vector3 _R3)		{ m_vR3 = _R3;}

	void SetVitesseAngulaire(Vector3 _W) { m_vWAng = _W; }
	void SetAngle( float _angle ) {m_fAngle = _angle;}
	
	float GetAngle() { return m_fAngle;}

	Vector3 GetR1()				{ return m_vR1;}
	Vector3 GetR2()				{ return m_vR2;}
	Vector3 GetR3()				{ return m_vR3;}

	Vector3 GetVitesseAngulaire() { return m_vWAng; }

	float	GetDim1()			{ return m_fDim1;}
	float	GetDim2()			{ return m_fDim2;}
	float	GetDim3()			{ return m_fDim3;}

	TMatrix<float> GetInertieMatrix() { return m_mInertie; }
	TMatrix<float> GetInvInertieMatrix() { return m_mInertieInv; }
	
	// ---------------------------------------------------------------
	// Methods
	// ---------------------------------------------------------------
	void Display();
	void UpdateRotation();
	void ApplyRotation(float fAngle, Vector3 AxeRotation);
	void SetCollisionPosition( Vector3 _vPos, float ColAngle )	{ m_vCollisionPosition = _vPos; m_fColAngle = ColAngle;}


private:
	
	// ---------------------------------------------------------------
	// Attributs
	// ---------------------------------------------------------------
	float	m_fDim1, m_fDim2, m_fDim3;
	float   m_fAngle;					//Angle de rotation
	Vector3 m_vWAng;					// vitesse angulaire
	Vector3 m_vR1, m_vR2, m_vR3;		// Axes propres au cube
	TMatrix<float> m_mInertie;           //Matrice d'inertie
	TMatrix<float> m_mInertieInv;       //Matrice d'inertie inverse

	float       m_fColAngle;
};

#endif