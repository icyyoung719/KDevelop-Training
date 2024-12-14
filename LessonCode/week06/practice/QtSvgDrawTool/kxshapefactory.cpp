#include "kxshapefactory.h"

KxShapeFactory::KxShapeFactory(QObject* parent /* = nullptr */)
	:QObject(parent)
{

}

KxShapeFactory::~KxShapeFactory()
{
}

std::unique_ptr<KxShape> KxShapeFactory::createShape(KxCanvas::KDrawFlag drawFlag)
{
	switch (drawFlag)
	{
	case KxCanvas::KDrawFlag::RectDrawFlag:
		return std::make_unique<KxRect>();
	case KxCanvas::KDrawFlag::LineDrawFlag:
        return std::make_unique<KxLine>();
	case KxCanvas::KDrawFlag::HexDrawFlag:
		return std::make_unique<KxHex>();
	//case KxCanvas::KDrawFlag::CircleDrawFlag:
 //       return std::make_unique<Kx>();

	default:
		break;
	}
}

