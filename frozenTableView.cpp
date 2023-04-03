#include "frozenTableView.h"
#include <QScrollBar>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDebug>

frozenTableView::frozenTableView(QWidget *parent):
    QTableView(parent),
    m_lastFrozenColumn(0)
{
    m_frozenView = new QTableView(this);

    QHeaderView *header = verticalHeader();
    header->hide();
    connect(horizontalHeader(), &QHeaderView::sectionResized, this, &frozenTableView::updateSectionWidth);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged, m_frozenView->verticalScrollBar(), &QAbstractSlider::setValue);
    connect(m_frozenView->verticalScrollBar(), &QAbstractSlider::valueChanged, verticalScrollBar(), &QAbstractSlider::setValue);

}

frozenTableView::~frozenTableView()
{
    delete m_frozenView;
}

void frozenTableView::initFrozenView()
{
    if(model() == nullptr)
    {
        qDebug() << QString("[%1:%2] model is not added").arg(__func__).arg(__LINE__);
    }
    // disable frozen table scroll bar
    QHeaderView *header = m_frozenView->horizontalHeader();
    m_frozenView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_frozenView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_frozenView->verticalHeader()->hide();

    //set model
    m_frozenView->setModel(model());    
    m_frozenView->setSelectionModel(selectionModel());
    //disable focus, resize, selection
    m_frozenView->setFocusPolicy(Qt::NoFocus);
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setSelectionMode(QAbstractItemView::NoSelection);
    viewport()->stackUnder(m_frozenView);
    m_frozenView->setStyleSheet("QTableView {background-color: #8EDE21;"
                                "selection-background-color: #22DE21;"
                                "gridline-color : gray;"
                                "border : 1px;}"
                                "QTableView::item {height : 50px;}");

    //disable moveable, clickable, auto scroll
    header->setSectionsMovable(false);
    header->setSectionsClickable(false);
    setAutoScroll(false);
    m_frozenView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //set scrollMode
    setHorizontalScrollMode(ScrollPerPixel);
    setVerticalScrollMode(ScrollPerPixel);
    m_frozenView->setVerticalScrollMode(ScrollPerPixel);
    updateFrozenViewGeometry();
    updateFrozenColumn();
}

void frozenTableView::updateFrozenColumn()
{
    for(int col = 0; col < model()->columnCount(); ++col)
    {
        if(col < m_lastFrozenColumn) m_frozenView->setColumnHidden(col, false);
        else m_frozenView->setColumnHidden(col, true);
    }
    if(m_lastFrozenColumn > 0) m_frozenView->show();
}

void frozenTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    updateFrozenViewGeometry();
}

void frozenTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    QHeaderView *header = horizontalHeader();

    setStyleSheet("QTableView {background-color:green}"
                  "QTableView::item {"
                  "height : 50px};"
                  "border : 1px");
    header->setSectionsMovable(false);
    header->setSectionsClickable(false);
    header->setStretchLastSection(false);
    header->setSectionResizeMode(QHeaderView::Fixed);

    initFrozenView();
}

QModelIndex frozenTableView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex current =  QTableView::moveCursor(cursorAction, modifiers);
    const int x = visualRect(current).topLeft().x();
    if(cursorAction == MoveLeft && x < m_frozenView->columnWidth(0))
    {
        const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x() - m_frozenView->columnWidth(0);
        horizontalScrollBar()->setValue(newValue);
    }
    return current;
}

void frozenTableView::updateFrozenViewGeometry()
{
    const int x = frameWidth();
    const int y = frameWidth();
    int w = 0;
    int h = viewport()->height() + horizontalHeader()->height();
    for(int col = 0; col < m_lastFrozenColumn; ++col)
    {
        w += m_frozenView->columnWidth(col);
    }
    m_frozenView->setGeometry(x, y, w, h);
}

void frozenTableView::updateFrozenView()
{
    updateFrozenColumn();
    updateFrozenViewGeometry();
}

void frozenTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
    updateFrozenView();
    qDebug() << QString("original : %1, frozen : %2").arg(columnWidth(0)).arg(m_frozenView->columnWidth(0));
}

void frozenTableView::updateSectionWidth(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize)
    m_frozenView->setColumnWidth(logicalIndex, newSize);
    updateFrozenViewGeometry();
}

int frozenTableView::lastFrozenColumn() const
{
    return m_lastFrozenColumn;
}

void frozenTableView::setLastFrozenColumn(int lastFrozenColumn)
{
    m_lastFrozenColumn = lastFrozenColumn;
    updateFrozenView();
}
