#ifndef __KX_LINE_H_
#define __KX_LINE_H_

#include "kxshape.h"

class KxLine : public KxShape
{
public:
	KxLine() = default;
	virtual ~KxLine();
	virtual void drawShape(QPaintDevice* parent /* = Q_NULLPTR */) override;
};
#endif