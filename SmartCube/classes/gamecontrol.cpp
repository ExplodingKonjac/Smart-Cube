#include "gamecontrol.h"
#include <QtCore/QDebug>

Controller::Controller(): xmov(0),ymov(0),zmov(0)
{
	workthread=new std::thread(&Controller::work,this);
}

void Controller::work()
{
	while(true)
	{
		if(!rotq.empty())
		{
			Rotation rot=rotq.front();
			if(rot.type==Rotation::XRotation)
			{
				while(xmov[rot.line+1] || ymov.count() || zmov.count())
					Sleep(1);
				xmov[rot.line+1]=1;
				emit xTurned(rot.line,rot.dir);
			}
			else if(rot.type==Rotation::YRotation)
			{
				while(ymov[rot.line+1] || xmov.count() || zmov.count())
					Sleep(1);
				ymov[rot.line+1]=1;
				emit yTurned(rot.line,rot.dir);
			}
			else if(rot.type==Rotation::ZRotation)
			{
				while(zmov[rot.line+1] || xmov.count() || ymov.count())
					Sleep(1);
				zmov[rot.line+1]=1;
				emit zTurned(rot.line,rot.dir);
			}
			rotq.pop();
		}
		Sleep(1);
	}
}

void Controller::sendTurnX(int l, int d)	{ rotq.push({Rotation::XRotation,l,d}); }
void Controller::sendTurnY(int l, int d)	{ rotq.push({Rotation::YRotation,l,d}); }
void Controller::sendTurnZ(int l, int d)	{ rotq.push({Rotation::ZRotation,l,d}); }

void Controller::turnLayersX(const QList<int>& ls, int d)
	{ for(auto &i: ls)	sendTurnX(i,d); }

void Controller::turnLayersY(const QList<int>& ls, int d)
	{ for(auto &i: ls)	sendTurnY(i,d); }

void Controller::turnLayersZ(const QList<int>& ls, int d)
	{ for(auto &i: ls)	sendTurnZ(i,d); }

Controller::~Controller()
	{ pthread_cancel(workthread->native_handle()); }
