#ifndef _GAMECONTROL_H_
#define _GAMECONTROL_H_

#include <QtCore/QObject>

#include <bitset>
#include <thread>
#include <queue>
#include <windows.h>

class Controller: public QObject
{
	Q_OBJECT
 private:
	struct Rotation
	{
		static const int XRotation=0;
		static const int YRotation=1;
		static const int ZRotation=2;
		int type;
		int line,dir;
	};
	std::queue<Rotation> rotq;
	std::thread *workthread;
	void work();

 signals:
	void xTurned(int line,int dir);
	void yTurned(int line,int dir);
	void zTurned(int line,int dir);
	void initFinished();

 public:
	std::bitset<3> xmov,ymov,zmov;
	Controller();
	void sendTurnX(int l,int d);
	void sendTurnY(int l,int d);
	void sendTurnZ(int l,int d);
	void turnLayersX(const QList<int> &ls,int d);
	void turnLayersY(const QList<int> &ls,int d);
	void turnLayersZ(const QList<int> &ls,int d);
	~Controller();
};

#endif
