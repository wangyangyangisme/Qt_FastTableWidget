#include "customfasttablewidget.h"

CustomFastTableWidget::CustomFastTableWidget(QWidget *parent) :
    QAbstractScrollArea(parent)
{
    START_PROFILE

    mRowCount=0;
    mColumnCount=0;

    mDefaultHeight=30;
    mDefaultWidth=100;
    mTotalHeight=0;
    mTotalWidth=0;

    mDefaultBackgroundBrush.setColor(QColor(255, 255, 255));
    mDefaultForegroundColor.setRgb(0, 0, 0);
    mGridColor.setRgb(0, 0, 0);
    mSelectionBrush.setColor(QColor(0, 0, 255));

    mVisibleLeft=-1;
    mVisibleRight=-1;
    mVisibleTop=-1;
    mVisibleBottom=-1;

    horizontalScrollBar()->setSingleStep(100);
    verticalScrollBar()->setSingleStep(100);

    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScrollBarValueChanged(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(verticalScrollBarValueChanged(int)));

    END_PROFILE("CustomFastTableWidget::CustomFastTableWidget(QWidget *parent)")
}

CustomFastTableWidget::~CustomFastTableWidget()
{
    START_PROFILE

    clearTable();

    END_PROFILE("CustomFastTableWidget::~CustomFastTableWidget()")
}

void CustomFastTableWidget::clearTable()
{
    START_PROFILE

    mRowCount=0;
    mColumnCount=0;

    mTotalHeight=0;
    mTotalWidth=0;

    mVisibleLeft=-1;
    mVisibleRight=-1;
    mVisibleTop=-1;
    mVisibleBottom=-1;

    mRowHeights.clear();
    mColumnWidths.clear();
    mOffsetX.clear();
    mOffsetY.clear();
    mData.clear();
    mSelectedCells.clear();
    mCurSelection.clear();

    END_PROFILE("void CustomFastTableWidget::clearTable()")
}

void CustomFastTableWidget::selectRow(const int row)
{
    START_PROFILE

    for (int i=0; i<mColumnCount; ++i)
    {
        setCellSelected(row, i, true);
    }

    END_PROFILE("void CustomFastTableWidget::selectRow(const int row)")
}

void CustomFastTableWidget::unselectRow(const int row)
{
    START_PROFILE

    for (int i=0; i<mColumnCount; ++i)
    {
        setCellSelected(row, i, false);
    }

    END_PROFILE("void CustomFastTableWidget::unselectRow(const int row)")
}

void CustomFastTableWidget::selectColumn(const int column)
{
    START_PROFILE

    for (int i=0; i<mRowCount; ++i)
    {
        setCellSelected(i, column, true);
    }

    END_PROFILE("void CustomFastTableWidget::selectColumn(const int column)")
}

void CustomFastTableWidget::unselectColumn(const int column)
{
    START_PROFILE

    for (int i=0; i<mRowCount; ++i)
    {
        setCellSelected(i, column, false);
    }

    END_PROFILE("void CustomFastTableWidget::unselectColumn(const int column)")
}

void CustomFastTableWidget::selectAll()
{
    START_PROFILE

    mCurSelection.clear();

    QPoint aCellPos;

    for (int i=0; i<mRowCount; ++i)
    {
        for (int j=0; j<mColumnCount; ++j)
        {
            aCellPos.setX(j);
            aCellPos.setY(i);

            mCurSelection.append(aCellPos);

            mSelectedCells[i][j]=true;
        }
    }

    END_PROFILE("void CustomFastTableWidget::selectAll()")
}

void CustomFastTableWidget::unselectAll()
{
    START_PROFILE

    for (int i=0; i<mCurSelection.length(); ++i)
    {
        mSelectedCells[mCurSelection.at(i).y()][mCurSelection.at(i).x()]=false;
    }

    mCurSelection.clear();

    END_PROFILE("void CustomFastTableWidget::unselectAll()")
}

void CustomFastTableWidget::mousePressEvent(QMouseEvent *event)
{
    START_PROFILE

    QAbstractScrollArea::mousePressEvent(event);

    END_PROFILE("void CustomFastTableWidget::mousePressEvent(QMouseEvent *event)")
}

void CustomFastTableWidget::resizeEvent(QResizeEvent *event)
{
    START_PROFILE

    updateBarsRanges();
    updateVisibleRange();

    QAbstractScrollArea::resizeEvent(event);

    END_PROFILE("void CustomFastTableWidget::resizeEvent(QResizeEvent *event)")
}

