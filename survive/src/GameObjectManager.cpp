#include "StdAfx.h"
#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager* GameObjectManager::getInstance()
{
	static GameObjectManager instance;
	return &instance;
}

void GameObjectManager::addObject(std::shared_ptr<GameObject> object)
{
	object->setId(m_nextAvailableId++);

	// add to object list
	m_objectMap[object->getId()] = object;

	const std::type_info& type = typeid(*object.get());
	
	// create type list if not already existing
	if (m_typeMap.count(&type) == 0)
	{
		m_typeMap[&type] = std::vector<std::shared_ptr<GameObject>>();
	}

	// add to type list
	m_typeMap[&type].push_back(object);
}



void GameObjectManager::removeObject(std::shared_ptr<GameObject> object)
{
	m_objectMap.erase(object->getId());

	const std::type_info& type = typeid(*object.get());
	std::vector<std::shared_ptr<GameObject>>& list = m_typeMap[&type];
	list.erase(std::find(list.begin(), list.end(), object));
}

std::vector<std::shared_ptr<GameObject>>* GameObjectManager::findObjectByType(const std::type_info& type)
{
	if (m_typeMap.count(&type) != 0)
	{
		return &m_typeMap[&type];
	}

	return nullptr;
}

std::shared_ptr<GameObject> GameObjectManager::findObjectById(int id)
{
	if (m_objectMap.count(id))
	{
		return m_objectMap[id];
	}

	return std::shared_ptr<GameObject>();
}

void  GameObjectManager::clear()
{
	m_objectMap.clear();
	m_typeMap.clear();
}