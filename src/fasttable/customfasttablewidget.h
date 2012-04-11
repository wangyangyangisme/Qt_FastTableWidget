#ifndef CUSTOMCustomFastTableWidget_H
#define CUSTOMCustomFastTableWidget_H

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

class CustomFastTableWidget : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(int     rowCount                                READ rowCount                                WRITE setRowCount)
    Q_PROPERTY(int     columnCount                             READ columnCount                             WRITE setColumnCount)
    Q_PROPERTY(qint16  horizontalHeader_RowCount               READ horizontalHeader_RowCount               WRITE horizontalHeader_SetRowCount)
    Q_PROPERTY(qint16  verticalHeader_ColumnCount              READ verticalHeader_ColumnCount              WRITE verticalHeader_SetColumnCount)
    Q_PROPERTY(quint16 defaultWidth                            READ defaultWidth                            WRITE setDefaultWidth)
    Q_PROPERTY(quint16 defaultHeight                           READ defaultHeight                           WRITE setDefaultHeight)
    Q_PROPERTY(int     totalWidth                              READ totalWidth)
    Q_PROPERTY(int     totalHeight                             READ totalHeight)
    Q_PROPERTY(int     verticalHeader_TotalWidth               READ verticalHeader_TotalWidth)
    Q_PROPERTY(int     horizontalHeader_TotalHeight            READ horizontalHeader_TotalHeight)
    Q_PROPERTY(QBrush  defaultBackgroundBrush                  READ defaultBackgroundBrush                  WRITE setDefaultBackgroundBrush)
    Q_PROPERTY(QColor  defaultForegroundColor                  READ defaultForegroundColor                  WRITE setDefaultForegroundColor)
    Q_PROPERTY(QColor  gridColor                               READ gridColor                               WRITE setGridColor)
    Q_PROPERTY(QBrush  horizontalHeader_DefaultBackgroundBrush READ horizontalHeader_DefaultBackgroundBrush WRITE horizontalHeader_SetDefaultBackgroundBrush)
    Q_PROPERTY(QColor  horizontalHeader_DefaultForegroundColor READ horizontalHeader_DefaultForegroundColor WRITE horizontalHeader_SetDefaultForegroundColor)
    Q_PROPERTY(QColor  horizontalHeader_GridColor              READ horizontalHeader_GridColor              WRITE horizontalHeader_SetGridColor)
    Q_PROPERTY(QBrush  verticalHeader_DefaultBackgroundBrush   READ verticalHeader_DefaultBackgroundBrush   WRITE verticalHeader_SetDefaultBackgroundBrush)
    Q_PROPERTY(QColor  verticalHeader_DefaultForegroundColor   READ verticalHeader_DefaultForegroundColor   WRITE verticalHeader_SetDefaultForegroundColor)
    Q_PROPERTY(QColor  verticalHeader_GridColor                READ verticalHeader_GridColor                WRITE verticalHeader_SetGridColor)
    Q_PROPERTY(QBrush  selectionBrush                          READ selectionBrush                          WRITE setSelectionBrush)

