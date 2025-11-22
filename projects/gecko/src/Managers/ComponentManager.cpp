#include "Managers/ComponentManager.hpp"

template<> Gecko::ComponentManager* Ogre::Singleton<Gecko::ComponentManager>::msSingleton = nullptr;
