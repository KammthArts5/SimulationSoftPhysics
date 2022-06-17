/*
 *  Scene.cpp
 *  PhysicEngine
 *
 *  Created by GrâˆšÂ©gory CorgiâˆšÂ© on 17/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Config.h"

#ifdef __MAC_OS__
#include <GLUT/glut.h>
#endif

#ifdef __WINDOWS__
#include <stdlib.h>
#include <GL/glut.h>
#endif

#include <list>
#include <iostream>
#include <math.h>

#include "Scene.h"
#include "Sphere.h"
#include "Force.h"
#include "Matrix.h"


// ---------------------------------------------------------------	
// Name : Constructor
// Goal : Constructor
// Params : None
// ---------------------------------------------------------------	
Scene::Scene()
{
	m_iNbObjects	=	0;
	m_dTimeElapsed	=	0;
	m_dCurrentTime	=	0;
	m_dDt			=	0;	
}

// ---------------------------------------------------------------	
// Name : Display
// Goal : Display all objects in the scene at their current position
// Params : None
// ---------------------------------------------------------------	
void Scene::Display()
{
	list<Object*>::iterator itList = this->GetListObject()->begin();
	
	// Call specifics display methods
	while( itList!=this->GetListObject()->end() )
	{	
		(*itList)->Display();
		++itList;
	}
}


// ---------------------------------------------------------------	
// Name : ApplyForces
// Goal : Apply forces to every object
// Params : None
// ---------------------------------------------------------------	
void Scene::ApplyForces()
{
	Vector3 Acceleration, AccelerationAngulaire;
	Vector3 Velocity, VitesseAngulaire;
	Vector3 Position;
	float vAngle;
	Vector3 SommeDesForces, SommeDesMoments;
	
	list<Object*>::iterator it_object = m_ListObject.begin();
	
	// For each object
	while( it_object != m_ListObject.end() )
	{
		if( (*it_object)->GetType() != OBJ_PLAN && !(*it_object)->IsImmobile())
		{
			// Init
			SommeDesForces = Vector3( 0.0f, 0.0f, 0.0f);
			SommeDesMoments = Vector3( 0.0f, 0.0f, 0.0f );

			// If the object is colliding smthg 
			// ---------------------------------------------------------------------
			if ( (*it_object)->IsInCollision() == true )
			{	
				if( (*it_object)->GetVelocity().y>0 && (*it_object)->GetVelocity().Module()<0.05
					&& (*it_object)->GetColVelocity().Module() > (*it_object)->GetVelocity().Module() )
					int r1=1;
				
				(*it_object)->SetAcceleration( Vector3( 0.0f, 0.0f, 0.0f) );
				(*it_object)->SetVelocity( (*it_object)->GetColVelocity() );
				(*it_object)->SetPos( (*it_object)->GetColPosition() );

			/*	if((*it_object)->GetType()==OBJ_BOX)
				{	std::cout<<"    &&&1 Old Vitesse"; (*it_object)->GetVelocity().DBG_Vector();
					std::cout<<"    &&&1 Old Pos"; (*it_object)->GetPos().DBG_Vector();
					if((*it_object)->GetVelocity().Module()==0.f)
						int re=1;
				}*/
			
				if((*it_object)->GetType() == OBJ_BOX)
				{
					Box* box = (Box*)(*it_object);
					box->UpdateRotation();
				}

				(*it_object)->ResetIsInCollision();
				
			}
			
			
			// Mecanique classique
			// ---------------------------------------------------------------------
			
			list<Force>::iterator it_forces = (*it_object)->m_ForcesVariables.begin();
			
			// Sum all forces that are interacting with the object
			while( it_forces != (*it_object)->m_ForcesVariables.end() )
			{
				SommeDesForces += (*it_forces).GetForce();
				//SommeDesMoments += ((*it_forces).GetApplicationPoint()).ProdVectoriel((*it_forces).GetForce());
				SommeDesMoments += ((*it_forces).GetForce()).ProdVectoriel((*it_forces).GetApplicationPoint());
				++it_forces;
			}
			
			SommeDesForces += (*it_object)->GetConstantForces();

			// Dynamic laws
			Acceleration = SommeDesForces / (*it_object)->GetMass();
			(*it_object)->SetNewAcceleration( Acceleration );
			
			Velocity = (*it_object)->GetVelocity() + Acceleration * m_dDt;
			(*it_object)->SetNewVelocity( Velocity );

			Position = (*it_object)->GetPos() + Acceleration * m_dDt * m_dDt * 1/2 + m_dDt * (*it_object)->GetVelocity();
			(*it_object)->SetNewPos( Position );

	/*		if((*it_object)->GetType()==OBJ_BOX)
			{
				std::cout<<"    ___1 SommeForce ";SommeDesForces.DBG_Vector();
				std::cout<<"    ___1 SommeMoment ";SommeDesMoments.DBG_Vector();
				std::cout<<"    ___1 New Vit"; Velocity.DBG_Vector();
				std::cout<<"    ___1 New Pos"; Position.DBG_Vector();
			}*/
	
		}


		// ---------------------------------------------------------------------------------
		// Greg 11/01 : Patch pour updater la rotation des boites, d'ailleurs, ceci devrait 
		//					s'étendre aux spheres
		if( (*it_object)->GetType() == OBJ_BOX )
		{	
			Box* CurrentBox = (Box *) (*it_object);
			AccelerationAngulaire = CurrentBox->GetInvInertieMatrix() * SommeDesMoments;
			VitesseAngulaire = CurrentBox->GetVitesseAngulaire() + AccelerationAngulaire * m_dDt;
			vAngle = (AccelerationAngulaire.Module() * m_dDt * m_dDt * 1/2) + (m_dDt * CurrentBox->GetVitesseAngulaire().Module());
			CurrentBox->SetVitesseAngulaire( VitesseAngulaire );
			CurrentBox->SetAngle( vAngle );

			CurrentBox->UpdateRotation( );

		}
		// ---------------------------------------------------------------------------------


		(*it_object)->RemoveVariableForces();
		++it_object;
	}
	// Fin de la boucle de calculs de la dynamique
}

