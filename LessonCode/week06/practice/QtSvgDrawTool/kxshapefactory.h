#ifndef __KX_FACTORY_H_
#define __KX_FACTORY_H_

#include "kxcanvas.h"
#include "kxrect.h"
#include "kxline.h"
#include "kxhex.h"

#include <QObject>

class KxShapeFactory : public QObject
{
	Q_OBJECT
public:
	explicit KxShapeFactory(QObject* parent = nullptr);
	~KxShapeFactory();
	static std::unique_ptr<KxShape> createShape(KxCanvas::KDrawFlag drawFlag);
};

#endif