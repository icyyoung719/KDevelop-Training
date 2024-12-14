#ifndef __KX_HEX_H_
#define __KX_HEX_H_

#include "kxshape.h"


//class KxRect : public KxShape
//{
//public:
//	KxRect() = default;
//	virtual ~KxRect();
//	virtual void drawShape(QPaintDevice* parent /* = Q_NULLPTR */) override;
//};
class KxHex : public KxShape
{
public:
    KxHex() = default;
    virtual ~KxHex();
    virtual void drawShape(QPaintDevice* parent /* = Q_NULLPTR */) override;
};

#endif