// ---------------------------------------------------------------	
// Name : Update
// Goal : Update the scene (move the objects), detect collisions
// Params : None
// ---------------------------------------------------------------	
void Scene::Update()
{
	int iNumeroObjet = 0;
	list<Object*>::iterator it_object = m_ListObject.begin();
	
	while( it_object != m_ListObject.end() ) //Test collisions
	{
		if( (*it_object)->GetType() != OBJ_PLAN )
		{
			TestCollision(&it_object,iNumeroObjet);
		}	
		++it_object;
		iNumeroObjet++;
		
	}

	//Gere les collisions (calcul des vitesse réfléchie, forces de reaction...)
	list<Intersection>::iterator it = m_ListInter.begin();
	while( it != m_ListInter.end() )
	{
		Object* A,*B;
		A = (*it).A; B = (*it).B;
		
		int typeA = A->GetType();
		int typeB = B->GetType();

		if( typeA==OBJ_SPHERE && typeB==OBJ_SPHERE )
		{
			GererSphereSphereCollision((Sphere*)A,(Sphere*)B);
		}
		if( typeA==OBJ_SPHERE && typeB==OBJ_PLAN)
		{
			GererSpherePlanCollision((Sphere*)A,(Plan*)B);
		}
		if( typeA==OBJ_BOX && typeB==OBJ_PLAN )
		{
			GererBoxPlanCollision((Box*)A,(Plan*)B);
		}
		++it;
	}
	m_ListInter.clear();

	it_object = m_ListObject.begin();
	while( it_object != m_ListObject.end() ) //Update object's attributes
	{
		if( (*it_object)->GetType() != OBJ_PLAN )
		{
			(*it_object)->SetAcceleration( (*it_object)->GetNewAcceleration() );
			(*it_object)->SetVelocity( (*it_object)->GetNewVelocity() );
			(*it_object)->SetPos(  (*it_object)->GetNewPos() );

			//if((*it_object)->GetType()==OBJ_BOX)
			//{std::cout<<"    &&&2 "; (*it_object)->GetVelocity().DBG_Vector();}

			//Update Forces
			if( (*it_object)->GetType() == OBJ_SPHERE )
			{	
				//Force de frottement de l'air
				Vector3 Frottement = (*it_object)->GetVelocity();
				Sphere *sphere = (Sphere*)(*it_object);
				//Loi de Stokes
				Frottement = -6*PI*0.0000181*sphere->GetRayon()*Frottement;
				Vector3 Application(0.0f,0.0f,0.0f);
				Force frot(Frottement,Application);
				sphere->AddVariableForce(frot);
			}
		}	
		++it_object;
	}
	
}


// ---------------------------------------------------------------------------------	
// Name : GetMaterial
// Goal : Retourne un pointeur à partir d'une id de materiau
// Params : _id	: id du Materiau souhaité
// ---------------------------------------------------------------------------------
Materiau* Scene::GetMaterial( int _id )
{
	list<Materiau*>::iterator		it;

	// On s'assure que l'id est valide
	if ( _id > m_ListMat.size() )
		return (Materiau*) *m_ListMat.begin ();	// On retourne le materiau par default ( exist tjrs )


	it = m_ListMat.begin ();
	
	for ( int i = 0; i < _id; i++)
		++it;
		
	return (Materiau*) *it;
}


