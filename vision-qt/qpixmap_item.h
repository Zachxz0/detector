#ifndef QPIXMAP_ITEM_H
#define QPIXMAP_ITEM_H
#include "QPixmap"
#include "QStyledItemDelegate"
#include "QObject"
class QPixmapItemdele : public QStyledItemDelegate
{
public:
    QPixmapItemdele(QObject* parent = 0):QStyledItemDelegate(parent){ }
    //在委托类的paint中画图
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if(index.data(Qt::DisplayRole).canConvert<QPixmap>())
        {
            QPixmap pix = index.data(Qt::DisplayRole).value<QPixmap>();
            painter->drawPixmap(option.rect,pix);
        }
        QStyledItemDelegate::paint(painter,option,index);
    }
};


#endif // QPIXMAP_ITEM_H
