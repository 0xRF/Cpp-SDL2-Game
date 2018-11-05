#pragma once

#include <memory>

inline const std::size_t GetComponentID()
{
    static std::size_t id = 0;
    return id++;
}

template <typename T> const inline std::size_t GetComponentID()
{
    static std::size_t id = GetComponentID();
    return id;
}

class BaseEntity;

class BaseComponent {
public:
    virtual std::size_t Size() = 0;
    virtual std::size_t ID() = 0;
    bool bDestroy = false;
    bool bEnabled = true;
    BaseEntity* entity = nullptr;

    virtual void ForceUpdate() {}
    virtual void Update(const float &deltaTime) = 0;

    virtual void Render() = 0;

    virtual void Start() = 0;

    virtual void OnDestroy() = 0;

    virtual ~BaseComponent(){}
};

template <typename T>
class Component : public BaseComponent{
public:
    const static inline std::size_t CID(){ return GetComponentID<T>(); };

    std::size_t Size() override {
        static std::size_t size = sizeof(T);
        return size;
    }

    std::size_t  ID() override {
        static size_t id = GetComponentID<T>();
        return id;
    }
};
