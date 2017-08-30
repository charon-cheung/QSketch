#ifndef CIRCLEPT_H
#define CIRCLEPT_H
#include "baseitem.h"

class CirclePt : public BaseItem
{
public:
    explicit CirclePt(QGraphicsItem *parent = Q_NULLPTR);
    ~CirclePt();

    enum {Type = UserType + 1};
    int type() const Q_DECL_OVERRIDE;
    enum {
        size = 2,
    };

protected:
    void DrawShape(QPainter* painter);
};

#endif // CIRCLEPT_H
