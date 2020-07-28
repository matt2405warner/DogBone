//
// Created by matt on 2020-07-27.
//

#ifndef DOGBONE_DBE_EDITOR_H
#define DOGBONE_DBE_EDITOR_H

#include "DBE_API.h"

namespace dogb::GS
{
class Entity;
class EntityManager;
}

namespace dogb::DBE
{
class DB_DBE_API Editor
{
public:
    static void drawEntity(GS::EntityManager& mgr, GS::Entity& entity);
};
}

#endif // DOGBONE_DBE_EDITOR_H
