//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_UT_IDMANAGER_H
#define DOGBONE_UT_IDMANAGER_H

#include "UT_API.h"

#include <set>

namespace dogb::UT
{
template <typename T>
class UT_API IDManager
{
public:
    using IdType = T;

    IDManager() = default;

    IdType requestId();
    void releaseId(IdType id);

    [[maybe_unused]] void clear();

private:
    std::set<IdType> m_free;
    IdType m_currentMax;
};

template <typename T>
typename IDManager<T>::IdType
IDManager<T>::requestId()
{
    if (!m_free.empty())
    {
        // If there is something in our set then grab the first id and remove
        // it from the free list
        auto iter = m_free.begin();
        IdType id = *(iter);
        m_free.erase(iter);
        return id;
    }
    IdType id = m_currentMax;
    m_currentMax++;
    return id;
}

template <typename T>
void
IDManager<T>::releaseId(IdType id)
{
    m_free.emplace(id);
}

template <typename T>
[[maybe_unused]] void
IDManager<T>::clear()
{
    m_free.clear();
    m_currentMax = IdType();
}
} // namespace dogb::UT

#endif // DOGBONE_UT_IDMANAGER_H