// ---------------------------------------------------------------------------------	
// Name : IsInsidePlan
// Goal : Test si A est à l'interieur du plan B
// Params : A		: objet testé 
//			_pPlan	: plan testé
//			_fDist	: distance objet / plan
// ---------------------------------------------------------------------------------
bool Scene::IsInsidePlan( Object* A, Plan* _pPlan, float _fDist )
{
	Vector3		V2, ProjectionPlan;
	Vector3		CotePlan1,CotePlan2,CotePlan3,CotePlan4;
	int			Sens, SensCourant;	

	// On place le centre de l'objet A sur la plan 
	V2				= _pPlan->GetNormal();
	ProjectionPlan	= A->GetNewPos() - _fDist * V2;
	
	// Calcul des cotes du plan	
	CotePlan1 = _pPlan->GetPoint2() - _pPlan->GetPoint1();
	CotePlan2 = _pPlan->GetPoint3() - _pPlan->GetPoint2();
	CotePlan3 = _pPlan->GetPoint4() - _pPlan->GetPoint3();
	CotePlan4 = _pPlan->GetPoint1() - _pPlan->GetPoint4();
		
	// On regarde si l'objet est à gauche ou a droite
	Sens = ( ProjectionPlan - _pPlan->GetPoint1() ) * CotePlan1;
	
	if( Sens != 0 )
		Sens = Sens / abs(Sens);
		
	SensCourant = ( ProjectionPlan - _pPlan->GetPoint2() ) * CotePlan2;
		
	if( SensCourant != 0 ) 
		SensCourant = SensCourant / abs(SensCourant);

	if( SensCourant == Sens )
	{
		SensCourant = ( ProjectionPlan - _pPlan->GetPoint3() ) * CotePlan3;

		if( SensCourant != 0 ) 
			SensCourant = SensCourant / abs(SensCourant);
			
		if( SensCourant == Sens )
		{
			SensCourant = (ProjectionPlan - _pPlan->GetPoint4() ) * CotePlan4;
				
			if( SensCourant != 0 ) 
				SensCourant = SensCourant / abs(SensCourant);

			if( SensCourant == Sens )
					return true;
	
		}
	}

	return false;
}


// ---------------------------------------------------------------------------------	
// Name : TestCollision
// Goal : Se contente d'appeler les bonnes méthodes de collision en fonction des objets
// Params : it				: objet testé 
//			iNumeroObjet	: Plan considéré
// ---------------------------------------------------------------------------------
void Scene::TestCollision( list<Object*>::iterator *it, int iNumeroObjet )
{
	int iNumeroObjetCourant = 0;
	list<Object*>::iterator it_object = m_ListObject.begin();
	

	// Parcours tous les objets de la scene
	// --------------------------------------
	while( it_object != m_ListObject.end() )
	{
		// On ne cherche pas de collision avec le meme objet
		if( iNumeroObjetCourant != iNumeroObjet )
		{
			int typeA = (**it)->GetType();
			int typeB = (*it_object)->GetType();

			// Collision Sphere / Sphere
			if( typeA==OBJ_SPHERE && typeB==OBJ_SPHERE )
			{
				if ( TestSphereSphereCollision((Sphere*)(**it), (Sphere*)(*it_object)) )
					AddIntersection( (**it),(Sphere*)(*it_object) );
			}

			// Collision Sphere / Plane
			if( typeA==OBJ_SPHERE && typeB==OBJ_PLAN)
			{
				if ( TestPlanSphereCollision((Sphere*)(**it),(Plan*)(*it_object)) )
					AddIntersection( (**it),(Plan*)(*it_object) );
			}

			// Collision Box / Plan
			if ( typeA==OBJ_BOX && typeB==OBJ_PLAN)
			{
				if ( TestBoxPlanCollision( (Box*) (**it),(Plan*) (*it_object)) )
					AddIntersection( (**it),(Plan*)(*it_object) );
			}
			
		}
		++it_object;
		iNumeroObjetCourant++;
	}
	// --------------------------------------

}

bool Scene::TestSphereSphereCollision(Sphere *sphereA, Sphere *sphereB)
{
	Vector3 V = sphereB->GetNewPos() - sphereA->GetNewPos();
	float norme = V.Module();

	if( norme <= sphereA->GetRayon() + sphereB->GetRayon() )
	{

		return true;
	}
	return false;
	
}

bool Scene::TestPlanSphereCollision(Sphere *sphere, Plan *plan)
{
	Vector3 V1 = sphere->GetNewPos() - plan->GetPos();
	Vector3 V2 = plan->GetNormal();
	float d = V1*V2;
	
	// Intersection dâˆšÂ©tectâˆšÂ©e
	if( fabs(d) <= sphere->GetRayon() )
	{
		return IsInsidePlan( sphere, plan, d);
	}
	
	return false;
}

// ---------------------------------------------------------------	
// Name : Reinit
// Goal : Reinit all objects to their initial positions
// Params : None
// --------------------------------------------------------------	
void Scene::Reinit()
{
	this->m_ListObject.clear();
	this->m_ListInter.clear();
}


void Scene::AddIntersection(Object* A, Object*B)
{
	list<Intersection>::iterator it = m_ListInter.begin();
	bool Already = false;
	//Evite les doublons dans la liste
	while( it != m_ListInter.end() )
	{
		if( (*it).A == B && (*it).B == A )
		{
			Already=true;
			break;
		}
		++it;
	}
	if( Already==false )
		m_ListInter.push_back( Intersection(A,B) );

}

