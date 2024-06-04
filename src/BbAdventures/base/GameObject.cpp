#include <BbAdventures/base/GameObject.hpp>
using namespace Bba;

entt::registry GameObject::_registry;

GameObject::GameObject() {
	_entity = _registry.create();
}

GameObject::~GameObject() {
	_registry.destroy(_entity);
}