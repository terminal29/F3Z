#include <entity/Entity.h>
#include <entity/component/WorldComponent.h>

Entity::Entity() { }

Entity::Entity(C3DModel model, C3DTransform transform)
    : model_(model)
    , transform_(transform)
{
}

Entity::Entity(C3DModel model)
    : model_(model)
{
}

Entity::~Entity()
{
    for (auto& component : components_) {
        component.second->setDeleting(true);
    }
}

C3DTransform const& Entity::getTransform() { return transform_; }
void Entity::setTransform(C3DTransform transform) { transform_ = transform; }

C3DModel const& Entity::getModel() { return model_; }

void Entity::setModel(C3DModel model) { model_ = model; }

void Entity::receive(MessageType message)
{
    for (auto& child : children_) {
        child.second->receive(message);
    }
    for (auto& component : components_) {
        component.second->receive(message);
    }
    for (auto& component : components_) {
        if (component.second->isDeleting()) {
            components_.erase(component.first);
        }
    }
}

std::string Entity::addChild(Entity child)
{
    std::string childName = child.getName();
    while (children_.contains(childName)) {
        childName += "_";
    }
    children_[childName] = std::make_shared<Entity>(std::move(child));
    if (auto worldComponent = getComponent<WorldComponent>().lock()) {
        children_[childName]->setWorld(worldComponent);
    }
    return childName;
}

bool Entity::removeChildByName(std::string name)
{
    if (children_.contains(name)) {
        children_.erase(name);
        return true;
    }
    return false;
}

std::weak_ptr<Entity> Entity::findChildByName(std::string name)
{
    if (children_.contains(name)) {
        return children_[name];
    }
    return std::weak_ptr<Entity>();
}

void Entity::setWorld(std::weak_ptr<WorldComponent> world) { world_ = world; }

std::weak_ptr<WorldComponent> Entity::getWorld() const { return world_; }

void Entity::setName(std::string name) { name_ = name; }

std::string const& Entity::getName() const { return name_; }
