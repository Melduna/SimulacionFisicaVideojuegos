#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Ship.h"

#include <iostream>

std::string display_text = "";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

Ship* ship;


//Particle* myparticle;
//Projectile* myprojectile;

//std::vector<Projectile*> projectiles;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	
	projectile_config ship_conf{
			{
				custom::Vector3(-200,0,-100),
				custom::Vector3::blank(),
				1.0,
				10,
				20,
				{1,1,1,1}
			},
			300, //Speed,
			0.0
	};
	ship = new Ship(ship_conf);

	//PxTransform* spheretrans = new PxTransform(0, 0, 0);
	//mysphere = new Sphere(spheretrans);
	//RegisterRenderItem(mysphere);
	//myparticle = new Particle(custom::Vector3(0, 0, 0), custom::Vector3(0, 0, 0));
	//myprojectile = new Projectile(custom::Vector3(-50, 0, -50), custom::Vector3(0, 10, 0), 2.0, 10.0);
	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	ship->step(t);

	//myparticle->integrate(t);
	//myprojectile->integrate(t);
	//firing_system->step(t);
	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	delete ship;
	//DeregisterRenderItem(mysphere);
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	char temp = toupper(key);

	switch(temp)
	{
	//case 'B': break;
	//case ' ':	break;
	case ' ':
	{
		break;
	}
	case 'Z':
	{
		//projectile_config temp{
		//	{
		//		custom::Vector3::convert(GetCamera()->getEye()),
		//		custom::Vector3::convert(GetCamera()->getDir()) * 100,
		//		0.2
		//	},
		//	10,
		//	100

		//};
		//projectiles.push_back(new Projectile(temp));
		////TODO: Fire projectile
		//firing_system->update_direction(custom::Vector3::convert(GetCamera()->getDir()) * 10);
		//firing_system->generate();
		ship->fire();
		break;
	}
	case 'I':
		ship->set_accel(UP);
		break;
	case 'J':
		ship->set_accel(LEFT);
		break;
	case 'K':
		ship->set_accel(DOWN);
		break;
	case 'L':
		ship->set_accel(RIGHT);
		break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}