void CustomFastTableWidget::paintEvent(QPaintEvent *event)
{
    START_FREQUENT_PROFILE

    QPainter painter(viewport());

    int offsetX=-horizontalScrollBar()->value();
    int offsetY=-verticalScrollBar()->value();

    if (mVisibleTop<0 || mVisibleLeft<0)
    {
        return;
    }

    for (int i=mVisibleTop; i<=mVisibleBottom; ++i)
    {
        for (int j=mVisibleLeft; j<=mVisibleRight; ++j)
        {
            paintCell(painter, offsetX+mOffsetX.at(j), offsetY+mOffsetY.at(i), mColumnWidths.at(j), mRowHeights.at(i), i, j);
        }
    }

    END_FREQUENT_PROFILE("void CustomFastTableWidget::paintEvent(QPaintEvent *event)")
}

void CustomFastTableWidget::paintCell(QPainter &painter, const int x, const int y, const int width, const int height, const int row, const int column)
{
    START_FREQUENT_PROFILE

    painter.setPen(QPen(mGridColor));
    painter.setFont(font());

    painter.fillRect(x, y, width, height, mDefaultBackgroundBrush);
    painter.drawRect(x, y, width, height);

    painter.setPen(QPen(mDefaultForegroundColor));
    painter.drawText(x+4, y+4, width-8, height-8, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap , mData.at(row).at(column));

    END_FREQUENT_PROFILE("void CustomFastTableWidget::paintCell(QPainter &painter, const int x, const int y, const int row, const int column)")
}

void CustomFastTableWidget::horizontalScrollBarValueChanged(int value)
{
    START_PROFILE

    updateVisibleRange();

    END_PROFILE("void CustomFastTableWidget::horizontalScrollBarValueChanged(int value)")
}

void CustomFastTableWidget::verticalScrollBarValueChanged(int value)
{
    START_PROFILE

    updateVisibleRange();

    END_PROFILE("void CustomFastTableWidget::verticalScrollBarValueChanged(int value)")
}

void CustomFastTableWidget::updateBarsRanges()
{
    START_PROFILE

    QSize areaSize = viewport()->size();

    horizontalScrollBar()->setPageStep(mTotalWidth);
    verticalScrollBar()->setPageStep(mTotalHeight);

    horizontalScrollBar()->setRange(0, mTotalWidth - areaSize.width());
    verticalScrollBar()->setRange(0, mTotalHeight - areaSize.height());

    END_PROFILE("void CustomFastTableWidget::updateBarsRanges()")
}

void CustomFastTableWidget::updateVisibleRange()
{
    START_PROFILE

    if (mRowCount==0 || mColumnCount==0)
    {
        mVisibleLeft=-1;
        mVisibleRight=-1;
        mVisibleTop=-1;
        mVisibleBottom=-1;
    }
    else
    {
        if (
            mVisibleLeft<0
            ||
            mVisibleRight<0
            ||
            mVisibleTop<0
            ||
            mVisibleBottom<0
           )
        {
            mVisibleLeft=0;
            mVisibleRight=0;
            mVisibleTop=0;
            mVisibleBottom=0;
        }
        else
        {
            if (mVisibleLeft>=mColumnCount)
            {
                mVisibleLeft=mColumnCount-1;
            }
            if (mVisibleRight>=mColumnCount)
            {
                mVisibleRight=mColumnCount-1;
            }
            if (mVisibleTop>=mRowCount)
            {
                mVisibleTop=mRowCount-1;
            }
            if (mVisibleBottom>=mRowCount)
            {
                mVisibleBottom=mRowCount-1;
            }
        }

        int minX=horizontalScrollBar()->value();
        int minY=verticalScrollBar()->value();
        int maxX=minX+viewport()->width();
        int maxY=minY+viewport()->height();

        while (mVisibleLeft<mColumnCount-1 && mOffsetX.at(mVisibleLeft)<minX && mOffsetX.at(mVisibleLeft)+mColumnWidths.at(mVisibleLeft)<minX)
        {
            mVisibleLeft++;
        }

        while (mVisibleLeft>0 && mOffsetX.at(mVisibleLeft)>minX && mOffsetX.at(mVisibleLeft)+mColumnWidths.at(mVisibleLeft)>minX)
        {
            mVisibleLeft--;
        }

        while (mVisibleRight<mColumnCount-1 && mOffsetX.at(mVisibleRight)<maxX && mOffsetX.at(mVisibleRight)+mColumnWidths.at(mVisibleRight)<maxX)
        {
            mVisibleRight++;
        }

        while (mVisibleRight>0 && mOffsetX.at(mVisibleRight)>maxX && mOffsetX.at(mVisibleRight)+mColumnWidths.at(mVisibleRight)>maxX)
        {
            mVisibleRight--;
        }

        while (mVisibleTop<mRowCount-1 && mOffsetY.at(mVisibleTop)<minY && mOffsetY.at(mVisibleTop)+mRowHeights.at(mVisibleTop)<minY)
        {
            mVisibleTop++;
        }

        while (mVisibleTop>0 && mOffsetY.at(mVisibleTop)>minY && mOffsetY.at(mVisibleTop)+mRowHeights.at(mVisibleTop)>minY)
        {
            mVisibleTop--;
        }

        while (mVisibleBottom<mRowCount-1 && mOffsetY.at(mVisibleBottom)<maxY && mOffsetY.at(mVisibleBottom)+mRowHeights.at(mVisibleBottom)<maxY)
        {
            mVisibleBottom++;
        }

        while (mVisibleBottom>0 && mOffsetY.at(mVisibleBottom)>maxY && mOffsetY.at(mVisibleBottom)+mRowHeights.at(mVisibleBottom)>maxY)
        {
            mVisibleBottom--;
        }
    }

    END_PROFILE("void CustomFastTableWidget::updateVisibleRange()")
}