void Scene::GererSphereSphereCollision(Sphere* sphereA, Sphere* sphereB)
{
	Vector3 V = sphereB->GetNewPos() - sphereA->GetNewPos();
	float norme = V.Module();
	//std::cout<<"norme de V= "<<norme<<std::endl;
	V.Normaliser();

	//Find Velocities
	Vector3 VelocityA,VelocityB;
	if(sphereA->IsInCollision() == true)
		VelocityA=sphereA->GetColVelocity();
	else
		VelocityA=sphereA->GetNewVelocity();

	if(sphereB->IsInCollision() == true)
		VelocityB=sphereB->GetColVelocity();
	else
		VelocityB=sphereB->GetNewVelocity();
	
	float 	AmortCoef 	= sphereA->GetMateriau()->GetAmortCoef() * sphereA->GetMateriau()->GetAmortCoef();
	float   fM1 = sphereA->GetMass();
	float   fM2 = sphereB->GetMass();
	
	Vector3 U1x = (V * VelocityA) * V; 
	Vector3 U1y = VelocityA - U1x;
	Vector3 U2x = (-V * VelocityB) * (-V); 
	Vector3 U2y = VelocityB - U2x;
	
	//Find new velocity
	Vector3 V1x = ( (U1x*fM1) + (U2x*fM2) - (U1x-U2x)*fM2 )/(fM1+fM2);
	Vector3 V1y = U1y;
	Vector3 V2x = ( (U1x*fM1) + (U2x*fM2) - (U2x-U1x)*fM1 )/(fM1+fM2);
	Vector3 V2y = U2y;
	
	Vector3 vReflechie1 = V1x+V1y;
	Vector3 vReflechie2 = V2x+V2y;
	//std::cout<<"sphereA: "<<sphereA->GetNewVelocity().x<<" "<<sphereA->GetNewVelocity().y<<" "<<sphereA->GetNewVelocity().z<<std::endl;
	//std::cout<<"sphereB: "<<sphereB->GetNewVelocity().x<<" "<<sphereB->GetNewVelocity().y<<" "<<sphereB->GetNewVelocity().z<<std::endl;
	//std::cout<<"V= "<<V.x<<" "<<V.y<<" "<<V.z<<std::endl;
	//std::cout<<"U1x= "<<U1x.x<<" "<<U1x.y<<" "<<U1x.z<<std::endl;
	//std::cout<<"U2x= "<<U2x.x<<" "<<U2x.y<<" "<<U2x.z<<std::endl;
	//std::cout<<"Vitesse reflechie1 "<<vReflechie1.x<<" "<<vReflechie1.y<<" "<<vReflechie1.z<<std::endl;
	
	//Amortissement?????
	Vector3 VelocityNormA = VelocityA;
	VelocityNormA.Normaliser();
	Vector3 VelocityNormB = VelocityB;
	VelocityNormB.Normaliser();

	float PenteVitesse1 = fabs( (VelocityA-(VelocityA*V)*V).Module()/(VelocityA*V));
	float PenteVitesse2 = fabs( (VelocityB-(VelocityB*V)*V).Module()/(VelocityB*V));

	//Vector3 vNewPos = sphereB->GetNewPos() - (sphereA->GetRayon() + sphereB->GetRayon())*VelocityNorm;
	Vector3 vNewPos;
	float DeltaPos1 = PenteVitesse2*(sphereA->GetRayon()+sphereB->GetRayon()-norme)/(PenteVitesse1+PenteVitesse2);
	DeltaPos1 = DeltaPos1 * sqrt(1+PenteVitesse1*PenteVitesse1);
	float DeltaPos2 = PenteVitesse1*(sphereA->GetRayon()+sphereB->GetRayon()-norme)/(PenteVitesse1+PenteVitesse2);
	DeltaPos2 = DeltaPos2 * sqrt(1+PenteVitesse2*PenteVitesse2);
	float CurrentDeltaPos = sphereA->GetDeltaPosition();
	//std::cout<<"Delta1= "<<DeltaPos1<<"   Delta2= "<<DeltaPos2<<std::endl;
	if( DeltaPos1 > CurrentDeltaPos )
	{
		vNewPos = sphereA->GetNewPos()-DeltaPos1*VelocityNormA;
		sphereA->SetCollisionPosition( vNewPos );
		sphereA->SetCollision(vReflechie1, DeltaPos1 );
	}
	else
		sphereA->SetCollision(vReflechie1, CurrentDeltaPos );

	//std::cout<<"Old PosA = "<<sphereA->GetNewPos().x<<" "<<sphereA->GetNewPos().y<<" "<<sphereA->GetNewPos().z<<std::endl;
	//std::cout<<"Old PosB = "<<sphereB->GetNewPos().x<<" "<<sphereB->GetNewPos().y<<" "<<sphereB->GetNewPos().z<<std::endl;
	//std::cout<<"New PosA = "<<vNewPos.x<<" "<<vNewPos.y<<" "<<vNewPos.z<<std::endl;

	CurrentDeltaPos = sphereB->GetDeltaPosition();
	if( DeltaPos2 > CurrentDeltaPos )
	{
		vNewPos = sphereB->GetNewPos()-DeltaPos2*VelocityNormB;
		sphereB->SetCollisionPosition( vNewPos );
		sphereB->SetCollision(vReflechie2, DeltaPos1 );
		//std::cout<<"New PosB = "<<vNewPos.x<<" "<<vNewPos.y<<" "<<vNewPos.z<<std::endl;
	}
	else
		sphereB->SetCollision(vReflechie2, CurrentDeltaPos );

	//Force de reaction
	if(V.y!=0)
	{
		Vector3 ForceVerticale( 0.0f, sphereA->GetConstantForces().y, 0.0f );
		Vector3 ForceReaction =  fabs(ForceVerticale.y/V.y)*V;
		Vector3 Application(0.0f,0.0f,0.0f);
		Force reaction(ForceReaction,Application);
		sphereA->AddVariableForce( reaction );

		ForceVerticale.Set( 0.0f, sphereB->GetConstantForces().y, 0.0f );
		ForceReaction =  fabs(ForceVerticale.y/V.y)*V;
		reaction.SetForce(ForceReaction);
		sphereB->AddVariableForce( reaction );
	}

}

