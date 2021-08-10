#include "techbrowser.h"
#include <QHeaderView>
#include <QLabel>

using namespace GUI;

TechBrowser::TechBrowser(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // cell table view

#if 0
    m_layerTreeView = new QTableView(parent);
    m_layerTreeView->setSelectionBehavior(QTableView::SelectRows);
    m_layerTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_layerTreeView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_layerTreeView->horizontalHeader()->setStretchLastSection(true);
    m_layerTreeView = new CellLayoutView(parent);

    m_cellModel.reset(new CellLibTableModel(nullptr));
    m_cellTableView->setModel(m_cellModel.get());
#endif

    m_layerTableView = new QTableView(this);
    m_layerTableView->setSelectionBehavior(QTableView::SelectRows);
    m_layerTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_layerTableView->setSizeAdjustPolicy(QAbstractScrollArea::adju);

    m_layerTableModel.reset(new LayerTableModel(nullptr));
    m_layerTableView->setModel(m_layerTableModel.get());

    // pin list view
    m_layerTreeView = new QTreeView(this);
    m_layerTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers); // make read-only
    //m_layerTreeView->setHeaderHidden(true);

    m_layerInfoModel.reset(new LayerInfoModel());
    m_layerTreeView->setModel(m_layerInfoModel.get());

    //m_layout2 = new QVBoxLayout();
    //m_layout2->addWidget(new QLabel("Cell information"),0);
    //m_layout2->addWidget(m_cellTreeView,1);

    m_layout = new QHBoxLayout();
    m_layout->addWidget(m_layerTableView,1);
    m_layout->addWidget(m_layerTreeView,2);
    //m_layout->addWidget(m_cellLayoutView,2);
    //m_layout->addLayout(m_layout2,1);

    setLayout(m_layout);

    connect(m_layerTableView->selectionModel(), 
        SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
        this,
        SLOT(onLayerSelectionChanged(const QItemSelection&, const QItemSelection&)));
}

TechBrowser::~TechBrowser()
{
}

QSize TechBrowser::sizeHint() const
{
    return m_layerTreeView->sizeHint();
}

void TechBrowser::setTechLib(ChipDB::TechLib *techLib)
{
    auto layer = techLib->m_layers.at(0);
    m_layerInfoModel->setLayer(layer);
    m_layerTableModel->setTechLib(techLib);

    for(size_t c=0; c < m_layerTableView->horizontalHeader()->count(); c++)
    {
        m_layerTableView->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}


void TechBrowser::onLayerSelectionChanged(const QItemSelection &cur, const QItemSelection &prev)
{
    QModelIndex index = m_layerTableView->currentIndex();

    if (index.isValid())
    {        
        auto layer = m_layerTableModel->getLayer(index.row());
        if (layer != nullptr)
        {
            m_layerInfoModel->setLayer(layer);
            //m_cellInfoModel->setCell(cell);
            update();
            doLog(LOG_VERBOSE, "Selected layer %s\n", layer->m_name.c_str());
        }
    }
}
