#include "BulletComponent.h"

#include <iostream>

#include "EnemyControllerComponent.h"
#include "EventQueue.h"
#include "GameObject.h"
#include "PlayerControllerComponent.h"
#include "RenderSpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"


BulletComponent::BulletComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::shared_ptr<dae::GameObject>>* pTanks, unsigned Dims, glm::vec2 Size, glm::vec2 Directions, glm::vec2 startPos)
	: m_SourcePath{"Tron/"}
	  , m_Speed{50}
	  , m_Velocity{Directions}
	  , m_Dims{Dims}
	  , m_Size{Size}
	  , m_StartPos{startPos}
	  , m_pLevelIndices{pLevelIndices}
	  , m_Tanks{ pTanks }
{
}

BulletComponent::BulletComponent()
{
	dae::EventQueue::GetInstance().Unsubscribe("ClearAllBullets", this);
}

bool BulletComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "ClearAllBullets")
	{
		dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_RenderObj);
		dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_pOwner);
		
		dae::EventQueue::GetInstance().Unsubscribe("ClearAllBullets", this);
	}
	return false;
}

void BulletComponent::Startup()
{
	Component::Startup();

	dae::EventQueue::GetInstance().Subscribe("ClearAllBullets", this);


	//Get the image
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent(m_FullPath, m_Dims, m_Dims, m_Dims * static_cast<unsigned int>(m_Size.x), m_Dims * static_cast<unsigned int>(m_Size.y)));
	gObject->GetTransform().SetPosition(m_StartPos.x, m_StartPos.y, 0);
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_RenderObj = gObject;
	m_Velocity *= m_Speed;

	utils::Rectf box = CalculateBox();
	m_RenderObj->GetTransform().SetRect(box);
}

void BulletComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);

	if (HitWall)
	{
		m_ElapsedSecWall += deltaSec;
		if (m_ElapsedSecWall >= 0.1f)
		{
			HitWall = false;
			m_ElapsedSecWall = 0.0f;
		}
	}
	else
	{
		CheckHitWall();
	}

	m_ElapsedSec += deltaSec;
	if (m_ElapsedSec >= 1.6f && m_FirstHit == true)
	{
		CheckIfHitTank();
	}
	TranslateSprite(deltaSec);

	if (m_AmountOfBounces <= 0)
	{
		dae::EventQueue::GetInstance().Unsubscribe("ClearAllBullets", this);
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

bool BulletComponent::CheckIfHitTank()
{
	for (auto tank : *m_Tanks)
	{
		auto tankPos = tank->GetTransform().GetPosition();
		auto box = tank->GetTransform().GetRect();

		if (IsOverlapping(utils::Rectf{tankPos.x, tankPos.y, box.width, box.height}, CalculateBox()))
		{
			if (tank->GetComponentOfType<PlayerControllerComponent>())
			{
				if (tank->GetComponentOfType<PlayerControllerComponent>()->m_PlayerIndex == 0)
				{
					dae::EventQueue::GetInstance().Broadcast(new dae::Event("KilledPlayer0"));
				}
				else if(tank->GetComponentOfType<PlayerControllerComponent>()->m_PlayerIndex == 1)
				{
					dae::EventQueue::GetInstance().Broadcast(new dae::Event("KilledPlayer1"));
				}

				dae::EventQueue::GetInstance().Unsubscribe("ClearAllBullets", this);
				dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_RenderObj);
				dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_pOwner);

				m_FirstHit = false;

				return true;
	
			}

			if (tank->GetComponentOfType<EnemyControllerComponent>())
			{
				dae::EventQueue::GetInstance().Unsubscribe("ClearAllBullets", this);
				tank->GetComponentOfType<EnemyControllerComponent>()->IsHit();
				//dae::SceneManager::GetInstance().GetActiveScene()->Remove(tank);

				dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_RenderObj);
				dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_pOwner);

				m_FirstHit = false;

				return true;
			}
		}

	}

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
