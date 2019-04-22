#pragma once

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

#include <physics/interfaces/iPhysicsFactory.h>

EXTERN_DLL_EXPORT nPhysics::iPhysicsFactory* CreateFactory();