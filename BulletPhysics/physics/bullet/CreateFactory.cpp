#include "CreateFactory.h"
#include "cBulletPhysicsFactory.h"

EXTERN_DLL_EXPORT nPhysics::iPhysicsFactory* CreateFactory()
{
	return new nPhysics::cBulletPhysicFactory();
}
