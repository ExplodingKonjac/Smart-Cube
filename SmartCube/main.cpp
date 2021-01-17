#include <QtCore/QVector>
#include <QtCore/QtMath>
#include <QtGui/QFont>
#include <QtGui/QtEvents>
#include <QtWidgets/QMessageBox>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DRender/QCamera>

#include <random>

#include "classes/mainwindow.h"
#include "classes/gamecontrol.h"
#include "classes/cubeentity.h"
#include "classes/selectorentity.h"

Controller *ctrler;
std::mt19937 mt_random(time(0));
QPoint lastpos,mousepos;
bool mousedown;
ClickDetector *selected;

inline int random(int l,int r)
	{ return mt_random()%(r-l+1)+l; }

class MainWindow: public Ui_MainWindow,public QMainWindow
{
 private:
	Qt3DExtras::Qt3DWindow *view;
	Qt3DRender::QCamera *camera;
	Qt3DCore::QEntity *scene_root;
	QVector<CubeEntity*> cubes;
	SelectorEntity *selector;
	QVector<ClickDetector*> detectors;
	void suitableFont(QWidget *w)
	{
		QFont f=w->font();
		f.setPixelSize(w->height()*0.5);
		w->setFont(f);
	}

 protected:
	void init3DObjects()
	{
		scene_root=new Qt3DCore::QEntity();
		camera=view->camera();
		camera->setPosition({20,15,25});
		camera->setUpVector({0,1,0});
		camera->setViewCenter({0,0,0});
		cubes.clear();
		cubes.append({new CubeEntity(scene_root,"Y_center",{0,1,0}),
					  new CubeEntity(scene_root,"YO_edge",{0,1,1}),
					  new CubeEntity(scene_root,"YB_edge",{1,1,0}),
					  new CubeEntity(scene_root,"YR_edge",{0,1,-1}),
					  new CubeEntity(scene_root,"YG_edge",{-1,1,0}),
					  new CubeEntity(scene_root,"YOB_corner",{1,1,1}),
					  new CubeEntity(scene_root,"YBR_corner",{1,1,-1}),
					  new CubeEntity(scene_root,"YRG_corner",{-1,1,-1}),
					  new CubeEntity(scene_root,"YGO_corner",{-1,1,1}),
					  new CubeEntity(scene_root,"O_center",{0,0,1}),
					  new CubeEntity(scene_root,"B_center",{1,0,0}),
					  new CubeEntity(scene_root,"R_center",{0,0,-1}),
					  new CubeEntity(scene_root,"G_center",{-1,0,0}),
					  new CubeEntity(scene_root,"OB_edge",{1,0,1}),
					  new CubeEntity(scene_root,"RB_edge",{1,0,-1}),
					  new CubeEntity(scene_root,"RG_edge",{-1,0,-1}),
					  new CubeEntity(scene_root,"GO_edge",{-1,0,1}),
					  new CubeEntity(scene_root,"W_center",{0,-1,0}),
					  new CubeEntity(scene_root,"WO_edge",{0,-1,1}),
					  new CubeEntity(scene_root,"WB_edge",{1,-1,0}),
					  new CubeEntity(scene_root,"WR_edge",{0,-1,-1}),
					  new CubeEntity(scene_root,"WG_edge",{-1,-1,0}),
					  new CubeEntity(scene_root,"WOB_corner",{1,-1,1}),
					  new CubeEntity(scene_root,"WBR_corner",{1,-1,-1}),
					  new CubeEntity(scene_root,"WRG_corner",{-1,-1,-1}),
					  new CubeEntity(scene_root,"WGO_corner",{-1,-1,1})});
		selector=new SelectorEntity(scene_root,SelectorEntity::SurfZ,0,0);
		detectors.clear();
		for(int i=0;i<=2;i++)
			for(int r=-1;r<=1;r++) for(int c=-1;c<=1;c++)
			{
				PlaneEntityBase::Surfaces surf=(PlaneEntityBase::Surfaces)i;
				ClickDetector *now=new ClickDetector(scene_root,surf,r,c);
				detectors.append(now);
				connect(now,&ClickDetector::clicked,[this,now]()
					{
						mousedown=true,lastpos=mousepos;
						selected=now;
						selector->setSurface(now->getSurface());
						selector->setPosition(now->getRow(),now->getColum());
					});
				connect(now,&ClickDetector::released,[](){ mousedown=false; });
			}
		for(auto &i: cubes)
		{
			connect(ctrler,&Controller::xTurned,i,&CubeEntity::turnAroundX);
			connect(ctrler,&Controller::yTurned,i,&CubeEntity::turnAroundY);
			connect(ctrler,&Controller::zTurned,i,&CubeEntity::turnAroundZ);
			connect(i,&CubeEntity::xFinished,[](int l){ ctrler->xmov[l+1]=0; });
			connect(i,&CubeEntity::yFinished,[](int l){ ctrler->ymov[l+1]=0; });
			connect(i,&CubeEntity::zFinished,[](int l){ ctrler->zmov[l+1]=0; });
		}
		view->setRootEntity(scene_root);
	}
	void shuffleCube()
	{
		int tms;
		tms=random(30,40);
		while(tms--)
		{
			int surf,l,d;
			void (Controller::*turnFunc)(int,int);
			surf=random(1,3);
			l=random(-1,1);
			d=random(0,1)?1:-1;
			if(surf==1)	turnFunc=&Controller::sendTurnX;
			if(surf==2)	turnFunc=&Controller::sendTurnY;
			if(surf==3)	turnFunc=&Controller::sendTurnZ;
			(ctrler->*turnFunc)(l,d);
		}
	}
	void resetCube()
	{
		for(auto &i: cubes)
			i->turnToOrgPos();
	}
	void reloadAll()
	{
		for(auto &i: cubes)	i->deleteLater();
		for(auto &i: detectors)	i->deleteLater();
		selector->deleteLater();
		scene_root->deleteLater();
		init3DObjects();
	}

