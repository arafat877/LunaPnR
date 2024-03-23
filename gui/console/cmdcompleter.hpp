// SPDX-FileCopyrightText: 2021-2024 Niels Moseley <asicsforthemasses@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include <list>
#include <QString>
#include <QStringList>
#include "replwidget.hpp"

namespace GUI
{

struct LunaCommandCompleter : public ReplWidget::ICompleter
{
    QString tryComplete(const QString &str);

    std::vector<QString> m_words;
};

};