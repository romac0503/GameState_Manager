#pragma once

#include <memory>
#include <map>
#include <vector>


class GameObject;

/// A global GameObject manager.
class GameObjectManager
{
public:
	static GameObjectManager* getInstance();

	/// adds a new GameObject to the manager
	void addObject(std::shared_ptr<GameObject> object);

	/// removes a GameObject from the manager
	void removeObject(std::shared_ptr<GameObject> object);

	/// finds a registered GameObject with the given id
	std::shared_ptr<GameObject> findObjectById(int id);

	/// returns all GameObjects of a certain type
	/// Usage: 
	/// <pre>
	/// std::vector<std::shared_ptr<GameObject>>* players = GameObjectManager::getInstance()->findObjectByType(typeid(Player));
	/// </pre>
	std::vector<std::shared_ptr<GameObject>>* findObjectByType(const std::type_info& type);

	/// Fills the given vector with all GameObjects of vector type.
	/// If the vector contains std::shared_ptr<Player> it is filled with all Players.
	/// Usage: 
	/// <pre>
	/// std::vector<std::shared_ptr<Player>>* players;
	/// GameObjectManager::getInstance()->findObjectByType(players);
	/// </pre>
	template<class N>
	void findObjectByType(std::vector<std::shared_ptr<N>>& list)
	{
		auto source = findObjectByType(typeid(N));

		if (source != nullptr)
		{
			for (int i = 0; i < (int) source->size(); i++)
			{
				list.push_back(std::dynamic_pointer_cast<N>((*source)[i]));
			}
		}
	}

	/// Remove all registered GameObjects
	void clear();

private:
	GameObjectManager() : m_nextAvailableId(0) {};
	~GameObjectManager() {};

	GameObjectManager(const GameObjectManager& p);
	GameObjectManager& operator=(GameObjectManager const&);

	typedef std::shared_ptr<GameObject> GameObjectPtr;
	typedef std::vector<GameObjectPtr> GameObjectList;

	/// holds a map with all registered GameObjects.
	std::map<int, GameObjectPtr> m_objectMap;

	/// holds a list of GameObjects per GameObject type.
	std::map<const std::type_info *, GameObjectList> m_typeMap;

	/// tracks the nex available id
	int m_nextAvailableId;
};