 public:
	MainWindow(QWidget *parent=nullptr): QMainWindow(parent)
	{
		view=new Qt3DExtras::Qt3DWindow();
		_3d_widget=QWidget::createWindowContainer(view,this);
		setupUi(this);
		init3DObjects();
		view->defaultFrameGraph()->setClearColor(QColor(150,220,255));
		setWindowTitle("Smart Cube");
		setWindowIcon(QIcon("qrc:/icon.png"));
		connect(run_cmd,&QPushButton::clicked,this,&MainWindow::runCommand);
	}
	void switchSelector(int t)
		{ selector->turnSurface(t); }
	void moveSelector(int dr,int dc)
		{ selector->move(dr,dc); }
	SelectorEntity *getSelector()
		{ return selector; }
	const QVector<ClickDetector*> &getDetectors()
		{ return detectors; }
	void runCommand()
	{
		int idx=select_cmd->currentIndex();
		switch(idx)
		{
		 case 0:
			shuffleCube();
			break;
		 case 1:
			resetCube();
			break;
		 case 2:
			QMessageBox::critical(this,"Tips","This command couldn't be used yet!");
			break;
		 case 3:
			reloadAll();
			break;
		}
	}
	void resizeEvent(QResizeEvent *event)
	{
		suitableFont(run_cmd);
		suitableFont(select_cmd);
	}
};

class Application: public QApplication
{
 private:
	Controller *ctrl;
	MainWindow *mainw;
	clock_t key_timer;

 public:
	Application(int argc,char *argv[]):
			QApplication(argc,argv),ctrl(nullptr),mainw(nullptr),key_timer(0)
		{}
	void setController(Controller *c)	{ ctrl=c; }
	void setMainWindow(MainWindow *w)	{ mainw=w; }
	bool notify(QObject *obj, QEvent *e)
	{
		if(e->type()==QEvent::KeyPress)
		{
			clock_t now=clock();
			QKeyEvent* event=static_cast<QKeyEvent*>(e);
			if(now-key_timer>100)
			{
				key_timer=now;
				int dir=1,key=event->key();
				if(event->modifiers()&Qt::SHIFT)
					dir=-1;
				if(key==Qt::Key_X)
					ctrl->turnLayersX({-1,0,1},-1*dir);
				else if(key==Qt::Key_Y)
					ctrl->turnLayersY({-1,0,1},-1*dir);
				else if(key==Qt::Key_Down)
					ctrl->turnLayersX({-1,0,1},-1);
				else if(key==Qt::Key_Right)
					ctrl->turnLayersY({-1,0,1},-1);
				else if(key==Qt::Key_Up)
					ctrl->turnLayersX({-1,0,1},1);
				else if(key==Qt::Key_Left)
					ctrl->turnLayersY({-1,0,1},1);
				else if(key==Qt::Key_Z)
					ctrl->turnLayersZ({-1,0,1},-1*dir);
				else if(key==Qt::Key_J)
					mainw->moveSelector(0,-1);
				else if(key==Qt::Key_L)
					mainw->moveSelector(0,1);
				else if(key==Qt::Key_I)
					mainw->moveSelector(1,0);
				else if(key==Qt::Key_K)
					mainw->moveSelector(-1,0);
				else if(key==Qt::Key_Tab)
					mainw->switchSelector(1);
				else if(key==Qt::Key_Backtab)
					mainw->switchSelector(-1);
				else if(key==Qt::Key_W || key==Qt::Key_S || key==Qt::Key_A || key==Qt::Key_D)
				{
					SelectorEntity *s=mainw->getSelector();
					if(key==Qt::Key_W)
						turnCFromSelector(s,-1,ctrl);
					else if(key==Qt::Key_S)
						turnCFromSelector(s,1,ctrl);
					else if(key==Qt::Key_A)
						turnRFromSelector(s,1,ctrl);
					else if(key==Qt::Key_D)
						turnRFromSelector(s,-1,ctrl);
				}
			}
		}
		else if(e->type()==QEvent::MouseMove)
		{
			QMouseEvent *event=static_cast<QMouseEvent*>(e);
			mousepos=event->globalPos();
			if(mousedown)
			{
				double dis=qSqrt((mousepos.x()-lastpos.x())*(mousepos.x()-lastpos.x())+
								 (mousepos.y()-lastpos.y())*(mousepos.y()-lastpos.y()));
				if(dis>200)
				{
					int dx=mousepos.x()-lastpos.x();
					int dy=mousepos.y()-lastpos.y();
					if(qAbs(dx)>qAbs(dy))
						turnRFromSelector(selected,dx>0?-1:1,ctrler);
					else
						turnCFromSelector(selected,dy>0?1:-1,ctrler);
					mousedown=false;
				}
			}
		}
		return QApplication::notify(obj,e);
	}
};

int main(int argc, char *argv[])
{
	ctrler=new Controller();
	Application app(argc,argv);
	MainWindow window;
	app.setController(ctrler);
	app.setMainWindow(&window);
	window.show();
	return app.exec();
}
