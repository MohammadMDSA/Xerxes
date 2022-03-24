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
