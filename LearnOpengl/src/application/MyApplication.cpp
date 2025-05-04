#include "MyApplication.h"

#include "RenderManager.h"
#include "log.h"
#include "application.h"
#include "big_man_model.h"
#include "axio_model.h"
#include "window_model.h"
#include "triangle_model.h"
#include "geometry_ex.h"
#include "Instancing_ex.h"

void MyApplication::onProjectLoad()
{
    auto geometryNode = std::make_shared<GeometryEx>();
	geometryNode->setName("GeometryModel");
	geometryNode->setVisible(false);
	RenderManager::getInstance().addNode(geometryNode);

	auto windowNode = std::make_shared<WindowModel>();
	windowNode->setName("Window");
	windowNode->setVisible(false);
	RenderManager::getInstance().addNode(windowNode);

	auto axioNode = std::make_shared<AxioModel>();
	axioNode->setName("Axio");
	axioNode->setVisible(false);
	RenderManager::getInstance().addNode(axioNode);

	auto bigmanNode = std::make_shared<BigManModel>();
	bigmanNode->setName("BigMan");
	bigmanNode->setVisible(true);
	RenderManager::getInstance().addNode(bigmanNode);

	auto triangleNode = std::make_shared<TriangleModel>();
	triangleNode->setName("Triangle");
	triangleNode->setVisible(false);
	RenderManager::getInstance().addNode(triangleNode);

	auto instanceExNode = std::make_shared<InstancingEx>();
	instanceExNode->setName("instanceEx");
	instanceExNode->setVisible(false);
	RenderManager::getInstance().addNode(instanceExNode);
}

void MyApplication::onKeyInputEvent(const KeyEvent& keyEvent, const KeyActionEvent& keyActionEvent)
{
    // LOGD("key: {}, action: {}", static_cast<int>(keyEvent), static_cast<int>(keyActionEvent));
}


void MyApplication::onUpdate()
{

}

Application* createApplication() {
    return new MyApplication();
}