#include "hpch.h"
#include "Entity.h"

namespace HEngine {

Entity::Entity(entt::entity id, Scene* scn)
    : m_id(id)
    , scene(scn)
{
}

}
