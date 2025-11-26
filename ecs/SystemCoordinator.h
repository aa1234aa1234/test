//
// Created by D412 on 2025-08-13.
//

#ifndef SYSTEMCOORDINATOR_H
#define SYSTEMCOORDINATOR_H
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"


class SystemCoordinator {
    static SystemCoordinator* instance;
    SystemManager* systemManager;
    EntityManager* entityManager;
    ComponentManager* componentManager;

    void init()
    {
        systemManager = new SystemManager();
        entityManager = new EntityManager();
        componentManager = new ComponentManager();
    }
    void destroy()
    {
        delete systemManager;
        delete entityManager;
        delete componentManager;
    }
public:
    SystemCoordinator() { init(); }
    ~SystemCoordinator() { destroy(); }

    static SystemCoordinator* getInstance()
    {
        if (!instance) instance = new SystemCoordinator();
        return instance;
    }

    static void destroyInstance()
    {
        if (instance) delete instance;
    }

    void RegisterEntity(Entity* entity);

    EntityID CreateEntity();

    Entity* GetEntity(EntityID entity) {
        return entityManager->getEntity(entity);
    }

    template<typename T>
    void RegisterComponent() {
        componentManager->registerComponent<T>();
    }

    template<typename T>
    void AddComponent(EntityID entity, T component) {
        componentManager->addComponent(entity, component);
        auto signature = entityManager->getSignature(entity);
        signature.set(componentManager->getComponentType<T>(), true);
        entityManager->setSignature(entity, signature);
        systemManager->updateEntitySignature(entity, signature);
    }

    template<typename T>
    void RemoveComponent(EntityID entity)
    {
        auto& signature = entityManager->getSignature(entity).set(componentManager->getComponentType<T>(), false);
        componentManager->removeComponent<T>(entity);
        systemManager->updateEntitySignature(entity, signature);
    }

    template<typename T>
    T& GetComponent(EntityID entity) {
        return componentManager->getComponent<T>(entity);
    }

    template<typename T>
    uint32_t GetComponentType() {
        return componentManager->getComponentType<T>();
    }

    template<typename T>
    T* RegisterSystem() {
        return systemManager->registerSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature) {
        systemManager->setSignature<T>(signature);
    }

    template<typename T>
    bool EntityHasComponent(EntityID entity) {
        return componentManager->EntityHasComponent<T>(entity);
    }
};



#endif //SYSTEMCOORDINATOR_H
