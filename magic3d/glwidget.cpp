#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent):
	QGLWidget(parent),
	m_mainWindow(parent),
	m_fScale(1.0),
	m_offsetX(0),
	m_offsetY(0),
	magicdata(NULL),
	lastOrient(ORIENT_NONE),
	m_angle1(0),
	m_angle2(0)
{
	setFocusPolicy(Qt::StrongFocus);
}

void GLWidget::loadFile(QString file)
{
	magic::layer_rects_t layers;
	magic::rects_t layer;

	filePath = file;
	if(magicdata) delete magicdata;
	magicdata = new magic::MagicData(file);

	layers = magicdata->getRectangles();

	m_highestX = 0;
	m_highestY = 0;
	foreach(QString layerN, layers.keys()) {
		layer = layers[layerN];
		foreach (magic::rect_t e, layer)
		{
			if(e.x1 > m_highestX) {
				m_highestX = e.x1;
			}
			if(e.x2 > m_highestX) {
				m_highestX = e.x2;
			}
			if(e.y1 > m_highestY) {
				m_highestY = e.y1;
			}
			if(e.y2 > m_highestY) {
				m_highestY = e.y2;
			}
		}
	}

	m_lowestX = m_highestX;
	m_lowestY = m_highestY;
	foreach(QString layerN, layers.keys()) {
		layer = layers[layerN];
		foreach (magic::rect_t e, layer)
		{
			if(e.x1 < m_lowestX) {
				m_lowestX = e.x1;
			}
			if(e.x2 < m_lowestX) {
				m_lowestX = e.x2;
			}
			if(e.y1 < m_lowestY) {
				m_lowestY = e.y1;
			}
			if(e.y2 < m_lowestY) {
				m_lowestY = e.y2;
			}
		}
	}

}

void GLWidget::paintGL()
{
	glPushMatrix();
	glLoadIdentity();
	glRotatef( m_angle1, 1.0f, 0.0f, 0.0f );
	glRotatef( m_angle2, 0.0f, 1.0f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);
	addModules();
	addWires();
	glPopMatrix();
}

void GLWidget::addModules()
{
	int x,y,w,h;
	QString macro_name;
	magic::mods_t mods = magicdata->getModules();
	foreach (magic::module_info e, mods)
	{
		// fill in LEF library content:
		lef::LEFMacro *macro;
		lef::LEFPin *pin;
		lef::LEFPort *port;
		lef::LEFLayer *layer;

		macro_name = e.module_name;
		x = e.x1;
		y = e.y1;
		w = e.a*(e.x2-e.x1);
		h = e.e*(e.y2-e.y1);

		/*if(project) if(project->isDefinedMacro(macro_name)) {
			macro = project->getMacro(macro_name);

			foreach(pin, macro->getPins()) {
				port = pin->getPort();
				foreach(layer, port->getLayers()) {
					foreach(lef::rect_t rect, layer->getRects()) {
						addBox(layer->getName(), rect.x+e.c, rect.y+e.f, rect.x+rect.w+e.c, rect.y+rect.h+e.f);
					}
				}
			}

			foreach (layer, macro->getObstruction()->getLayers()) {
				foreach(lef::rect_t rect, layer->getRects()) {
					addBox(layer->getName(), rect.x+e.c, rect.y+e.f, rect.x+rect.w+e.c, rect.y+rect.h+e.f);
				}
			}
		}*/

		// fill in GDS data:
		QString layer_name;
		QPolygonF polygon;
		QRectF rect;
		GDSCell *cell;

		/*if(project) if(project->isDefinedGDSMacro(macro_name)) {
			cell = project->getGDSMacro(macro_name);
			if(cell) {
				cell->setRectangle(x,y,w,h);
				foreach(GDSBoundary *b, cell->getBoundaries()) {
					layer_name = project->layerNameFromCIF(b->getLayerIndex());
					if(layer_name==QString()) {
						layer_name = project->layerNameFromDStyle(b->getLayerIndex());
					}
					if(layer_name==QString()) {
						qDebug() << "Couldn't map layer " << b->getLayerIndex();
					} else {
						polygon = b->getPolygon();
						rect = polygon.boundingRect();
						addBox(layer_name, rect.x()+e.c, rect.y()+e.f, rect.x()+rect.width()+e.c, rect.y()+rect.height()+e.f);
					}
				}
			}
		}*/
	}
}

void GLWidget::addWires()
{
	magic::layer_rects_t layers = magicdata->getRectangles();
	magic::rects_t layer;
	foreach(QString layerN, layers.keys()) {
		layer = layers[layerN];
		foreach (magic::rect_t e, layer) {
			addBox(layerN, e.x1, e.y1, e.x2, e.y2);
		}
	}
}

void GLWidget::initializeGL()
{
	qglClearColor(QColor(Qt::white));

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_FRONT_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//glShadeModel(GL_SMOOTH);
	//glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);
}


void GLWidget::resizeGL(int w, int h) {
	glViewport(0,0,w,h);
}

