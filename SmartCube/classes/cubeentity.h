#ifndef _CUBEENTITY_H_
#define _CUBEENTITY_H_

#include <QtCore/QParallelAnimationGroup>
#include <QtGui/QVector3D>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QTextureImage>
#include <Qt3DExtras/QTextureMaterial>

#include <random>
#include <ctime>

#include "cubeposcontroller.h"

const int rotX_sfc[6]={180,0,90,0,-90,0};
const int rotZ_sfc[6]={0,0,0,-90,0,90};

struct CubePos
{
	int x,y,z;
	CubePos()=default;
	CubePos(int _x,int _y,int _z):
			x(_x),y(_y),z(_z)
		{}
	CubePos operator =(const QVector3D &a)
	{
		x=a.x(),y=a.y(),z=a.z();
		return *this;
	}
	CubePos abs()
		{ return CubePos(qAbs(x),qAbs(y),qAbs(z)); }
	QVector3D toQV3D()const
		{ return QVector3D(x*2.3,y*2.3,z*2.3); }
};

class CubeEntity: public Qt3DCore::QEntity
{
	Q_OBJECT
 private:
	QString name;
	Qt3DRender::QMesh *mesh;
	Qt3DExtras::QTextureMaterial *material;
	Qt3DCore::QTransform *transform;
	CubePos org,now,x_axis,y_axis,z_axis;
	CubePosController *pos;

 protected:
	void initMeterial();
	void initMesh();
	void initTransform();
	void correctPos();

 public:
	explicit CubeEntity(QNode *parent,const QString &_n,const QVector3D &pos);
	int x();
	int y();
	int z();
	void setPosition(int x,int y,int z);
	void turnToOrgPos();
	void turnAroundX(int l,int dir);
	void turnAroundY(int l,int dir);
	void turnAroundZ(int l,int dir);
	void emitXFinished(int l);
	void emitYFinished(int l);
	void emitZFinished(int l);

 signals:
	void xFinished(int l);
	void yFinished(int l);
	void zFinished(int l);
};

bool allInited();

#endif
