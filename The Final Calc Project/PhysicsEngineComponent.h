
#ifndef PhysicsEngineComponent_H
#define PhysicsEngineComponent_H
#pragma once
#include"PhysicsEngine.h"

class PhysicsEngineComponent {

private:
	PhysicsEngine Object;
public:

	PhysicsEngineComponent(PhysicsEngine object);

	void Update(float alpha);

	PhysicsEngine GetPhysicsEngine();


};




PhysicsEngineComponent::PhysicsEngineComponent(PhysicsEngine object) {

	Object = object;
}

void PhysicsEngineComponent::Update(float alpha) {

	Object.Simulate(alpha);

}

PhysicsEngine PhysicsEngineComponent::GetPhysicsEngine() {
	return Object;
}



#endif // !PhysicsEngineComponent_H

