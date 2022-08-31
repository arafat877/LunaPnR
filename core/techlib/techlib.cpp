// SPDX-FileCopyrightText: 2021-2022 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "techlib.h"
#include <algorithm>

using namespace ChipDB;

std::string ChipDB::toString(const LayerType &lt)
{
    switch(lt)    
    {
    default:
    case LAYER_UNDEFINED:
        return "UNDEFINED";
    case LAYER_ROUTING:
        return "ROUTING";        
    case LAYER_CUT:
        return "CUT";
    case LAYER_MASTERSLICE:
        return "MASTERSLICE";
    case LAYER_OVERLAP:
        return "OVERLAP";
    }    
}

std::string ChipDB::toString(const LayerDirection &ldir)
{
    switch(ldir)    
    {
    default:
    case LAYERDIR_UNDEFINED:
        return "UNDEFINED";
    case LAYERDIR_HORIZONTAL:
        return "HORIZONTAL";
    case LAYERDIR_VERTICAL:
        return "VERTICAL";
    }
}

void TechLib::clear()
{
    m_layers.clear();
    m_sites.clear();
    m_manufacturingGrid = 0;
}

KeyObjPair<LayerInfo> TechLib::createLayer(const std::string &name)
{
    auto layerKeyObj = m_layers.add(std::make_shared<LayerInfo>(name));
    if (layerKeyObj)
    {
        return layerKeyObj.value();
    }

    return KeyObjPair<LayerInfo>{};
}

KeyObjPair<LayerInfo> TechLib::lookupLayer(const std::string &name) const
{
    return m_layers[name];
}

KeyObjPair<SiteInfo> TechLib::createSiteInfo(const std::string &name)
{
    auto siteKeyObj = m_sites.add(std::make_shared<SiteInfo>(name));
    if (siteKeyObj)
    {
        return siteKeyObj.value();
    }

    return KeyObjPair<SiteInfo>{};
}

KeyObjPair<SiteInfo> TechLib::lookupSiteInfo(const std::string &name) const
{
    return m_sites[name];
}

void TechLib::addLayerListener(ChipDB::INamedStorageListener *listener)
{
    m_layers.addListener(listener);
}

void TechLib::removeLayerListener(ChipDB::INamedStorageListener *listener)
{
    m_layers.removeListener(listener);
}

void TechLib::addSiteListener(ChipDB::INamedStorageListener *listener)
{
    m_sites.addListener(listener);
}

void TechLib::removeSiteListener(ChipDB::INamedStorageListener *listener)
{
    m_sites.removeListener(listener);
}

void TechLib::contentsChanged() const
{
    m_sites.contentsChanged();
    m_layers.contentsChanged();
}
