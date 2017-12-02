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

#include <zlayout.h>

ZLayout::ZLayout()
{
    cW=0;
    mW=0;
}

void ZLayout::addItem(QWidget *widget)
{
    widgetList.push_back(widget);
    widgetRects.push_back(widget->frameGeometry());
}

void ZLayout::initMainWidgets(QWidget *centralWidget, QWidget *middleWidget)
{
    cW=centralWidget;
    mW=middleWidget;
    mwRect=mW->frameGeometry();
}

bool ZLayout::isNull()
{
    if (mW==0 || cW==0)
    {
        qWarning("ZLayout: in isNull() function, Main widgets was not added.");
        return true;
    }

    if (widgetList.isEmpty())
    {
        qWarning("ZLayout: in isNull() function, Child widgets was not added.");
        return true;
    }

    return false;
}

void ZLayout::updateWidgets()
{
    if (!this->isNull())
    {
        float ratioConstantH = ((float)(cW->width()))/mwRect.width();
        float ratioConstantV = ((float)(cW->height()))/mwRect.height();
        for (int i=0;i<widgetList.size();i++)
            widgetList[i]->setGeometry(widgetRects[i].x()*ratioConstantH,
                                       widgetRects[i].y()*ratioConstantV,
                                       widgetRects[i].width()*ratioConstantH,
                                       widgetRects[i].height()*ratioConstantV);
        mW->setGeometry(mwRect.x()*ratioConstantH,
                        mwRect.y()*ratioConstantV,
                        mwRect.width()*ratioConstantH,
                        mwRect.height()*ratioConstantV);

    }
}
