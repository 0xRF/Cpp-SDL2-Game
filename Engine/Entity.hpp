#pragma once


#include <vector>
#include <iostream>

#include "Component.hpp"
#include "Vector2.hpp"

inline const std::size_t GetEntityID()
{
    static std::size_t id = 0;
    return id++;
}

//gives each entity type declared a new identifer
template <typename T> const inline std::size_t GetEntityID()
{
    static std::size_t id = GetEntityID();
    return id;
}



class BaseEntity{
public:

    bool bDestroy = false;
    virtual void OnDestroy() = 0;
    virtual void Start() = 0;
    bool bNeedsStart = true;
    bool bEnabled = true;
    virtual std::size_t ID() = 0;
    BaseComponent* components[8];

    Vector2 position = {0.0f, 0.0f};
    float scale = 1.0f;

    BaseEntity();

     BaseEntity(const float& scale);

     BaseEntity(const float& scale, const Vector2& startingPos);


     //Adds a component...
    template <typename C>
    C* const AddComponent(const C& component)
    {

        components[C::CID()] = (BaseComponent*) new C(component);
        components[C::CID()]->entity = this;
        components[C::CID()]->Start();

        return static_cast<C*>(components[C::CID()]);
    }

    //...Returns component...
    template <typename C>
    C* GetComponent() {
        if (components[C::CID()])
            return (C *) components[C::CID()];
        else {
   //         std::cout << "Component Doesn't exist\n";
            return nullptr;
        }
    }

    //Remove component of type
    template <typename C>
    void RemoveComponent()
    {
        components[C::CID()]->bDestroy = true;
    }



};

//Entity as a template
template <typename T>
class Entity : public BaseEntity {

public:
    inline Entity() : BaseEntity() {}
    inline Entity(const float& scale) : BaseEntity(scale){};
    inline Entity(const float& scale, const Vector2& startingPos) : BaseEntity(scale, startingPos){}

    const static std::size_t EID(){ return GetEntityID<T>(); };

    //give each entity a unique id
    std::size_t ID() override {
        static size_t id = GetEntityID<T>();
        return id;
    };

};


//an empty entity with now special things, suppose to be a default thinng
class Empty : public Entity<Empty>{
public:

    Empty(const Vector2& position) : Entity(1.0f, position){}
    Empty() : Entity(1.0f){}

    void OnDestroy() override;

    void Start() override;
};


