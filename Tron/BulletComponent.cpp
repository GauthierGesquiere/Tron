#include "BulletComponent.h"

#include "GameObject.h"
#include "RenderSpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"


BulletComponent::BulletComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned Dims, glm::vec2 Size, glm::vec2 Directions, glm::vec2 startPos)
	: m_SourcePath{"Tron/"}
	  , m_Speed{50}
	  , m_Velocity{Directions}
	  , m_Dims{ Dims }
	  , m_Size{Size}
	  , m_StartPos{startPos}
	  , m_pLevelIndices{pLevelIndices}
{
}

void BulletComponent::Startup()
{
	Component::Startup();

	//Get the image
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent(m_FullPath, m_Dims, m_Dims, m_Dims * static_cast<unsigned int>(m_Size.x), m_Dims * static_cast<unsigned int>(m_Size.y)));
	gObject->GetTransform().SetPosition(m_StartPos.x, m_StartPos.y, 0);
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_RenderObj = gObject;
	m_Velocity *= m_Speed;

}

void BulletComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);

	if (HitWall)
	{
		ElapsedSec += deltaSec;
		if (ElapsedSec >= 0.1f)
		{
			HitWall = false;
			ElapsedSec = 0.0f;
		}
	}
	else
	{
		CheckHitWall();
	}

	TranslateSprite(deltaSec);

	if (m_AmountOfBounces <= 0)
	{
		dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_RenderObj);
		dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_pOwner);
	}
}

void BulletComponent::TranslateSprite(float deltaTime) const
{
	m_RenderObj->GetTransform().MoveWith(glm::vec3{ m_Velocity.x * deltaTime, m_Velocity.y * deltaTime, 0 });
}

bool BulletComponent::CheckHitWall()
{
	utils::HitInfo hitInfo{};
	utils::Rectf box = CalculateBox();

	glm::vec2 startP{ box.left + box.width / 2, box.bottom - box.height / 2 - (m_Size.y * m_Dims) / 2 };
	glm::vec2 endP{ box.left + box.width / 2, box.bottom - box.height / 2 + (m_Size.y * m_Dims) / 2 };


	/*
 *		+---------+
 *		|         |
 *		|    |    |
 *		|   -+-   |
 *		|    |    |
 *		|         |
 *		+----+----+
 */


	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			HitWall = true;
			m_Velocity.y = -m_Velocity.y;
			m_AmountOfBounces--;
		}
	}


	startP = { box.left + box.width / 2 - (m_Size.x * m_Dims) / 2 , box.bottom - box.height / 2};
	endP = { box.left + box.width / 2 + (m_Size.x * m_Dims) / 2, box.bottom - box.height / 2};

	for (const auto& levelIndices : *m_pLevelIndices)
	{
		if (Raycast(levelIndices, startP, endP, hitInfo))
		{
			HitWall = true;
			m_Velocity.x = -m_Velocity.x;
			m_AmountOfBounces--;
			return true;
		}
	}

	//m_HitHorizontal = false;

	return false;
}

const utils::Rectf BulletComponent::CalculateBox() const
{
	utils::Rectf box;
	const glm::vec2 topLeft = m_RenderObj->GetTransform().GetPosition();
	box.left = topLeft.x;
	box.bottom = topLeft.y + m_Dims * m_Size.y;
	box.width = m_Dims * m_Size.x;
	box.height = m_Dims * m_Size.y;
	return box;
}
