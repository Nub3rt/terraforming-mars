#pragma once

namespace model
{
enum class Tag
{
    BUILDING = 0,
    SPACE,
    POWER,
    SCIENCE,
    JOVIAN,
    EARTH,
    PLANT,
    MICROBE,
    ANIMAL,
    CITY,
    EVENT,
    MAX = EVENT
};

constexpr inline int operator+( Tag tag ) {
    return static_cast<int>( tag );
}
}
