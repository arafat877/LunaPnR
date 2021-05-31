/*

    String list model that adapts a vector 
    of strings to a QAbstractListModel

*/

#pragma once

#include <QAbstractListModel>
#include <QAbstractTableModel>
#include <string>
#include <vector>

#include "lunacore.h"

namespace GUI
{

/** String list model that adapts a vector of strings to a QAbstractListModel */
class CellLibListModel : public QAbstractListModel
{
public:
    CellLibListModel(const ChipDB::CellLib *m_cellLib);

    void setCellLib(const ChipDB::CellLib *m_cellLib);

    /** return the number of rows in the list */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /** returns various information about enabled/disabled items etc */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /** query the data a 'index' position */
    virtual QVariant data(const QModelIndex &index, int role) const override;

    /** query the view/list header information */
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    const ChipDB::Cell* getCell(int row) const;

    void cellLibUpdated();

protected:
    const ChipDB::CellLib *m_cellLib;
};


/** String list model that adapts a vector of strings to a QAbstractListModel */
class CellLibTableModel : public QAbstractTableModel
{
public:
    CellLibTableModel(const ChipDB::CellLib *m_cellLib);

    void setCellLib(const ChipDB::CellLib *m_cellLib);

    /** return the number of rows in the table */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /** return the number of columns in the table */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /** returns various information about enabled/disabled items etc */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /** query the data a 'index' position */
    virtual QVariant data(const QModelIndex &index, int role) const override;

    /** query the view/list header information */
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    const ChipDB::Cell* getCell(int row) const;

    void cellLibUpdated();

protected:
    const ChipDB::CellLib *m_cellLib;
};


}; // namespace