void Scene::GererSpherePlanCollision(Sphere* sphere, Plan* plan)
{
	Vector3 V1 = sphere->GetNewPos() - plan->GetPos();
	Vector3 V2 = plan->GetNormal();
	float d = V1*V2;

	Vector3 Normale = plan->GetNormal();
	Vector3 SphereVelocity;
	SphereVelocity = sphere->GetNewVelocity();

	Vector3 SphereVelocityNorm = SphereVelocity;
	SphereVelocityNorm.Normaliser();
	if( Normale * SphereVelocity > 0 )
		Normale = - Normale;
	Vector3 PointIntersect;
	if(SphereVelocity*Normale!=0)
		PointIntersect = sphere->GetNewPos() - (SphereVelocity.Module()*(sphere->GetRayon()-fabs(d))/fabs(SphereVelocity*Normale))*SphereVelocityNorm - sphere->GetRayon() * Normale;
	else
		PointIntersect = sphere->GetNewPos() - sphere->GetRayon() * Normale;
	//std::cout<<"sphere vitesse= "<<SphereVelocity.x<<" "<<SphereVelocity.y<<" "<<SphereVelocity.z<<std::endl;
	//std::cout<<"sphere vitesse norm= "<<SphereVelocityNorm.x<<" "<<SphereVelocityNorm.y<<" "<<SphereVelocityNorm.z<<std::endl;
	//std::cout<<"normale= "<<Normale.x<<" "<<Normale.y<<" "<<Normale.z<<std::endl;
	//std::cout<<"d= "<<fabs(d)<<std::endl;
	// On se base sur la vitesse des precendentes collisions pour le calcul des vitesses reflechies
	if(sphere->IsInCollision() == true )
		SphereVelocity = sphere->GetColVelocity();
	else
		SphereVelocity = sphere->GetNewVelocity();

	// Calcul de la vitesse reflechie par rapport à la normale du plan
	float 	AmortCoef 	= sphere->GetMateriau()->GetAmortCoef() * plan->GetMateriau()->GetAmortCoef();
	Vector3 vReflechie 	= (fabs( SphereVelocity * Normale ) * Normale) *2 + SphereVelocity;
	
	//Amortissement suivant la normale du plan
	float fVitesseNormale = fabs(vReflechie*Normale);
	vReflechie = vReflechie +(fVitesseNormale - fVitesseNormale*AmortCoef)* (-Normale);
	
	float DeltaPos = (PointIntersect + Normale*sphere->GetRayon() - sphere->GetNewPos()).Module();
	float CurrentDeltaPos = sphere->GetDeltaPosition();
	//std::cout<<"plan: old pos: "<<sphere->GetNewPos().x<<" "<<sphere->GetNewPos().y<<" "<<sphere->GetNewPos().z<<std::endl;
	//std::cout<<"plan: new pos: "<<PointIntersect.x<<" "<<PointIntersect.y<<" "<<PointIntersect.z<<std::endl;

	if( DeltaPos > CurrentDeltaPos )
	{
		sphere->SetCollisionPosition( PointIntersect + Normale*sphere->GetRayon() );
		sphere->SetCollision(vReflechie , DeltaPos );
	}
	else
		sphere->SetCollision(vReflechie , CurrentDeltaPos );

	Vector3 v = PointIntersect + Normale*sphere->GetRayon();
	//std::cout<<"Collision avec plan : "<<Normale.x<<" "<<Normale.y<<" "<<Normale.z<<" New pos: "<<v.x<<" "<<v.y<<" "<<v.z<<std::endl;
	
	// Si le plan n'est pas vertical
	if(Normale.y!=0)
	{	
		float CoeffFrot = sphere->GetMateriau()->GetFrottementGlissementCoef();
		CoeffFrot *= plan->GetMateriau()->GetFrottementGlissementCoef();
		Vector3 ForceVerticale( 0.0f, sphere->GetConstantForces().y, 0.0f );
		Vector3 ForceReactionNormale =  fabs(ForceVerticale.y/Normale.y)*Normale;
		Vector3 ForceReactionTangeantielle=-(ForceVerticale-ForceReactionNormale);
		if(ForceReactionTangeantielle.Module()>fabs(ForceReactionNormale.Module()*CoeffFrot))
		{
			ForceReactionTangeantielle.Normaliser();
			ForceReactionTangeantielle = fabs(ForceReactionNormale.Module()*CoeffFrot)*ForceReactionTangeantielle;
		}
		Vector3 ForceReaction = ForceReactionNormale + ForceReactionTangeantielle;

		Vector3 Application(0.0f,0.0f,0.0f);
		Force reaction(ForceReaction,Application);
		sphere->AddVariableForce( reaction );

	}	
}

