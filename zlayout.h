/****************************************************************************
**
** Copyright (C) 2015 Omer Goktas.
** Contact: kozmon@hotmail.com
**
** ZLayout, auto expandable free layout for Qt Creator. This file is part
** of the ZLayout.
**
** Commercial License Usage
** Licensees holding valid commercial ZLayout licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Omer Goktas. For further information
** use my email address.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#ifndef ZLAYOUT_H
#define ZLAYOUT_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QtGlobal>
#include <QRect>

class ZLayout : public QObject
{
        Q_OBJECT
    private:
        QVector<QWidget*> widgetList;
        QVector<QRect> widgetRects;
        QRect mwRect;
        QWidget *cW, *mW;
        bool isNull();

    public:
        ZLayout();
        void addItem(QWidget *widget);
        void initMainWidgets(QWidget *centralWidget, QWidget *middleWidget);

    public slots:
        void updateWidgets();

};


#endif // ZLAYOUT_H
