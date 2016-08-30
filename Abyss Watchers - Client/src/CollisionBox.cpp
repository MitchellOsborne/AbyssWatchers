#include "CollisionBox.h"

CollisionBox::CollisionBox(glm::vec2 a_oPos, glm::vec2 a_oOffset, glm::vec2 a_oSize,  BoxType a_eType)
{
	m_oPos = a_oPos;
	m_oOffset = a_oOffset;
	m_oSize = a_oSize;
	m_eType = a_eType;

	TL = glm::vec2(m_oPos.x - m_oSize.x / 2, m_oPos.y + m_oSize.y / 2);
	TR = glm::vec2(m_oPos.x + m_oSize.x / 2, m_oPos.y + m_oSize.y / 2);
	BL = glm::vec2(m_oPos.x - m_oSize.x / 2, m_oPos.y - m_oSize.y / 2);
	BR = glm::vec2(m_oPos.x + m_oSize.x / 2, m_oPos.y - m_oSize.y / 2);
}

void CollisionBox::SetOwner(Agent* a_oOwner)
{
	Owner = a_oOwner;
}

void CollisionBox::Update()
{
	m_oPos = Owner->m_oPos + glm::vec2(m_oOffset.x * Owner->m_iDir, m_oOffset.y);
	TL = glm::vec2(m_oPos.x - m_oSize.x / 2, m_oPos.y + m_oSize.y / 2);
	TR = glm::vec2(m_oPos.x + m_oSize.x / 2, m_oPos.y + m_oSize.y / 2);
	BL = glm::vec2(m_oPos.x - m_oSize.x / 2, m_oPos.y - m_oSize.y / 2);
	BR = glm::vec2(m_oPos.x + m_oSize.x / 2, m_oPos.y - m_oSize.y / 2);
}

void CollisionBox::Draw(SpriteBatch* sb)
{
	sb->setSpriteColor(1, 0, 1, 1);
	sb->drawLine(TL.x, TL.y, TR.x, TR.y, 1.f, 0);
	sb->drawLine(TL.x, TL.y, BL.x, BL.y, 1.f, 0);
	sb->drawLine(TR.x, TR.y, BR.x, BR.y, 1.f, 0);
	sb->drawLine(BL.x, BL.y, BR.x, BR.y, 1.f, 0);
	sb->setSpriteColor(1, 1, 1, 1);
}

bool CollisionBox::CheckCollision(CollisionBox* other)
{
	if (IsWithin(other->TL) || IsWithin(other->TR) || IsWithin(other->BL) || IsWithin(other->BR))
	{
		return true;
	}
	return false;
}

bool CollisionBox::IsWithin(glm::vec2 point)
{
	if (point.x >= TL.x && point.y <= TL.y)
	{
		if (point.x <= BR.x && point.y >= BR.y)
		{
			return true;
		}
	}
	return false;
}