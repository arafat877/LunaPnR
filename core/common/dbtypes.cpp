#include "dbtypes.h"

#include <iostream>
#include "dbtypes.h"

std::ostream& operator<<(std::ostream& os, const ChipDB::Coord64& r)
{
    os << "(" << r.m_x << "," << r.m_y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const ChipDB::Rect64& r)
{
    os << r.m_ll << "-" << r.m_ur;
    return os;    
}

bool ChipDB::isInsideRect(const ChipDB::Coord64 &p, const ChipDB::Rect64 &r) noexcept
{
    bool inx = ((p.m_x >= r.m_ll.m_x) && (p.m_x <= r.m_ur.m_x));
    if (!inx)
        return false;

    bool iny = ((p.m_y >= r.m_ll.m_y) && (p.m_y <= r.m_ur.m_y));
        if (!iny)
        return false;

    return true;
}

bool ChipDB::intersects(const ChipDB::Rect64 &p, const ChipDB::Rect64 &q) noexcept
{
    if ((p.m_ur.m_x < q.m_ll.m_x) || 
        (q.m_ur.m_x < p.m_ll.m_x) ||
        (p.m_ur.m_y < q.m_ll.m_y) ||
        (q.m_ur.m_y < p.m_ll.m_y))
        return false;

    return true;
}

ChipDB::Rect64 ChipDB::unionRect(const ChipDB::Rect64 &r1, const ChipDB::Rect64 &r2) noexcept
{
    ChipDB::Rect64 result;
    result.m_ll.m_x = std::max(r1.m_ll.m_x, r2.m_ll.m_x);
    result.m_ur.m_x = std::min(r1.m_ur.m_x, r2.m_ur.m_x);
    result.m_ll.m_y = std::max(r1.m_ll.m_y, r2.m_ll.m_y);
    result.m_ur.m_y = std::min(r1.m_ur.m_y, r2.m_ur.m_y);    
    return std::move(result);
}
