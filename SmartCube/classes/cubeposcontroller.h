#ifndef _CUBEPOSCONTROLLER_H_
#define _CUBEPOSCONTROLLER_H_

#include <QObject>
#include <Qt3DCore/QTransform>
#include <QtCore/QPropertyAnimation>

class CubePosController : public QObject
{
	Q_OBJECT
	Q_PROPERTY(double angle READ angle WRITE setAngle NOTIFY angleChanged)

 private:
	Qt3DCore::QTransform *target;
	QMatrix4x4 matrix;
	QVector3D rot_axis,rot_trans;
	double rota;

 protected:
	void updateMatrix();

 public:
	int working_line;
	CubePosController(Qt3DCore::QTransform *tg,QObject *p=nullptr);
	double angle();
	void setAngle(double a);
	void setRotationTrans(const QVector3D& trans);
	void rotate(double a,const QVector3D& axis);
	void setTarget(Qt3DCore::QTransform* tg);
	void resetMatrix();

 signals:
	void angleChanged();
	void workFinished(int l);
};

#endif