// ---------------------------------------------------------------	
// Name : TestBoxPlanCollision
// Goal : Test si une boite touche un plan, enregistre l'intersection si vrai
// Params : _pBox	: Boite considérée
//			_pPlan	: Plan considéré
// ---------------------------------------------------------------
bool Scene::TestBoxPlanCollision( Box* _pBox, Plan* _pPlan)
{
	Vector3		vPNormal, vV1;
	float		fR1Proj, fR2Proj, fR3Proj;
	float		fR1Dot, fR2Dot, fR3Dot;
	float		fDepth;
	float		fBoxPlanD;

	if ( !_pBox || !_pPlan )
		return false;

	vPNormal	=	_pPlan->GetNormal();
	vV1			=	_pBox->GetNewPos() - _pPlan->GetPos();
	fBoxPlanD	=	fabs( vV1 * vPNormal);

	if ( !IsInsidePlan( _pBox, _pPlan, fBoxPlanD) )
		return false;

	// Projection du repère local du cube selon la normale au plan
	fR1Dot	=	vPNormal * _pBox->GetR1();
	fR2Dot	=	vPNormal * _pBox->GetR2();
	fR3Dot	=	vPNormal * _pBox->GetR3();

	fR1Proj	=	fabs( _pBox->GetDim1() * fR1Dot );
	fR2Proj	=	fabs( _pBox->GetDim2() * fR2Dot );
	fR3Proj	=	fabs( _pBox->GetDim3() * fR3Dot );
	

	// Calcul de l'interpenetration entre les deux objets
	fDepth	=	fBoxPlanD - 0.5f * ( fR1Proj + fR2Proj +fR3Proj );

	if (  fDepth <= 0.0f )
		return true;

	return false;
}


