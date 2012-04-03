#ifndef FASTTABLEWIDGET_H
#define FASTTABLEWIDGET_H

#include <QAbstractScrollArea>
#include <QTableWidget>
#include <QList>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QPainter>
#include <QScrollBar>
#include <QDateTime>

#include "qdebug.h"

#define FASTTABLE_DEBUG

#if 0
    #define START_PROFILE      qint64 profileStart=QDateTime::currentMSecsSinceEpoch();
    #define END_PROFILE(text)  qDebug()<<text" :"<<QDateTime::currentMSecsSinceEpoch()-profileStart;
#else
    #define START_PROFILE
    #define END_PROFILE(text)
#endif

#if 0
    #define START_FREQUENT_PROFILE      START_PROFILE
    #define END_FREQUENT_PROFILE(text)  END_PROFILE(text)
#else
    #define START_FREQUENT_PROFILE
    #define END_FREQUENT_PROFILE(text)
#endif

//------------------------------------------------------------------------------

class FastTableWidget : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(int     rowCount               READ rowCount               WRITE setRowCount)
    Q_PROPERTY(int     columnCount            READ columnCount            WRITE setColumnCount)
    Q_PROPERTY(quint16 defaultHeight          READ defaultHeight          WRITE setDefaultHeight)
    Q_PROPERTY(quint16 defaultWidth           READ defaultWidth           WRITE setDefaultWidth)
    Q_PROPERTY(QBrush  defaultBackgroundBrush READ defaultBackgroundBrush WRITE setDefaultBackgroundBrush)
    Q_PROPERTY(QColor  defaultForegroundColor READ defaultForegroundColor WRITE setDefaultForegroundColor)
    Q_PROPERTY(QColor  gridColor              READ gridColor              WRITE setGridColor)
    Q_PROPERTY(QBrush  selectionBrush         READ selectionBrush         WRITE setSelectionBrush)
    Q_PROPERTY(int     totalHeight            READ totalHeight)
    Q_PROPERTY(int     totalWidth             READ totalWidth)

public:
    explicit FastTableWidget(QWidget *parent = 0);
    ~FastTableWidget();

    void clearTable();

    void resetBackgroundBrushes();
    void resetForegroundColors();
    void resetFonts();
    void resetTextFlags();

    void resetBackgroundBrush(const int row, const int column);
    void resetForegroundColor(const int row, const int column);
    void resetFont(const int row, const int column);
    void resetTextFlag(const int row, const int column);

    void selectRow(const int row);
    void unselectRow(const int row);

    void selectColumn(const int column);
    void unselectColumn(const int column);

    void selectAll();
    void unselectAll();

    int rowCount();
    void setRowCount(int count);

    int columnCount();
    void setColumnCount(int count);

    QBrush defaultBackgroundBrush();
    void setDefaultBackgroundBrush(QBrush brush);

    QColor defaultForegroundColor();
    void setDefaultForegroundColor(QColor color);

    QColor gridColor();
    void setGridColor(QColor color);

    QBrush selectionBrush();
    void setSelectionBrush(QBrush brush);

    quint16 defaultHeight();
    void setDefaultHeight(quint16 height);

    quint16 defaultWidth();
    void setDefaultWidth(quint16 width);

    int totalHeight();
    int totalWidth();

    QRect visibleRange();

    QString text(const int row, const int column);
    void setText(const int row, const int column, const QString text);

    quint16 rowHeight(const int row);
    void setRowHeight(const int row, const quint16 height);

    quint16 columnWidth(const int column);
    void setColumnWidth(const int column, const quint16 width);

    QBrush backgroundBrush(const int row, const int column);
    void setBackgroundBrush(const int row, const int column, const QBrush brush);

    QColor foregroundColor(const int row, const int column);
    void setForegroundColor(const int row, const int column, const QColor color);

    QFont cellFont(const int row, const int column);
    void setCellFont(const int row, const int column, const QFont font);

    int cellTextFlags(const int row, const int column);
    void setCellTextFlags(const int row, const int column, const int flags);

    bool cellSelected(const int row, const int column);
    void setCellSelected(const int row, const int column, const bool selected);

    void clearSpans();
    void setSpan(const int row, const int column, quint16 rowSpan, quint16 columnSpan);
    quint16 rowSpan(const int row, const int column);
    quint16 columnSpan(const int row, const int column);
    QPoint spanParent(const int row, const int column);

protected:
    int mRowCount;
    int mColumnCount;

    quint16 mDefaultHeight;
    quint16 mDefaultWidth;
    int mTotalHeight;
    int mTotalWidth;

    QBrush mDefaultBackgroundBrush;
    QColor mDefaultForegroundColor;
    QColor mGridColor;
    QBrush mSelectionBrush;

    int mVisibleLeft;
    int mVisibleRight;
    int mVisibleTop;
    int mVisibleBottom;

    QList< QStringList > mData;
    QList< quint16 > mRowHeights;
    QList< quint16 > mColumnWidths;
    QList< int > mOffsetX;
    QList< int > mOffsetY;
    QList< QList<bool> > mSelectedCells;
    QList< QPoint > mCurSelection;
    QList< QList<QBrush *> > mBackgroundBrushes;
    QList< QList<QColor *> > mForegroundColors;
    QList< QList<QFont *> > mCellFonts;
    QList< QList<int> > mCellTextFlags;
    QList< QList<quint16> > mCellMergeX;
    QList< QList<quint16> > mCellMergeY;
    QList< QList<int> > mCellMergeParentRow;
    QList< QList<int> > mCellMergeParentColumn;

    bool mStartSelection;

    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void paintCell(QPainter &painter, const int x, const int y, const int width, const int height, const int row, const int column);

    void updateBarsRanges();
    void updateVisibleRange();

protected slots:
    void horizontalScrollBarValueChanged(int value);
    void verticalScrollBarValueChanged(int value);
};

#endif // FASTTABLEWIDGET_H
