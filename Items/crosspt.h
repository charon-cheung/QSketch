#ifndef CROSSPT_H
#define CROSSPT_H

#include "baseitem.h"

class CrossPt : public BaseItem
{
public:
    explicit CrossPt(QGraphicsItem *parent = Q_NULLPTR);
    ~CrossPt();

    enum {Type = UserType + 2};
    int type() const Q_DECL_OVERRIDE;
    enum {
        size = 2,
    };

protected:
    void DrawShape(QPainter* painter);
};

#endif // CROSSPT_H