int CustomFastTableWidget::rowCount()
{
    return mRowCount;
}

void CustomFastTableWidget::setRowCount(int count)
{
    START_PROFILE

    if (count<0)
    {
        count=0;
    }

    if (mRowCount!=count)
    {
        while (mRowCount<count)
        {
            QStringList aNewRow;
            QList<QBrush *> aNewRowBrush;
            QList<QColor *> aNewRowColor;
            QList<QFont *> aNewRowFont;
            QList<bool> aNewRowbool;
            QList<int> aNewRowint;
            QList<quint16> aNewRowqint16;

            mTotalHeight+=mDefaultHeight;

            mData.append(aNewRow);
            mRowHeights.append(mDefaultHeight);
            mOffsetY.append(mRowCount==0? 0 : (mOffsetY.at(mRowCount-1)+mRowHeights.at(mRowCount-1)));
            mSelectedCells.append(aNewRowbool);

            for (int i=0; i<mColumnCount; ++i)
            {
                mData[mRowCount].append("");
                mSelectedCells[mRowCount].append(false);
            }

            mRowCount++;
        }

        while (mRowCount>count)
        {
            mRowCount--;

            mTotalHeight-=mRowHeights.at(mRowCount);

            mData.removeLast();
            mRowHeights.removeLast();
            mOffsetY.removeLast();

            for (int i=0; i<mColumnCount; ++i)
            {
                if (mSelectedCells.last().at(i))
                {
                    for (int j=0; j<mCurSelection.length(); ++j)
                    {
                        if (mCurSelection.at(j).y()==mRowCount && mCurSelection.at(j).x()==i)
                        {
                            mCurSelection.removeAt(j);
                            break;
                        }
                    }
                }
            }

            mSelectedCells.removeLast();
        }

        updateBarsRanges();
        updateVisibleRange();
    }

    END_PROFILE("void CustomFastTableWidget::setRowCount(int count)")
}

int CustomFastTableWidget::columnCount()
{
    return mColumnCount;
}

void CustomFastTableWidget::setColumnCount(int count)
{
    START_PROFILE

    if (count<0)
    {
        count=0;
    }

    if (mColumnCount!=count)
    {
        if (mColumnCount<count)
        {
            for (int i=mColumnCount; i<count; ++i)
            {
                mColumnWidths.append(mDefaultWidth);
                mTotalWidth+=mDefaultWidth;

                mOffsetX.append(i==0? 0 : (mOffsetX.at(i-1)+mColumnWidths.at(i-1)));
            }

            for (int i=0; i<mData.length(); ++i)
            {
                for (int j=mColumnCount; j<count; ++j)
                {
                    mData[i].append("");
                    mSelectedCells[i].append(false);
                }
            }
        }
        else
        {
            for (int i=mColumnCount-1; i>=count; --i)
            {
                mTotalWidth-=mColumnWidths.at(i);
                mColumnWidths.removeLast();
                mOffsetX.removeLast();
            }

            for (int i=0; i<mData.length(); ++i)
            {
                for (int j=mColumnCount-1; j>=count; --j)
                {
                    mData[i].removeLast();

                    if (mSelectedCells.at(i).last())
                    {
                        for (int k=0; k<mCurSelection.length(); ++k)
                        {
                            if (mCurSelection.at(k).y()==i && mCurSelection.at(k).x()==j)
                            {
                                mCurSelection.removeAt(k);
                                break;
                            }
                        }
                    }

                    mSelectedCells[i].removeLast();
                }
            }
        }

        mColumnCount=count;

        updateBarsRanges();
        updateVisibleRange();
    }

    END_PROFILE("void CustomFastTableWidget::setColumnCount(int count)")
}

QBrush CustomFastTableWidget::defaultBackgroundBrush()
{
    return mDefaultBackgroundBrush;
}

void CustomFastTableWidget::setDefaultBackgroundBrush(QBrush brush)
{
    START_PROFILE

    mDefaultBackgroundBrush=brush;

    END_PROFILE("void CustomFastTableWidget::setDefaultBackgroundBrush(QBrush brush)")
}

