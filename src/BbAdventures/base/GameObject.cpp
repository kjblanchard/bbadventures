#include <BbAdventures/base/GameObject.hpp>
using namespace Bba;

entt::registry GameObject::_registry;

GameObject::GameObject(entt::entity e) : _entity(e) {
}

void GameObject::FreeGameObject() {
	_registry.destroy(_entity);
}

GameObject::GameObject() {
	_entity = _registry.create();
}
