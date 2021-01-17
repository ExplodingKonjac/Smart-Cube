#include "selectorentity.h"

PlaneEntityBase::PlaneEntityBase(Qt3DCore::QNode *parent,const QString &fname):
		Qt3DCore::QEntity(parent),row(0),colum(0),surface(SurfX)
{
	mesh=new Qt3DRender::QMesh();
	material=new Qt3DExtras::QTextureMaterial();
	transform=new Qt3DCore::QTransform();
	mesh->setSource(QUrl("qrc:/cube/plane_model.obj"));
	Qt3DRender::QTexture2D* texture=new Qt3DRender::QTexture2D();
	Qt3DRender::QTextureImage* img=new Qt3DRender::QTextureImage();
	img->setSource(fname);
	texture->addTextureImage(img);
	material->setTexture(texture);
	material->setAlphaBlendingEnabled(true);
	addComponent(mesh);
	addComponent(material);
	addComponent(transform);
	updatePos();
}

void PlaneEntityBase::updatePos()
{
	constexpr float mul=2.3;
	QVector3D t;
	transform->setRotation(QQuaternion());
	switch(surface)
	{
	 case SurfX:
		t={mul,row*mul,-colum*mul},transform->setRotationZ(-90);
		break;
	 case SurfY:
		t={colum*mul,mul,-row*mul};
		break;
	 case SurfZ:
		t={colum*mul,row*mul,mul},transform->setRotationX(90);
		break;
	}
	transform->setTranslation(t);
}

void PlaneEntityBase::setSurface(Surfaces surf)
{
	surface=surf;
	updatePos();
}

void PlaneEntityBase::setPosition(int r, int c)
{
	row=r,colum=c;
	updatePos();
}

int PlaneEntityBase::getRowPos()
{
	switch(surface)
	{
	 case SurfX: case SurfZ:
		return row;
	 case SurfY:
		return -row;
	}
	return -1;
}

int PlaneEntityBase::getColumPos()
{
	switch(surface)
	{
	 case SurfX:
		return -colum;
	 case SurfY: case SurfZ:
		return colum;
	}
	return -1;
}

int PlaneEntityBase::getRow()	{ return row; }
int PlaneEntityBase::getColum()	{ return colum; }

PlaneEntityBase::Surfaces PlaneEntityBase::getSurface()
	{ return surface; }

SelectorEntity::SelectorEntity(QNode *parent,Surfaces surf,int r,int c):
		PlaneEntityBase(parent,"qrc:/cube/selector.png")
{
	row=r,colum=c;
	surface=surf;
	updatePos();
}

void SelectorEntity::turnSurface(int t)
{
	int s=surface+3;
	s=(s+t)%3;
	setSurface((Surfaces)s);
}

void SelectorEntity::move(int dtr,int dtc)
{
	int newr=row+dtr,newc=colum+dtc;
	if(qAbs(newr)<=1)	row=newr;
	if(qAbs(newc)<=1)	colum=newc;
	updatePos();
}

ClickDetector::ClickDetector(Qt3DCore::QNode *parent,Surfaces surf,int r,int c):
		PlaneEntityBase(parent,"qrc:/cube/empty.png")
{
	row=r,colum=c;
	surface=surf;
	picker=new Qt3DRender::QObjectPicker(this);
	connect(picker,&Qt3DRender::QObjectPicker::pressed,[this](){ emit clicked(); });
	connect(picker,&Qt3DRender::QObjectPicker::released,[this](){ emit released(); });
	picker->setDragEnabled(true);
	addComponent(picker);
	updatePos();
}

void turnRFromSelector(PlaneEntityBase* s,int d,Controller* ctrl)
{
	int dirr;
	void (Controller::*turnRFunc)(int,int);
	switch(s->getSurface())
	{
	 case SelectorEntity::SurfX:
		turnRFunc=&Controller::sendTurnY,dirr=-1;
		break;
	 case SelectorEntity::SurfY:
		turnRFunc=&Controller::sendTurnZ,dirr=1;
		break;
	 case SelectorEntity::SurfZ:
		turnRFunc=&Controller::sendTurnY,dirr=-1;
		break;
	}
	(ctrl->*turnRFunc)(s->getRowPos(),d*dirr);
}

void turnCFromSelector(PlaneEntityBase* s,int d,Controller* ctrl)
{
	int dirc;
	void (Controller::*turnCFunc)(int,int);
	switch(s->getSurface())
	{
	 case SelectorEntity::SurfX:
		turnCFunc=&Controller::sendTurnZ,dirc=-1;
		break;
	 case SelectorEntity::SurfY:
		turnCFunc=&Controller::sendTurnX,dirc=1;
		break;
	 case SelectorEntity::SurfZ:
		turnCFunc=&Controller::sendTurnX,dirc=1;
		break;
	}
	(ctrl->*turnCFunc)(s->getColumPos(),d*dirc);
}
