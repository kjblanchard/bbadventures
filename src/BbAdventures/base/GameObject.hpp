#pragma once
#include <entt/entity/registry.hpp>

namespace Bba {
class GameObject {
   public:
	GameObject();
	~GameObject();
	template <typename T, typename... Args>
	void AddComponent(Args&&... args);
	template <typename T>
	T& GetComponent();
	template <typename T>
	bool HasComponent() const;
	template <typename T>
	void RemoveComponent();
	static entt::registry _registry;
	// static entt::registry* GetRegistry() {return &_registry;}

   private:
	entt::entity _entity;
};


template <typename T, typename... Args>
void GameObject::AddComponent(Args&&... args) {
	_registry.emplace<T>(_entity, std::forward<Args>(args)...);
}
template <typename T>
T& GameObject::GetComponent() {
	return _registry.get<T>(_entity);
}
template <typename T>
bool GameObject::HasComponent() const {
	return _registry.all_of<T>(_entity);
}

template <typename T>
void GameObject::RemoveComponent() {
	_registry.remove<T>(_entity);
}

}  // namespace Bba