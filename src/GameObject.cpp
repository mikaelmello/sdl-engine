#include "GameObject.hpp"
#include "Component.hpp"
#include <memory>
#include <vector>
#include <algorithm>

GameObject::GameObject() : isDead(false) , started(false), angleDeg(0) {}

GameObject::~GameObject() {
    components.clear();
}

void GameObject::Start() {
    std::for_each(
        components.begin(),
        components.end(),
        [&](std::unique_ptr<Component>& cpt) { cpt->Start(); }
    );
}

void GameObject::Update(float dt) {
    std::for_each(
        components.begin(),
        components.end(),
        [&](std::unique_ptr<Component>& cpt) { cpt->Update(dt); }
    );
}

void GameObject::NotifyCollision(GameObject& other) {
    std::for_each(
        components.begin(),
        components.end(),
        [&](std::unique_ptr<Component>& cpt) { cpt->NotifyCollision(other); }
    );
}

void GameObject::Render() {
    std::for_each(
        components.begin(),
        components.end(),
        [&](std::unique_ptr<Component>& cpt) { cpt->Render(); }
    );
}

bool GameObject::IsDead() const {
    return isDead;
}

void GameObject::RequestDelete() {
    isDead = true;
}

void GameObject::AddComponent(Component* cpt) {
    components.emplace_back(cpt);
}

void GameObject::RemoveComponent(Component* cpt) {
    auto it = std::find_if(components.begin(), components.end(),
        [&](std::unique_ptr<Component>& cpt2){ return cpt2.get() == cpt; });

    if (it != components.end()) {
        components.erase(it);
    }
}

Component* GameObject::GetComponent(const std::string& type) {
    auto it = std::find_if(components.begin(), components.end(),
        [&](std::unique_ptr<Component>& cpt2){ return cpt2->Is(type); });

    if (it == components.end()) {
        return nullptr;
    }

    return (*it).get();
}
