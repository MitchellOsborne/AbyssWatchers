#pragma once
#include <glm/glm.hpp>
#include "Agent.h"

class Agent;

enum BoxType
{
	ATTACK,
	PARRY,
	BODY,
};

class CollisionBox
{
	
public:

	

	CollisionBox() {};
	CollisionBox(glm::vec2 a_oPos, glm::vec2 a_oOffset, glm::vec2 a_oSize, BoxType a_eType);
	void Update();
	void Draw(SpriteBatch* sb);
	void SetOwner(Agent* owner);
	bool CheckCollision(CollisionBox* Other);
	
	glm::vec2 m_oPos;
	glm::vec2 m_oOffset;
	glm::vec2 m_oSize;
	Agent* Owner;
	BoxType m_eType;
	bool Active = false;
private:
	glm::vec2 TL, TR, BL, BR;

	bool IsWithin(glm::vec2 point);

};