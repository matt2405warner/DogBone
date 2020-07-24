//
// Created by matt on 2020-07-17.
//

#ifndef DOGBONE_CE_ASSET_H
#define DOGBONE_CE_ASSET_H

#include "CE_API.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace dogb::CE
{
class UUID : public boost::uuids::uuid
{
public:
    UUID() : boost::uuids::uuid(boost::uuids::random_generator()()) {}

    explicit UUID(const boost::uuids::uuid &u) : boost::uuids::uuid(u) {}
};

class Asset
{
public:
    Asset() : m_id() {}

    [[nodiscard]] const UUID &id() const { return m_id; }

    bool operator<(const Asset &asset) const { return m_id < asset.m_id; }
    bool operator>(const Asset &asset) const { return m_id > asset.m_id; }
    bool operator>=(const Asset &asset) const
    {
        return (*this > asset) || (*this == asset);
    }
    bool operator<=(const Asset &asset) const
    {
        return (*this < asset) || (*this == asset);
    }
    bool operator==(const Asset &asset) const { return m_id == asset.m_id; }
    bool operator!=(const Asset &asset) const { return !(*this == asset); }

private:
    UUID m_id;
};
} // namespace dogb::CE

#endif // DOGBONE_CE_ASSET_H
