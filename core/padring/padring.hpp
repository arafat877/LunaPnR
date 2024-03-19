// SPDX-FileCopyrightText: 2021-2024 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include <string>
#include <memory>
#include <list>
#include <algorithm>
#include "database/database.h"

namespace LunaCore::Padring
{

class LayoutItem
{
public:
    enum class ItemType
    {
        UNDEFINED,
        CELL,
        CORNER,
        FIXEDSPACE,
        FLEXSPACE,
        FILLER
    };

    LayoutItem() = default;

    void clear()
    {
        m_cellName = "";
        m_instanceName = "";
        m_orientation = ChipDB::Orientation::UNDEFINED;
        m_size = 0;
        m_pos  = 0;
        m_itemType = ItemType::UNDEFINED;
    }

    std::string         m_instanceName;
    std::string         m_cellName;
    ChipDB::Orientation m_orientation{ChipDB::Orientation::UNDEFINED};
    ChipDB::CoordType   m_size{0};
    ChipDB::CoordType   m_pos{0};
    ItemType m_itemType{ItemType::UNDEFINED};
};

class Layout
{
public:
    enum class Direction
    {
        UNDEFINED,
        HORIZONTAL,
        VERTICAL
    };

    Layout() = default;

    void clear()
    {
        m_items.clear();
        m_cellCount = 0;
        m_direction = Direction::UNDEFINED;
    }

    [[nodiscard]] constexpr auto cellCount() const noexcept
    {
        return m_cellCount;
    }

    /** insert a layout item. Layout takes object ownership */
    void insertLayoutItem(LayoutItem *item)
    {
        m_items.emplace_back(item);
        if (item->m_itemType == LayoutItem::ItemType::CELL)
        {
            m_cellCount++;
        }
    }

    /** remove a layout item from the list. */
    bool removeLayoutItem(const std::string &instanceName)
    {
        auto iter = std::find_if(m_items.begin(), m_items.end(),
            [&](auto const &ptr)
            {
                return (ptr->m_instanceName == instanceName);
            }
        );

        if (iter != m_items.end())
        {
            if ((*iter)->m_itemType == LayoutItem::ItemType::CELL)
            {
                m_cellCount--;
            }

            m_items.erase(iter);
            return true;
        }
        return false;
    }

    auto begin() const {return m_items.begin(); }
    auto end() const {return m_items.end(); }

protected:
    Direction m_direction{Direction::UNDEFINED};
    std::list<std::unique_ptr<LayoutItem> > m_items;

    std::size_t m_cellCount{0};
    ChipDB::Rect64 m_layoutRect;    ///< the size of the area to use for layout
};

class Padring
{
public:
    Padring() = default;

    void clear();
    bool layout(Database &db);

    Layout m_top;       ///< top layout without the corner cells
    Layout m_bottom;    ///< bottom layout without the corner cells
    Layout m_left;      ///< left layout without the corner cells
    Layout m_right;     ///< right layout without the corner cells

    LayoutItem m_upperLeftCorner;
    LayoutItem m_upperRightCorner;
    LayoutItem m_lowerLeftCorner;
    LayoutItem m_lowerRightCorner;
};

};
