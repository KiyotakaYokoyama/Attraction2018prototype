#include "Application.h"
#include "Drawer.h"
#include "ModelMDL.h"
#include "ModelMV1.h"
#include "Device.h"
#include "Image.h"
#include "Gesture.h"
#include "FileSearchState.h"
#include <vector>
#include "DxLib.h"

class Test : public Task {
public:
	static std::string getTag( ) { return "TEST"; }
	Test( ) {
	}

	virtual ~Test( ) {
	}
	
	void initialize( ) {
		
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		GesturePtr gesture = Gesture::getTask( );
		
		for( int i = 0; i < Gesture::HUMAN_MAX; i++ ) {
			int n = 0;
			{
				Vector point = gesture->getJointPoint( i, Gesture::TYPE_HEAD );
				drawer->drawString( 0, i * 120 + n * 20, "HEAD：(%lf,%lf,%lf)", point.x, point.y, point.z );
				n++;
			}
			{
				Vector point = gesture->getJointPoint( i, Gesture::TYPE_RIGHTHAND );
				drawer->drawString( 0, i * 120 + n * 20, "RIGHTHAND：(%lf,%lf,%lf)", point.x, point.y, point.z );
				n++;
			}
			{
				Vector point = gesture->getJointPoint( i, Gesture::TYPE_LEFTHAND );
				drawer->drawString( 0, i * 120 + n * 20, "LEFTHAND：(%lf,%lf,%lf)", point.x, point.y, point.z );
				n++;
			}
			{
				Vector point = gesture->getJointPoint( i, Gesture::TYPE_RIGHTFOOT );
				drawer->drawString( 0, i * 120 + n * 20, "RIGHTFOOT：(%lf,%lf,%lf)", point.x, point.y, point.z );
				n++;
			}
			{
				Vector point = gesture->getJointPoint( i, Gesture::TYPE_LEFTFOOT );
				drawer->drawString( 0, i * 120 + n * 20, "LEFTFOOT：(%lf,%lf,%lf)", point.x, point.y, point.z );
				n++;
			}
		}

		drawer->waitForSync( );
		drawer->flip( );
	}
private:
	std::vector< std::string > _filename;
	int _select;
};

void main( ) {
	TaskPtr drawer = TaskPtr( new Drawer( "Movie" ) );
	TaskPtr test = TaskPtr( new Test( ) );
	TaskPtr gesture = GesturePtr( new Gesture );

	ApplicationPtr app = Application::getInstance( );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Test::getTag( ), test );
	app->addTask( Gesture::getTag( ), gesture );
}