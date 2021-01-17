#ifndef _SELECTORENTITY_H_
#define _SELECTORENTITY_H_

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>
#include <Qt3DExtras/QTextureMaterial>

#include "gamecontrol.h"

class PlaneEntityBase: public Qt3DCore::QEntity
{
 public:
   enum Surfaces
   {
	   SurfX,SurfY,SurfZ
   };

 protected:
	Qt3DRender::QMesh *mesh;
	Qt3DExtras::QTextureMaterial *material;
	Qt3DCore::QTransform *transform;
	int row,colum;
	Surfaces surface;
	void updatePos();

 public:
	PlaneEntityBase(Qt3DCore::QNode *parent,const QString &fname);
	void setSurface(Surfaces surf);
	void setPosition(int r,int c);
	int getRowPos();
	int getColumPos();
	int getRow();
	int getColum();
	Surfaces getSurface();
};

class SelectorEntity : public PlaneEntityBase
{
 public:
	SelectorEntity(QNode *parent,Surfaces surf,int r,int c);
	void initPos();
	void turnSurface(int t);
	void move(int dtr,int dtc);
};

class ClickDetector: public PlaneEntityBase
{
	Q_OBJECT
 private:
	Qt3DRender::QObjectPicker *picker;

 public:
	ClickDetector(QNode *parent,Surfaces surf,int r,int c);

 signals:
	void clicked();
	void released();
};

void turnRFromSelector(PlaneEntityBase *s,int d,Controller *ctrl);
void turnCFromSelector(PlaneEntityBase *s,int d,Controller *ctrl);

#endif
