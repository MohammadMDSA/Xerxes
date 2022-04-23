#pragma once

#ifdef XDEBUG
#define XAssert(cond) assert(cond); __assume(cond)
#endif

///////////////////////////////////////
//---------------Common--------------//
///////////////////////////////////////


#define XNameOf(T) #T
#define XNameOfC(T) #T.c_str()

#ifndef xmax
#define xmax(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef xmin
#define xmin(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef XNRAND
#define XNRAND (float)rand() / RAND_MAX
#endif

#ifndef XRAND
#define XRAND ((float)rand() / RAND_MAX - 0.5f)
#endif

///////////////////////////////////////
//----------Engine Common------------//
///////////////////////////////////////

#define XRootM() RootManager::GetInstance()

#define XCameraM() RootManager::GetInstance()->GetCameraManager()

#define XResourceM() RootManager::GetInstance()->GetResourceManager()

#define XResourceMTexture() RootManager::GetInstance()->GetResourceManager()->ResourceGroup<TextureResource>

#define XResourceMEffect() RootManager::GetInstance()->GetResourceManager()->ResourceGroup<EffectResource>

#define XResourceMModel() RootManager::GetInstance()->GetResourceManager()->ResourceGroup<ModelResource>

#define XResourceMGeoPri() RootManager::GetInstance()->GetResourceManager()->ResourceGroup<GeometricPrimitiveResource>

#define XSceneM() RootManager::GetInstance()->GetSceneManager()

#define XSelectionM() RootManager::GetInstance()->GetSelectionManager()

#define XRenderer() RootManager::GetInstance()->GetRenderer()

///////////////////////////////////////
//-----------Components--------------//
///////////////////////////////////////

#define EmptyCompByNameBody

#define CompByNameBody() EmptyCompByNameBody

#define XCOMP_META_REGISTER(COMP) \
{ \
	entt::meta<COMP>().type(); \
}

#define XCOMP(T) \
{
#define EmptyCompByNameBody EmptyCompByNameBody##\
if(strcmp(NameOf(T), compName)) \
{ \
comp = new T();\
} \
}

#define CompByName() \
	GameObjectComponent* comp = nullptr;##CompByNameBody()

#define XCOMP_GENERATE_BODY() \
private: \
static bool initialized;

#define XCOMP_GENERATE_DEFINITION(T) \
bool T::initialized = false;

#define XCOMP_GENERATE_CONSTRUCTOR(T) \
{ \
	if(!initialized) \
	{ \
		initialized = true; \
		XCOMP_META_REGISTER(T) \
	} \
}