void Scene::GererBoxPlanCollision(Box* box, Plan* plan)
{
	std::cout<<std::endl;
	std::cout<<"Collision Box/Plan"<<std::endl;
	std::cout<<"Pos box= :"; box->GetNewPos().DBG_Vector();
	
	Vector3 vReflechie, vAngulaire;
	Vector3 BoxVelocity,BoxVelocityNorm;
	Vector3 ContactPoint;
	Vector3 vNormal = plan->GetNormal();
	Vector3 PointIntersect;
	Vector3 vR1,vR2,vR3;
	Vector3 vV1;  //Vecteur qui lie le plan et la box
	TMatrix<float> Minv = box->GetInvInertieMatrix();

	float fImpulse, fImpNum, fImpDen;
	float fDim1,fDim2,fDim3;
	float fCoeffAm = box->GetMateriau()->GetAmortCoef()*plan->GetMateriau()->GetAmortCoef();
	float fBoxPlanD;

	vV1			=	box->GetNewPos() - plan->GetPos();
	fBoxPlanD	=	fabs( vV1 * vNormal);

	fDim1=box->GetDim1(); fDim2=box->GetDim2(); fDim3=box->GetDim3();
	vR1=box->GetR1(); vR2=box->GetR2(); vR3=box->GetR3();

	if(box->IsInCollision() == true )
		BoxVelocity = box->GetColVelocity();
	else
		BoxVelocity = box->GetNewVelocity();

//	if( BoxVelocity.Module()<0.2f && BoxVelocity*vNormal<0 ) //Arret de la boite
//		BoxVelocity.Set(0.f,0.f,0.f);

	if(vV1*vNormal<0)
		vNormal=-vNormal;

	//Find Contact Point
	//------------------
	
	if(vR1*vNormal != 0.f)
		ContactPoint = -fDim1/2*vR1*(vR1*vNormal)/fabs(vR1*vNormal);
	if(vR2*vNormal != 0.0f)
		ContactPoint = ContactPoint	- fDim2/2*vR2*(vR2*vNormal)/fabs(vR2*vNormal);
	if(vR3*vNormal != 0.0f)
	ContactPoint = ContactPoint - fDim3/2*vR3*(vR3*vNormal)/fabs(vR3*vNormal);

/*	if ( fabs(vR1*vNormal) >= 0.98f )
		ContactPoint = -fDim1/2*vNormal;
	if( fabs(vR2*vNormal) >= 0.98f )
		ContactPoint = -fDim2/2*vNormal;
	if( fabs(vR3*vNormal) >= 0.98f )
		ContactPoint = -fDim3/2*vNormal;*/

	std::cout<<"Point de contact: "; ContactPoint.DBG_Vector();
	
	//Find Intersect Point
	//--------------------

	float DT = FindDeltaT(box,plan,ContactPoint,BoxVelocity);
	
	float ColAngle = 0.f;
	
	if ( DT >= 0.f )
		ColAngle = box->GetVitesseAngulaire().Module() * DT;

	if ( DT == -1.f) 
		ContactPoint = -(ContactPoint*vNormal)*vNormal;

	//Vitesses Reflechies
	//-------------------
	Vector3 VitPointCol = BoxVelocity + ContactPoint.ProdVectoriel(box->GetVitesseAngulaire());
	Vector3 DistanceAxe = ContactPoint - (ContactPoint * vNormal)*vNormal; // Terme distance pt application a l'axe
	Vector3 Identity(1.,1.,1.);
	
	// Compute impulsion vector
	fImpNum		=	( 1 + fCoeffAm ) * (VitPointCol * vNormal);
	fImpNum		=	fabs( fImpNum );
	std::cout<<"!!test ";std::cout<<Minv;
	fImpDen		=	((Minv * DistanceAxe.ModuleCarre())*Identity).Module();
	Minv = box->GetInvInertieMatrix();
	std::cout<<"!!test ";std::cout<<Minv;
	std::cout<<"fImpDen= "<<fImpDen<<std::endl;
	fImpDen		+=	( 1/box->GetMass() );

	fImpulse	=	fImpNum / fImpDen;

	std::cout<<"Impulse: "<<fImpulse<<std::endl;
	std::cout<<"BoxVelocity*vNormal= "<<BoxVelocity*vNormal<<std::endl;
	std::cout<<"Normale: "; vNormal.DBG_Vector();

	if( BoxVelocity * vNormal < 0 )
	{
		if( fImpulse < -BoxVelocity * vNormal)
			vReflechie = BoxVelocity - (BoxVelocity * vNormal)*vNormal;
		else
		{
			vReflechie = BoxVelocity + fImpulse * vNormal/box->GetMass();
			std::cout<< " vReflechieImp = "; vReflechie.DBG_Vector();
		}
	}
	else
		vReflechie = BoxVelocity;

	//TODO:meme systeme pour la vangulaire que pour la vlineaire
	//vAngulaire = box->GetVitesseAngulaire() + box->GetInvInertieMatrix()*((fImpulse*vNormal).ProdVectoriel(ContactPoint));
	vV1 =	ContactPoint.ProdVectoriel(vNormal); 
	vV1.Normaliser();

//	vAngulaire	=	fImpulse * ( box->GetInvInertieMatrix() * ContactPoint).Module() * vV1;  
//	vAngulaire	+=	box->GetVitesseAngulaire(); 

	std::cout<<"Distance Axe: "; DistanceAxe.DBG_Vector();
	std::cout<<"matrice: "<< Minv;
	std::cout<<"produit: "; (Minv*DistanceAxe).DBG_Vector();
	vAngulaire = (Minv*DistanceAxe).Module()*fImpulse*vV1;
	vAngulaire	+=	box->GetVitesseAngulaire(); 

	std::cout<<"V lineaire: ";
	BoxVelocity.DBG_Vector();
	vReflechie.DBG_Vector();
	std::cout<<"V angulaire: ";
	box->GetVitesseAngulaire().DBG_Vector();
	vAngulaire.DBG_Vector();

	//Recalage de l'objet
	//-------------------
	float CurrentDelta = box->GetDeltaPosition();

	std::cout<<"DT= "<<DT<<std::endl;
	if( DT == -2.0 )
	{
		box->SetCollisionPosition(box->GetNewPos()+(fabs(ContactPoint*vNormal)-fBoxPlanD-0.05f)*vNormal , 0.0f );
		box->SetCollision(vReflechie , 0.0f );
	}

	else if( DT > CurrentDelta )
	{
		Vector3 VelocityNorm = BoxVelocity;
		VelocityNorm.Normaliser();
		
		std::cout<<"Angle= "<<ColAngle<<std::endl;
		std::cout<<"Norme V= "<<BoxVelocity.Module()<<std::endl;
		std::cout<<"direction V= ";VelocityNorm.DBG_Vector();
		std::cout<<"NewPos= ";(box->GetNewPos()-VelocityNorm*(BoxVelocity.Module()*DT)).DBG_Vector();
		box->SetCollisionPosition(box->GetNewPos()-VelocityNorm*(BoxVelocity.Module()*DT) , ColAngle );
		box->SetCollision(vReflechie , DT );
	}
	
	if(DT != -1.0f ) //ARRET
	{
		std::cout<<"New Pos DT:-1: "; box->GetNewPos().DBG_Vector();
		box->SetCollisionPosition(box->GetNewPos(), 0.0f);
		box->SetCollision(vReflechie , 0.0f );
	}

	box->SetVitesseAngulaire(vAngulaire);

	//Force de Réaction
	//-----------------
	// Si le plan n'est pas vertical
	if(vNormal.y!=0 && ( BoxVelocity * vNormal <= 0 || DT==-1.f))
	{
		float CoeffFrot = box->GetMateriau()->GetFrottementGlissementCoef();
		CoeffFrot *= plan->GetMateriau()->GetFrottementGlissementCoef();
		Vector3 ForceVerticale( 0.0f, box->GetConstantForces().y, 0.0f );
		Vector3 ForceReactionNormale =  fabs(ForceVerticale.y/vNormal.y)*vNormal;
		Vector3 ForceReactionTangeantielle=-(ForceVerticale-ForceReactionNormale);
		Force reaction;

		if(ForceReactionTangeantielle.Module()>fabs(ForceReactionNormale.Module()*CoeffFrot))
		{
			ForceReactionTangeantielle.Normaliser();
			ForceReactionTangeantielle = fabs(ForceReactionNormale.Module()*CoeffFrot)*ForceReactionTangeantielle;
		}
		Vector3 ForceReaction = ForceReactionNormale/* + ForceReactionTangeantielle*/;

		std::cout<<"Force de reaction: ";ForceReaction.DBG_Vector();
		
		reaction.SetForce( ForceReaction );
		reaction.SetPoint( ContactPoint);

		box->AddVariableForce( reaction );

	}	
}


