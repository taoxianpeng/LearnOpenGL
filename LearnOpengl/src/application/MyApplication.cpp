#include "MyApplication.h"

#include "RenderManager.h"
#include "keyboard.h"
#include "log.h"
#include "application.h"
#include "big_man_model.h"
#include "axio_model.h"
#include "window_model.h"
#include "triangle_model.h"
#include "geometry_ex.h"
#include "Instancing_ex.h"
#include "skybox.h"

void MyApplication::onProjectLoad()
{
	skyBoxNode = std::make_shared<SkyBox>();
	skyBoxNode->setName("SkyBox");
	skyBoxNode->setVisible(true);
	RenderManager::getInstance().addNode(skyBoxNode);

  geometryNode = std::make_shared<GeometryEx>();
	geometryNode->setName("GeometryModel");
	geometryNode->setVisible(false);
	RenderManager::getInstance().addNode(geometryNode);

	windowNode = std::make_shared<WindowModel>();
	windowNode->setName("Window");
	windowNode->setVisible(false);
	RenderManager::getInstance().addNode(windowNode);

	axioNode = std::make_shared<AxioModel>();
	axioNode->setName("Axio");
	axioNode->setVisible(false);
	RenderManager::getInstance().addNode(axioNode);

	bigmanNode = std::make_shared<BigManModel>();
	bigmanNode->setName("BigMan");
	bigmanNode->setVisible(true);
	RenderManager::getInstance().addNode(bigmanNode);

	triangleNode = std::make_shared<TriangleModel>();
	triangleNode->setName("Triangle");
	triangleNode->setVisible(false);
	RenderManager::getInstance().addNode(triangleNode);

	instanceExNode = std::make_shared<InstancingEx>();
	instanceExNode->setName("instanceEx");
	instanceExNode->setVisible(false);
	RenderManager::getInstance().addNode(instanceExNode);
}

void MyApplication::onKeyInputEvent(const KeyEvent& keyEvent, const KeyActionEvent& keyActionEvent)
{
    LOGD("key: {}, action: {}", static_cast<int>(keyEvent), static_cast<int>(keyActionEvent));

	if (keyEvent == KeyEvent::KEY_F && keyActionEvent == KeyActionEvent::PRESS) {
		if (bigmanNode->getVisible()) {
			bigmanNode->setVisible(false);
		} else {
			bigmanNode->setVisible(true);
		}
	} 
}


void MyApplication::onUpdate()
{

}

Application* createApplication() {
    return new MyApplication();
}