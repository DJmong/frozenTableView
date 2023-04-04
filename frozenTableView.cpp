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

    QHeaderView *vHeader = verticalHeader();
    vHeader->hide();
    connect(horizontalHeader(), &QHeaderView::sectionResized, this, &frozenTableView::updateSectionWidth);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged, m_frozenView->verticalScrollBar(), &QAbstractSlider::setValue);
    connect(m_frozenView->verticalScrollBar(), &QAbstractSlider::valueChanged, verticalScrollBar(), &QAbstractSlider::setValue);
    QHeaderView *hHeader = horizontalHeader();

    setStyleSheet("QTableView {background-color:green}"
                  "QTableView::item {"
                  "height : 50px};"
                  "border : 1px");
    hHeader->setSectionsMovable(false);
    hHeader->setSectionsClickable(false);
    hHeader->setStretchLastSection(false);
    hHeader->setSectionResizeMode(QHeaderView::Fixed);
    initFrozenView();
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
    //set model
    m_frozenView->setModel(model);
    m_frozenView->setSelectionModel(selectionModel());
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

void frozenTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
    updateFrozenColumn();
    updateFrozenViewGeometry();
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
    update();
}
