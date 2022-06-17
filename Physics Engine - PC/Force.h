#ifndef __FORCE__H__
#define __FORCE__H__

#include "Vector3.h"

class Force
{

public:
	
	Force(Vector3 _vForce, Vector3 _vApplicPoint ) {m_vForce=_vForce; m_vApplicationPoint =_vApplicPoint;}
	Force(Vector3 _vForce) {m_vForce=_vForce;}
	Force(){};

	// ---------------------------------------------------------------------
	// Attributs
	// ---------------------------------------------------------------------
	void SetForce(Vector3 _vForce) {m_vForce=_vForce;}
	void SetPoint(Vector3 _vApplicPoint) {m_vApplicationPoint=_vApplicPoint;}

	// ---------------------------------------------------------------------
	// Accesors
	// ---------------------------------------------------------------------
	Vector3 GetForce() { return m_vForce;}
	Vector3 GetApplicationPoint() { return m_vApplicationPoint;}

private:
	
	Vector3 m_vForce;
	Vector3 m_vApplicationPoint;

};

#endif