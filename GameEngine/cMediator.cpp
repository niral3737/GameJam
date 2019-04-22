#include "cMediator.h"

cMediator::cMediator()
{}


cMediator::~cMediator()
{}

void cMediator::LoadEntity(iEntity* entity)
{
	this->vecEntities.push_back(entity);
}

iEntity * cMediator::findEntityByName(std::string name)
{
	size_t numberOfEntities = vecEntities.size();
	for (size_t i = 0; i < numberOfEntities; i++)
	{
		if (vecEntities[i]->getName() == name)
		{
			return vecEntities[i];
		}
	}
	return NULL;
}

//void cMediator::sendMessageToEntity(std::vector<std::string> vecMessage)
//{
//	size_t numMessages = vecMessage.size();
//	if (vecMessage[0] == "hit")
//	{
//		iEquipment* equipment = this->findEquipmentByName(vecMessage[1]);
//
//		cBall* cueBall = (cBall*)equipment;
//		cMeshObject* ballMesh = (cMeshObject*) cueBall->getMesh();
//		ballMesh->position.x += 1.0f;
//	}
//}

void cMediator::sendMessageToEntity(nlohmann::json message)
{
	iEntity* entity = this->findEntityByName(message["to"].get<std::string>());

	std::string action = message["action"].get<std::string>();

	/*if (action == "hit")
	{
		cBall* ball = (cBall*) equipment;
		ball->hit();
	}*/
}


