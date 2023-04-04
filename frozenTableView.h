#ifndef GRAPHTREE_H
#define GRAPHTREE_H

#include <QTableView>
#include <QDebug>

class frozenTableView : public QTableView
{
public:
    explicit frozenTableView(QWidget *parent = 0);
    ~frozenTableView();

    // setter/getter
    void setModel(QAbstractItemModel *model);
    int lastFrozenColumn() const;
    void setLastFrozenColumn(int lastFrozenColumn);
protected:
    void resizeEvent(QResizeEvent *event) override;
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;

    void paintEvent(QPaintEvent *event);
private:
    QTableView *m_frozenView;
    void initFrozenView();
    void updateFrozenViewGeometry();
    void updateFrozenColumn();
private slots:
    void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
    int m_lastFrozenColumn;
};

#endif // GRAPHTREE_H
