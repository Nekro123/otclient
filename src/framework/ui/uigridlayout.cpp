/*
 * Copyright (c) 2010-2012 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "uigridlayout.h"
#include "uiwidget.h"

UIGridLayout::UIGridLayout(UIWidgetPtr parentWidget): UILayout(parentWidget)
{
    m_cellSize = Size(16,16);
    m_cellSpacing = 0;
    m_numColumns = 1;
    m_numLines = 1;
}

void UIGridLayout::applyStyle(const OTMLNodePtr& styleNode)
{
    UILayout::applyStyle(styleNode);


    for(const OTMLNodePtr& node : styleNode->children()) {
        if(node->tag() == "cell-size")
            setCellSize(node->value<Size>());
        else if(node->tag() == "cell-width")
            setCellWidth(node->value<int>());
        else if(node->tag() == "cell-height")
            setCellHeight(node->value<int>());
        else if(node->tag() == "cell-spacing")
            setCellSpacing(node->value<int>());
        else if(node->tag() == "num-columns")
            setNumColumns(node->value<int>());
        else if(node->tag() == "num-lines")
            setNumLines(node->value<int>());
    }
}

void UIGridLayout::removeWidget(const UIWidgetPtr& widget)
{
    update();
}

void UIGridLayout::addWidget(const UIWidgetPtr& widget)
{
    update();
}

bool UIGridLayout::internalUpdate()
{
    bool changed = false;
    UIWidgetPtr parentWidget = getParentWidget();
    UIWidgetList widgets = parentWidget->getChildren();

    Rect clippingRect = parentWidget->getClippingRect();
    Point topLeft = clippingRect.topLeft();

    int index = 0;
    for(const UIWidgetPtr& widget : widgets) {
        if(!widget->isExplicitlyVisible())
            continue;

        int line = index / m_numColumns;
        int column = index % m_numColumns;
        Point virtualPos = Point(column * (m_cellSize.width() + m_cellSpacing), line * (m_cellSize.height() + m_cellSpacing));
        Point pos = topLeft + virtualPos;

        if(widget->setRect(Rect(pos, m_cellSize)))
            changed = true;

        index++;

        if(index >= m_numColumns * m_numLines)
            break;
    }

    return changed;
}

