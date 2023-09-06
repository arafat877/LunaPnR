#include "pdkdialog.h"
#include <QBoxLayout>
#include "pdktile.h"

namespace GUI
{

PDKDialog::PDKDialog(std::vector<PDKInfo> &pdks) : m_pdks(pdks)
{
    setWindowTitle("Select PDK");

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto mainLayout = new QVBoxLayout();    
    m_tileList = new QScrollArea();
    m_tileList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    auto tileLayout = new QVBoxLayout();
    tileLayout->setSizeConstraint(QLayout::SetFixedSize);

    int id = 0;
    for(auto const &pdk : pdks)
    {
        auto tile = new PDKTile(pdk);
        m_pdkTiles.push_back(tile);        
        tile->setID(id);

        connect(tile, &PDKTile::clicked, this, &PDKDialog::onTileClicked);
        tileLayout->addWidget(tile);
        id++;
    }

    m_tileList->setLayout(tileLayout);
    mainLayout->addWidget(m_tileList);
    setLayout(mainLayout);
}

void PDKDialog::onTileClicked(int id)
{
    // de-select previous
    if (m_selected >= 0)
    {
        m_pdkTiles.at(m_selected)->setSelected(false);
    }

    if (id >= 0)
    {
        m_pdkTiles.at(id)->setSelected(true);
    }

    m_selected = id;
}

};
