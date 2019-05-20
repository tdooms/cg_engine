//============================================================================
// @name        : index.h
// @author      : Thomas Dooms
// @date        : 5/10/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#ifndef ENGINE_INDEX_H
#define ENGINE_INDEX_H

#include <unordered_set>

struct Index
{
    Index(const uint32_t p1, const uint32_t p2) : p1(p1), p2(p2) {}
    friend bool operator==(const Index& a, const Index& b);

    uint32_t p1, p2;
};


template<>
struct std::hash<Index>
{
    size_t operator()(const Index& a) const noexcept { return ((a.p1 << 16) + a.p2) + (((a.p2 << 16) + a.p1)); }
};

#endif //ENGINE_INDEX_H
