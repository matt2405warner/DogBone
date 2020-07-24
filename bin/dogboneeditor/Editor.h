//
// Created by matt on 2020-07-24.
//

#ifndef DOGBONE_EDITOR_H
#define DOGBONE_EDITOR_H

namespace dogb
{
namespace GS
{
    class EntityManager;
    class Entity;
}

extern void
EditorGUI(GS::EntityManager &mgr, GS::Entity &e);
} // namespace dogb

#endif // DOGBONE_EDITOR_H