public:

    enum DrawComponent {DrawCell, DrawHorizontalHeaderCell, DrawVerticalHeaderCell, DrawTopLeftCorner};

    explicit CustomFastTableWidget(QWidget *parent = 0);
    ~CustomFastTableWidget();

    virtual void clearTable();

    virtual void selectRow(const int row);
    virtual void unselectRow(const int row);

    virtual void selectColumn(const int column);
    virtual void unselectColumn(const int column);

    void selectAll();
    void unselectAll();

    void addRow();
    virtual void insertRow(int row);
    virtual void deleteRow(int row);

    void addColumn();
    virtual void insertColumn(int column);
    virtual void deleteColumn(int column);

    void horizontalHeader_AddRow();
    virtual void horizontalHeader_InsertRow(int row);
    virtual void horizontalHeader_DeleteRow(int row);

    void verticalHeader_AddColumn();
    virtual void verticalHeader_InsertColumn(int column);
    virtual void verticalHeader_DeleteColumn(int column);

    int rowCount();
    void setRowCount(int count);

    int columnCount();
    void setColumnCount(int count);

    qint16 horizontalHeader_RowCount();
    void horizontalHeader_SetRowCount(qint16 count);

    qint16 verticalHeader_ColumnCount();
    void verticalHeader_SetColumnCount(qint16 count);

    QBrush defaultBackgroundBrush();
    void setDefaultBackgroundBrush(QBrush brush);

    QColor defaultForegroundColor();
    void setDefaultForegroundColor(QColor color);

    QColor gridColor();
    void setGridColor(QColor color);

    QBrush horizontalHeader_DefaultBackgroundBrush();
    void horizontalHeader_SetDefaultBackgroundBrush(QBrush brush);

    QColor horizontalHeader_DefaultForegroundColor();
    void horizontalHeader_SetDefaultForegroundColor(QColor color);

    QColor horizontalHeader_GridColor();
    void horizontalHeader_SetGridColor(QColor color);

    QBrush verticalHeader_DefaultBackgroundBrush();
    void verticalHeader_SetDefaultBackgroundBrush(QBrush brush);

    QColor verticalHeader_DefaultForegroundColor();
    void verticalHeader_SetDefaultForegroundColor(QColor color);

    QColor verticalHeader_GridColor();
    void verticalHeader_SetGridColor(QColor color);

    QBrush selectionBrush();
    void setSelectionBrush(QBrush brush);

    quint16 defaultWidth();
    void setDefaultWidth(quint16 width);

    quint16 defaultHeight();
    void setDefaultHeight(quint16 height);

    quint16 columnWidth(const int column);
    void setColumnWidth(const int column, const quint16 width);

    quint16 rowHeight(const int row);
    void setRowHeight(const int row, const quint16 height);

    quint16 verticalHeader_ColumnWidth(const int column);
    void verticalHeader_SetColumnWidth(const int column, const quint16 width);

    quint16 horizontalHeader_RowHeight(const int row);
    void horizontalHeader_SetRowHeight(const int row, const quint16 height);

    int totalWidth();
    int totalHeight();
    int verticalHeader_TotalWidth();
    int horizontalHeader_TotalHeight();

    QRect visibleRange();
    QRect horizontalHeader_VisibleRange();
    QRect verticalHeader_VisibleRange();

    QString text(const int row, const int column);
    void setText(const int row, const int column, const QString text);

    QString horizontalHeader_Text(const int row, const int column);
    QString horizontalHeader_Text(const int column);
    void horizontalHeader_SetText(const int row, const int column, const QString text);
    void horizontalHeader_SetText(const int column, const QString text);

    QString verticalHeader_Text(const int row, const int column);
    QString verticalHeader_Text(const int row);
    void verticalHeader_SetText(const int row, const int column, const QString text);
    void verticalHeader_SetText(const int row, const QString text);

    virtual bool cellSelected(const int row, const int column);
    virtual void setCellSelected(const int row, const int column, const bool selected);

protected:
    int mRowCount;
    int mColumnCount;
    qint16 mHorizontalHeader_RowCount;
    qint16 mVerticalHeader_ColumnCount;

    QBrush mDefaultBackgroundBrush;
    QColor mDefaultForegroundColor;
    QColor mGridColor;

    QBrush mHorizontalHeader_DefaultBackgroundBrush;
    QColor mHorizontalHeader_DefaultForegroundColor;
    QColor mHorizontalHeader_GridColor;

    QBrush mVerticalHeader_DefaultBackgroundBrush;
    QColor mVerticalHeader_DefaultForegroundColor;
    QColor mVerticalHeader_GridColor;

    QBrush mSelectionBrush;

    quint16 mDefaultWidth;
    quint16 mDefaultHeight;
    int mTotalWidth;
    int mTotalHeight;
    int mVerticalHeader_TotalWidth;
    int mHorizontalHeader_TotalHeight;

    int mVisibleLeft;
    int mVisibleRight;
    int mVisibleTop;
    int mVisibleBottom;

    int mHorizontalHeader_VisibleLeft;
    int mHorizontalHeader_VisibleRight;
    int mHorizontalHeader_VisibleTop;
    int mHorizontalHeader_VisibleBottom;

    int mVerticalHeader_VisibleLeft;
    int mVerticalHeader_VisibleRight;
    int mVerticalHeader_VisibleTop;
    int mVerticalHeader_VisibleBottom;

    QList< QStringList > mData;
    QList< quint16 >     mRowHeights;
    QList< quint16 >     mColumnWidths;
    QList< int >         mOffsetX;
    QList< int >         mOffsetY;

    QList< QStringList > mHorizontalHeader_Data;
    QList< quint16 >     mHorizontalHeader_RowHeights;
    QList< int >         mHorizontalHeader_OffsetX;
    QList< int >         mHorizontalHeader_OffsetY;

    QList< QStringList > mVerticalHeader_Data;
    QList< quint16 >     mVerticalHeader_ColumnWidths;
    QList< int >         mVerticalHeader_OffsetX;
    QList< int >         mVerticalHeader_OffsetY;

    QList< QList<bool> > mSelectedCells;
    QList< QPoint >      mCurSelection;
    QList< bool >        mHorizontalHeader_SelectedColumns;
    QList< bool >        mVerticalHeader_SelectedRows;

    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void paintCell(QPainter &painter, const int x, const int y, const int width, const int height, const int row, const int column, const DrawComponent drawComponent);

    void updateBarsRanges();
    virtual void updateVisibleRange();

protected slots:
    void horizontalScrollBarValueChanged(int value);
    void verticalScrollBarValueChanged(int value);
};

#endif // CUSTOMCustomFastTableWidget_H