void GLWidget::addBox(QString layerN, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	//QColor color = project->colorMat(layerN);
	QColor color;
	qreal m_wireScaleX = m_highestX-m_lowestX;
	qreal m_wireScaleY = m_highestY-m_lowestY;

	//qreal z = project->posMat(layerN);
	//qreal th = project->thicknessMat(layerN);
	qreal z = 1;
	qreal th = 1;


	x1-=m_lowestX;
	y1-=m_lowestY;
	x2-=m_lowestX;
	y2-=m_lowestY;

	x1-=m_wireScaleX/2;
	y1-=m_wireScaleY/2;
	x2-=m_wireScaleX/2;
	y2-=m_wireScaleY/2;

	x1/=m_wireScaleX;
	y1/=m_wireScaleY;
	x2/=m_wireScaleX;
	y2/=m_wireScaleY;

	x1*=m_fScale;
	y1*=m_fScale;
	x2*=m_fScale;
	y2*=m_fScale;

	x1+=m_offsetX;
	x2+=m_offsetX;
	y1+=m_offsetY;
	y2+=m_offsetY;

	glBegin(GL_QUAD_STRIP);
	glColor4f(color.redF(), color.greenF(), color.blueF(), 0.75);

	// bottom
	glVertex3f( x1, y1, z );
	glVertex3f( x1, y2, z );
	glVertex3f( x2, y1, z );
	glVertex3f( x2, y2, z );

	// top
	glVertex3f( x2, y1, z+th );
	glVertex3f( x2, y2, z+th );
	glVertex3f( x1, y1, z+th );
	glVertex3f( x1, y2, z+th );

	// closing
	glVertex3f( x1, y1, z );
	glVertex3f( x1, y2, z );

	glEnd();
	glFlush();

	glBegin(GL_QUAD_STRIP);
	glColor4f(color.redF(), color.greenF(), color.blueF(), 0.75);

	// bottom
	glVertex3f( x1, y1, z );
	glVertex3f( x2, y1, z );
	glVertex3f( x1, y1, z+th );
	glVertex3f( x2, y1, z+th );

	glVertex3f( x1, y2, z+th );
	glVertex3f( x2, y2, z+th );
	glVertex3f( x1, y2, z );
	glVertex3f( x2, y2, z );

	glEnd();
	glFlush();

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		m_angle2+=dx;
		m_angle1+=dy;
		update();
	}

   lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	int numSteps = event->delta()/120;
	if( event->modifiers() & Qt::ShiftModifier ) {
		m_offsetX+=numSteps;
	} else if( event->modifiers() & Qt::ControlModifier ) {
		m_offsetY+=numSteps;
	} else {
		if((m_fScale-numSteps)>0) {
			m_fScale-=numSteps;
			event->accept();
		}
	}

	update();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
	switch(event->key()) {
		case Qt::Key_Left:
			if(event->modifiers() & Qt::ShiftModifier) {
				m_offsetX--;
			} else {
				m_angle2--;
			}
			break;
		case Qt::Key_Right:
			if(event->modifiers() & Qt::ShiftModifier) {
				m_offsetX++;
			} else {
				m_angle2++;
			}
			break;
		case Qt::Key_Down:
			if(event->modifiers() & Qt::ShiftModifier) {
				m_offsetY--;
			} else {
				m_angle1--;
			}
			break;
		case Qt::Key_Up:
			if(event->modifiers() & Qt::ShiftModifier) {
				m_offsetY++;
			} else {
				m_angle1++;
			}
			break;
		case Qt::Key_Plus:
			m_fScale++;
			break;
		case Qt::Key_Minus:
			m_fScale--;
			break;
	}
	update();

}

void GLWidget::axis3d_front()
{
	if(lastOrient!=ORIENT_FRONT) {
		m_angle1=0;
		m_angle2=0;
		update();
	}
	lastOrient=ORIENT_FRONT;
}

void GLWidget::axis3d_back()
{
	if(lastOrient!=ORIENT_BACK) {
		m_angle1=0;
		m_angle2=180;
		update();
	}
	lastOrient=ORIENT_BACK;
}

void GLWidget::axis3d_top()
{
	if(lastOrient!=ORIENT_TOP) {
		m_angle1=90;
		m_angle2=0;
		update();
	}
	lastOrient=ORIENT_TOP;
}

void GLWidget::axis3d_bottom()
{
	if(lastOrient!=ORIENT_BOTTOM) {
		m_angle1=-90;
		m_angle2=0;
		update();
	}
	lastOrient=ORIENT_BOTTOM;
}

void GLWidget::axis3d_left()
{
	if(lastOrient!=ORIENT_LEFT) {
		m_angle1=0;
		m_angle2=90;
		update();
	}
	lastOrient=ORIENT_LEFT;
}

void GLWidget::axis3d_right()
{
	if(lastOrient!=ORIENT_RIGHT) {
		m_angle1=0;
		m_angle2=-90;
		update();
	}
	lastOrient=ORIENT_RIGHT;
}

void GLWidget::axis3d_side()
{
	if(lastOrient!=ORIENT_SIDE) {
		m_angle1=45;
		m_angle2=45;
		update();
	}
	lastOrient=ORIENT_SIDE;
}