float Scene::FindDeltaT(Box* box, Plan* plan, Vector3 ContactPoint, Vector3 BoxVelocity)
{
	Vector3 vNormal = plan->GetNormal();
	float fBoxPlanD = 	fabs((box->GetNewPos() - plan->GetPos())*vNormal);
	Vector3 vContact = ContactPoint;
	float Phi = acosf(fabs(vContact*vNormal)/vContact.Module());
	float vVAngulaire = (box->GetVitesseAngulaire()).Module();
	float a = fabs(BoxVelocity*vNormal)/vContact.Module();
	float b = fBoxPlanD/vContact.Module();
	float DT;
	Vector3 VelocityNorme = BoxVelocity;
	VelocityNorme.Normaliser();

	std::cout<<"fBoxPlanD: "<<fBoxPlanD<<std::endl;

	if( vVAngulaire <= 0.3f && BoxVelocity.Module() <=0.4f && (fBoxPlanD - fabs(ContactPoint*vNormal))> -0.01f )
	{
		std::cout<<"!!!!!!!!!!!!! ARRET OBJET !!!!!"<<std::endl;
		box->SetCollision(Vector3(0.f,0.f,0.f) , 0.0f );
		box->SetVitesseAngulaire(Vector3(0.0f,0.0f,0.0f));
		return -1.0f;
	}

	if( vVAngulaire <= 0.3f && BoxVelocity.Module() <=0.4f ) //petite vitesse mais objet trop enfoncé
		return -2.0f;
	
	if( fabs(VelocityNorme*vNormal)<0.02f && VelocityNorme.Module()>0.99f) //Pas de vitesse normale
		return -2.0f;

	if(  BoxVelocity.Module() <=0.4f  && vVAngulaire > 0.3f )
	{
		std::cout<<"calcul1 petite vLineaire: "<<vVAngulaire<<" "<<b<<" "<<Phi<<std::endl;
		std::cout<<"angle pour recaler= "<<(acosf(b)-Phi)*180/PI<<std::endl;
		DT = (acosf(b)-Phi)/vVAngulaire;
		return DT;
	}

	if ( Phi > 1.48 || vVAngulaire <= 0.3f )
	{
		DT = (fabs(ContactPoint*vNormal)-fBoxPlanD)/fabs(BoxVelocity*vNormal);
		std::cout<<"calcul2: petite vAngulaire"<<std::endl;
		return DT;
	}
/*	DT = a*a+2*a*vVAngulaire*Phi-2*(b-1)*vVAngulaire*vVAngulaire;
	DT = sqrt( DT );
	float DT1 = - DT - a - vVAngulaire*Phi;
	float DT2 = DT -  a - vVAngulaire*Phi;
	DT1 /= vVAngulaire*vVAngulaire;
	DT2 /= vVAngulaire*vVAngulaire;

	std::cout<<" calcul 3 w= "<<vVAngulaire<<" Phi: "<<Phi<<" a: "<<a<<" b: "<<b<<std::endl;

	if( DT1>DT2 && DT1>0)
		return DT1;
	else
		return DT2;

*/	
	std::cout<<" calcul 3 w= "<<vVAngulaire<<" Phi: "<<Phi<<" a: "<<a<<" b: "<<b<<std::endl;
	std::cout<<" ContactPoint*vNormal= "<<ContactPoint*vNormal<<std::endl;

	float BorneSup = (PI/2-Phi)/vVAngulaire;
	float BorneInf = 0.0f;

	if( BorneSup > 1.0f)
		BorneSup = 1.0f;

	std::cout<<std::endl;
	std::cout<<"BorneSup= "<<BorneSup<<std::endl;
	std::cout<<std::endl;

	for(int i=0;i<10;i++)
	{
		float DichoDT = BorneInf+(BorneSup - BorneInf)/2;
		float terme1 = cos(vVAngulaire*DichoDT+Phi);
		float terme2 = -(a*DichoDT+b);
		if(terme1+terme2>0)
			BorneInf = DichoDT;
		else if(terme1+terme2<0)
			BorneSup = DichoDT;
		else
		{
			BorneSup = DichoDT;
			break;
		}
		std::cout<<i<<" "<<BorneInf<<" "<<BorneSup<<" "<<terme1<<" "<<terme2<<std::endl;
	}
	return BorneInf;
}