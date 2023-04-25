#include <entity/Entity.h>
#include <entity/component/WorldComponent.h>

baba::Entito::Entito() { }

baba::Entito::Entito(C3DModel model, C3DTransform transform)
    : model_(model)
    , transform_(transform)
{
}

baba::Entito::Entito(C3DModel model)
    : model_(model)
{
}

baba::Entito::Entito(const baba::Entito& other)
    : name_(other.name_)
    , model_(other.model_)
    , world_(other.world_)
    , transform_(other.transform_)
{
    for (auto& component : other.components_) {
        cloneComponent(*component.second);
    }
    for (const auto& otherChild : other.children_) {
        children_[otherChild.first] = std::make_shared<baba::Entito>(*otherChild.second);
    }
}

baba::Entito& baba::Entito::operator=(baba::Entito const& other)
{
    baba::Entito tmp(other);
    tmp.swap(*this);
    return *this;
}

baba::Entito::~Entito()
{
    for (auto& component : components_) {
        component.second->setDeleting(true);
    }
}

C3DTransform const& baba::Entito::getTransform() { return transform_; }
void baba::Entito::setTransform(C3DTransform transform) { transform_ = transform; }

C3DModel const& baba::Entito::getModel() { return model_; }

void baba::Entito::setModel(C3DModel model) { model_ = model; }

void baba::Entito::receive(MessageType message)
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

std::string baba::Entito::addChild(baba::Entito child)
{
    std::string childName = child.getName();
    while (children_.contains(childName)) {
        childName += "_";
    }
    children_[childName] = std::make_shared<baba::Entito>(std::move(child));
    if (auto worldComponent = getComponent<WorldComponent>().lock()) {
        children_[childName]->setWorld(worldComponent);
    }
    return childName;
}

bool baba::Entito::removeChildByName(std::string name)
{
    if (children_.contains(name)) {
        children_.erase(name);
        return true;
    }
    return false;
}

std::weak_ptr<baba::Entito> baba::Entito::findChildByName(std::string name)
{
    if (children_.contains(name)) {
        return children_[name];
    }
    return std::weak_ptr<baba::Entito>();
}

void baba::Entito::setWorld(std::weak_ptr<WorldComponent> world) { world_ = world; }

std::weak_ptr<WorldComponent> baba::Entito::getWorld() const { return world_; }

void baba::Entito::setName(std::string name) { name_ = name; }

std::string const& baba::Entito::getName() const { return name_; }
