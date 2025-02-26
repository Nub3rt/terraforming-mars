#pragma once

namespace model
{
enum class Resource
{
    CREDIT = 1,
    STEEL,
    TITANIUM,
    PLANTS,
    ENERGY,
    HEAT,
    MAX = HEAT
};

constexpr inline int operator +( Resource tag ) {
    return static_cast<int>(tag);
}
}
