#include "qschematicspart.h"

QSchematicsPart::QSchematicsPart(symbol::SchematicsSymbol* obj, QString name, int x, int y, QGraphicsItem *parent) :
	QGraphicsItem(parent),
	m_dragMode(false)
{
	m_name = name;

	m_externalRect = obj->createRect();
	m_partFrame = new QGraphicsRectItem(m_externalRect,this);

	setPos(x,y);

	m_pins = obj->createPins(this);

	m_partLabel = new QGraphicsSimpleTextItem(m_name,this);
	m_partLabel->setPos(0,20);
	m_partLabel->setScale(2);

	m_typeLabel = new QGraphicsSimpleTextItem(obj->getName(),this);
	m_typeLabel->setPos(0,-20);
	m_typeLabel->setScale(2);

	setBoundingRect();

	setFlags(QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsMovable);
}

QSchematicsPart::QSchematicsPart(symbol::SchematicsSymbol* obj, int x, int y, QGraphicsItem *parent) :
	QGraphicsItem(parent),
	m_dragMode(false)
{
	m_name = obj->getPrefix();

	m_externalRect = obj->createRect();
	m_partFrame = new QGraphicsRectItem(m_externalRect,this);

	setPos(x,y);

	m_pins = obj->createPins(this);

	m_partLabel = new QGraphicsSimpleTextItem(m_name,this);
	m_partLabel->setPos(0,20);
	m_partLabel->setScale(2);

	m_typeLabel = new QGraphicsSimpleTextItem(obj->getName(),this);
	m_typeLabel->setPos(0,-20);
	m_typeLabel->setScale(2);

	qDebug() << obj->getName();
	setBoundingRect();

	setFlags(QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsMovable);
}

void QSchematicsPart::setBoundingRect()
{
	m_xmin = m_externalRect.x();
	m_ymin = m_externalRect.y();
	m_xmax = m_xmin+m_externalRect.width();
	m_ymax = m_ymin+m_externalRect.height();

	foreach(QSchematicsPin* p, m_pins) {
		if(p->getX()>m_xmax) m_xmax = p->getX();
		if(p->getY()>m_ymax) m_ymax = p->getY();
		if(p->getX()<m_xmin) m_xmin = p->getX();
		if(p->getY()<m_ymin) m_ymin = p->getY();
	}

	m_boundingRect = QRect(m_xmin,m_ymin,width(),height());
}

QRectF QSchematicsPart::boundingRect() const
{
	return m_boundingRect;
}

qreal QSchematicsPart::width()
{
	return m_xmax-m_xmin;
}

qreal QSchematicsPart::height()
{
	return m_ymax-m_ymin;
}

void QSchematicsPart::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

bool QSchematicsPart::setDragMode(bool m)
{
	if(m && !m_dragMode) {
		m_lastOrig = pos();
	}
	m_dragMode = m;
}

bool QSchematicsPart::contains(const QPointF &point) const
{
	QPointF offsetOrig;
	offsetOrig.setX(point.x()-x());
	offsetOrig.setY(point.y()-y());

	return m_externalRect.contains(offsetOrig);
}

void QSchematicsPart::updateMovingOffset(qreal dx, qreal dy)
{
	setPos(m_lastOrig.x()+dx,m_lastOrig.y()+dy);
}

void QSchematicsPart::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << __FUNCTION__;
}

void QSchematicsPart::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << __FUNCTION__;
}

void QSchematicsPart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << __FUNCTION__;
}
