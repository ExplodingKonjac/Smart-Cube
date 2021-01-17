#include "cubeposcontroller.h"

CubePosController::CubePosController(Qt3DCore::QTransform *tg,QObject *parent):
		QObject(parent),target(tg)
	{}

double CubePosController::angle()	{ return rota; }

void CubePosController::updateMatrix()
{
	QMatrix4x4 m=matrix;
	m.rotate(rota,rot_axis);
	m.translate(rot_trans.x(),
				rot_trans.y(),
				rot_trans.z());
	target->setMatrix(m);
}

void CubePosController::setAngle(double a)
{
	if(!qFuzzyCompare(rota,a))
	{
		emit angleChanged();
		updateMatrix();
	}
	rota=a;
}

void CubePosController::setTarget(Qt3DCore::QTransform* tg)
	{ target=tg; }

void CubePosController::setRotationTrans(const QVector3D &trans)
	{ rot_trans=trans; }

void CubePosController::rotate(double a,const QVector3D& axis)
{
	QPropertyAnimation* an=new QPropertyAnimation(this,"angle");
	rot_axis=axis;
	an->setStartValue(0);
	an->setEndValue(a);
	an->setDuration(150);
	QObject::connect(an,&QPropertyAnimation::finished,[this,a]
		{
			matrix.rotate(a,rot_axis);
			rota=0;
			updateMatrix();
			emit workFinished(working_line);
		});
	an->start();
}

void CubePosController::resetMatrix()
{
	matrix=QMatrix4x4();
}
