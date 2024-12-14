#ifndef __KX_RECT_H_
#define __KX_RECT_H_

#include "kxshape.h"


class KxRect : public KxShape
{
public:
	KxRect() = default;
	virtual ~KxRect();
	virtual void drawShape(QPaintDevice* parent /* = Q_NULLPTR */) override;
};


#endif