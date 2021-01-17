#include <QtCore/QtMath>

#include <cstdio>
#include <ctime>

#include "cubeentity.h"

static std::mt19937 random(time(0));
static const double pi=qAcos(-1);

CubeEntity::CubeEntity(QNode* parent,const QString& _n,const QVector3D& pos):
		Qt3DCore::QEntity(parent),name(_n),x_axis(1,0,0),y_axis(0,1,0),z_axis(0,0,1)
{
	org=now=pos;
	initMeterial();
	initMesh();
	initTransform();
	addComponent(material);
	addComponent(mesh);
	addComponent(transform);
}

int CubeEntity::x()	{ return now.x; }
int CubeEntity::y()	{ return now.y; }
int CubeEntity::z()	{ return now.z; }

void CubeEntity::setPosition(int x,int y,int z)
{
	now=CubePos(x,y,z);
	transform->setTranslation(now.toQV3D());
}

void CubeEntity::initMeterial()
{
	material=new Qt3DExtras::QTextureMaterial();
	Qt3DRender::QTexture2D* texture=new Qt3DRender::QTexture2D();
	Qt3DRender::QTextureImage* img=new Qt3DRender::QTextureImage();
	img->setSource("qrc:/cube/"+name+".png");
	texture->addTextureImage(img);
	material->setTexture(texture);
}

void CubeEntity::initMesh()
{
	mesh=new Qt3DRender::QMesh();
	mesh->setSource(QUrl("qrc:/cube/cube_model.obj"));
}

void CubeEntity::initTransform()
{
	transform=new Qt3DCore::QTransform();
	pos=new CubePosController(transform);
	pos->setRotationTrans(org.toQV3D());
	transform->setTranslation(org.toQV3D());
}

void CubeEntity::turnToOrgPos()
{
	QPropertyAnimation *a_pos=new QPropertyAnimation(transform,"translation");
	QPropertyAnimation *a_agl=new QPropertyAnimation(transform,"rotation");
	QParallelAnimationGroup *an=new QParallelAnimationGroup();
	a_pos->setEndValue(org.toQV3D());
	a_agl->setEndValue(QQuaternion());
	a_pos->setDuration(1000);
	a_agl->setDuration(1000);
	an->addAnimation(a_pos);
	an->addAnimation(a_agl);
	QObject::connect(an,&QParallelAnimationGroup::finished,[this]
		{
			setPosition(org.x,org.y,org.z);
			x_axis=CubePos(1,0,0),y_axis=CubePos(0,1,0),z_axis=CubePos(0,0,1);
			transform->setRotation(QQuaternion());
			pos->resetMatrix();
		});
	an->start();
}

static void rotatePoint(int &x,int &y,double angle)
{
	double xx=x,yy=y;
	xx=x*qCos(angle*pi/180)-y*qSin(angle*pi/180);
	yy=x*qSin(angle*pi/180)+y*qCos(angle*pi/180);
	x=qRound(xx),y=qRound(yy);
}
static void rotatePoint(CubePos &p,const CubePos &axis,double angle)
{
	if(axis.x)
		rotatePoint(p.y,p.z,(axis.x>0?1:-1)*angle);
	if(axis.y)
		rotatePoint(p.z,p.x,(axis.y>0?1:-1)*angle);
	if(axis.z)
		rotatePoint(p.x,p.y,(axis.z>0?1:-1)*angle);
}

void CubeEntity::emitXFinished(int l)
{
	emit xFinished(l);
	disconnect(pos,&CubePosController::workFinished,this,&CubeEntity::emitXFinished);
}
void CubeEntity::emitYFinished(int l)
{
	emit yFinished(l);
	disconnect(pos,&CubePosController::workFinished,this,&CubeEntity::emitYFinished);
}
void CubeEntity::emitZFinished(int l)
{
	emit zFinished(l);
	disconnect(pos,&CubePosController::workFinished,this,&CubeEntity::emitZFinished);
}
void CubeEntity::turnAroundX(int l,int dir)
{
	if(l==now.x)
	{
		connect(pos,&CubePosController::workFinished,this,&CubeEntity::emitXFinished);
		pos->working_line=l;
		pos->rotate(dir*90,x_axis.toQV3D());
		rotatePoint(y_axis,x_axis,-dir*90);
		rotatePoint(z_axis,x_axis,-dir*90);
		rotatePoint(now,{1,0,0},dir*90);
	}
}

void CubeEntity::turnAroundY(int l,int dir)
{
	if(l==now.y)
	{
		connect(pos,&CubePosController::workFinished,this,&CubeEntity::emitYFinished);
		pos->working_line=l;
		pos->rotate(dir*90,y_axis.toQV3D());
		rotatePoint(x_axis,y_axis,-dir*90);
		rotatePoint(z_axis,y_axis,-dir*90);
		rotatePoint(now,{0,1,0},dir*90);
	}
}

void CubeEntity::turnAroundZ(int l,int dir)
{
	if(l==now.z)
	{
		connect(pos,&CubePosController::workFinished,this,&CubeEntity::emitZFinished);
		pos->working_line=l;
		pos->rotate(dir*90,z_axis.toQV3D());
		rotatePoint(x_axis,z_axis,-dir*90);
		rotatePoint(y_axis,z_axis,-dir*90);
		rotatePoint(now,{0,0,1},dir*90);
	}
}
