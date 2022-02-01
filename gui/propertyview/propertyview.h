/*
  LunaPnR Source Code
  
  SPDX-License-Identifier: GPL-3.0-only
  SPDX-FileCopyrightText: 2022 Niels Moseley <asicsforthemasses@gmail.com>
*/

#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

namespace GUI
{

class PropertyModelInterface
{

};

class PropertyView : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyView(QWidget *parent = nullptr);
    virtual ~PropertyView();
    
    void setPropertyModel(PropertyModelInterface *model)
    {
        m_model = model;
    }

protected:
    QLabel* createTextLabel(const std::string &txt);
    void addPropertyWidget(QWidget *widget, int32_t row, int32_t col);

    PropertyModelInterface* m_model;
    QGridLayout*            m_gridLayout;
};

};