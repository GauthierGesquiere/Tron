#include "ControllerComponent.h"

#include <iostream>

#include "Event.h"
#include "GameObject.h"

ControllerComponent::ControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::vector<glm::vec2>>* pLevelIndicesWalls, unsigned playerDims, glm::vec2 playerSize)
	: m_pLevelIndices{ pLevelIndices }
	, m_pLevelIndicesWalls{ pLevelIndicesWalls }
	, m_Size{ playerSize }
	, m_Dims{ playerDims }
	, m_MovementSpeed{ 40 }
{
}

void ControllerComponent::Startup()
{
	Component::Startup();

	utils::Rectf box = CalculateBox();
	m_pOwner->GetTransform().SetRect(box);
}

void ControllerComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);
}


bool ControllerComponent::HitHorizontal()
{
	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	glm::vec2 startP{ box.left + box.width / 2 + modifier , box.bottom - box.height / 2 - modifier };
	glm::vec2 endP{ box.left + box.width / 2 + modifier, box.bottom - box.height / 2 + modifier };


	/*
 *		+---------+
 *		|         |
 *		|         |
 *		|  |   |  |
 *		|         |
 *		|         |
 *		+----+----+
 */

	m_CanMoveLeft = false;
	m_CanMoveRight = false;

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			m_CanMoveRight = true;
		}
	}


	startP = { box.left + box.width / 2 - modifier , box.bottom - box.height / 2 - modifier };
	endP = { box.left + box.width / 2 - modifier, box.bottom - box.height / 2 + modifier };

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			//m_HitHorizontal = true;
  			m_CanMoveLeft = true;
			return true;
		}
	}

	//m_HitHorizontal = false;

	return false;
}

bool ControllerComponent::HitVertical()
{
	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	glm::vec2 startP{ box.left + box.width / 2 - modifier , box.bottom - box.height / 2 - modifier };
	glm::vec2 endP{ box.left + box.width / 2 + modifier, box.bottom - box.height / 2 - modifier };

	//std::cout << "Startpos" << startP.x << "  " << startP.y << std::endl;
	//std::cout << "Endpos" << endP.x << "  " << endP.y << std::endl << std::endl;

	/*
 *		+---------+
 *		|         |
 *		|    -    |
 *		|	      |
 *		|    -    |
 *		|         |
 *		+---------+
 */

	m_CanMoveDown = false;
	m_CanMoveUp = false;

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			m_CanMoveUp = true;
		}
	}

	startP = { box.left + box.width / 2 - modifier , box.bottom - box.height / 2 + modifier };
	endP = { box.left + box.width / 2 + modifier, box.bottom - box.height / 2 + modifier };

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			//m_HitVertical = true;
			m_CanMoveDown = true;
			return true;
		}
	}

	//m_HitVertical = false;

	return false;
}

void ControllerComponent::TranslateSprite(float deltaTime) const
{
	m_pOwner->GetTransform().MoveWith(glm::vec3{ m_Velocity.x * deltaTime, m_Velocity.y * deltaTime, 0 });
}

const utils::Rectf ControllerComponent::CalculateBox(glm::vec2 posToCalc) const
{
	if (posToCalc == glm::vec2{-1, -1})
	{
		utils::Rectf box;
		const glm::vec2 topLeft = m_pOwner->GetTransform().GetPosition();
		box.left = topLeft.x;
		box.bottom = topLeft.y + m_Dims * m_Size.y;
		box.width = m_Dims * m_Size.x;
		box.height = m_Dims * m_Size.y;
		return box;
	}

	utils::Rectf box;
	const glm::vec2 topLeft = posToCalc;
	box.left = topLeft.x;
	box.bottom = topLeft.y + m_Dims * m_Size.y;
	box.width = m_Dims * m_Size.x;
	box.height = m_Dims * m_Size.y;
	return box;
}
