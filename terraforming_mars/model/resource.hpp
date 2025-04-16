#pragma once

namespace model
{
enum class Resource
{
    CREDIT = 0,
    STEEL,
    TITANIUM,
    PLANTS,
    ENERGY,
    HEAT,
    MAX = HEAT
};

constexpr inline int operator+( Resource resource ) {
    return static_cast<int>( resource );
}
}