QColor CustomFastTableWidget::defaultForegroundColor()
{
    return mDefaultForegroundColor;
}

void CustomFastTableWidget::setDefaultForegroundColor(QColor color)
{
    START_PROFILE

    mDefaultForegroundColor=color;

    END_PROFILE("void CustomFastTableWidget::setDefaultForegroundColor(QColor color)")
}

QColor CustomFastTableWidget::gridColor()
{
    return mGridColor;
}

void CustomFastTableWidget::setGridColor(QColor color)
{
    START_PROFILE

    mGridColor=color;

    END_PROFILE("void CustomFastTableWidget::setGridColor(QColor color)")
}

QBrush CustomFastTableWidget::selectionBrush()
{
    return mSelectionBrush;
}

void CustomFastTableWidget::setSelectionBrush(QBrush brush)
{
    START_PROFILE

    mSelectionBrush=brush;

    END_PROFILE("void CustomFastTableWidget::setSelectionBrush(QBrush brush)")
}

quint16 CustomFastTableWidget::defaultHeight()
{
    return mDefaultHeight;
}

void CustomFastTableWidget::setDefaultHeight(quint16 height)
{
    START_PROFILE

    mDefaultHeight=height;

    END_PROFILE("void CustomFastTableWidget::setDefaultHeight(quint16 height)")
}

quint16 CustomFastTableWidget::defaultWidth()
{
    return mDefaultWidth;
}

void CustomFastTableWidget::setDefaultWidth(quint16 width)
{
    START_PROFILE

    mDefaultWidth=width;

    END_PROFILE("void CustomFastTableWidget::setDefaultWidth(quint16 width)")
}

int CustomFastTableWidget::totalHeight()
{
    return mTotalHeight;
}

int CustomFastTableWidget::totalWidth()
{
    return mTotalWidth;
}

QRect CustomFastTableWidget::visibleRange()
{
    START_PROFILE

    QRect aRect;
    aRect.setCoords(mVisibleLeft, mVisibleTop, mVisibleRight, mVisibleBottom);

    END_PROFILE("QRect CustomFastTableWidget::visibleRange()")

    return aRect;
}

QString CustomFastTableWidget::text(const int row, const int column)
{
    return mData.at(row).at(column);
}

void CustomFastTableWidget::setText(const int row, const int column, const QString text)
{
    START_FREQUENT_PROFILE

    mData[row][column]=text;

    END_FREQUENT_PROFILE("void CustomFastTableWidget::setText(const int row, const int column, const QString text)")
}

quint16 CustomFastTableWidget::rowHeight(const int row)
{
    return mRowHeights.at(row);
}

void CustomFastTableWidget::setRowHeight(const int row, const quint16 height)
{
    START_PROFILE

    if (mRowHeights.at(row)!=height)
    {
        int aDiff=height-mRowHeights.at(row);

        mTotalHeight+=aDiff;
        mRowHeights[row]=height;

        for (int i=row+1; i<mRowCount; ++i)
        {
            mOffsetY[i]=mOffsetY.at(i)+aDiff;
        }

        updateBarsRanges();
        updateVisibleRange();
    }

    END_PROFILE("void CustomFastTableWidget::setRowHeight(const int row, const quint16 height)")
}

quint16 CustomFastTableWidget::columnWidth(const int column)
{
    return mColumnWidths.at(column);
}

void CustomFastTableWidget::setColumnWidth(const int column, const quint16 width)
{
    START_PROFILE

    if (mColumnWidths.at(column)!=width)
    {
        int aDiff=width-mColumnWidths.at(column);

        mTotalWidth+=aDiff;
        mColumnWidths[column]=width;

        for (int i=column+1; i<mColumnCount; ++i)
        {
            mOffsetX[i]=mOffsetX.at(i)+aDiff;
        }

        updateBarsRanges();
        updateVisibleRange();
    }

    END_PROFILE("void CustomFastTableWidget::setColumnWidth(const int column, const quint16 width)")
}

bool CustomFastTableWidget::cellSelected(const int row, const int column)
{
    return mSelectedCells.at(row).at(column);
}

void CustomFastTableWidget::setCellSelected(const int row, const int column, const bool selected)
{
    START_PROFILE

    if (mSelectedCells.at(row).at(column)!=selected)
    {
        mSelectedCells[row][column]=selected;

        if (selected)
        {
            mCurSelection.append(QPoint(column, row));
        }
        else
        {
            for (int i=0; i<mCurSelection.length(); ++i)
            {
                if (mCurSelection.at(i).y()==row && mCurSelection.at(i).x()==column)
                {
                    mCurSelection.removeAt(i);
                    break;
                }
            }
        }
    }

    END_PROFILE("void CustomFastTableWidget::setCellSelected(const int row, const int column, const bool selected)")
}
