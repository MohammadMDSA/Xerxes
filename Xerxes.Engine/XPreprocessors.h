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
//----------Engine Resource----------//
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
//-----------Registration------------//
///////////////////////////////////////

#define XRegisterClass(name) \
rttr::registration::class_<name>(XNameOf(name))

#define XRegisterClassCns(name) \
XRegisterClass(name) \
.constructor<>()

#define XRegisterProperty(propName, cls) \
.property(XNameOf(propName), &cls::get_##propName, &cls::set_##propName)

#define XRegisterProperty_Val(propName, cls) \
.property(XNameOf(propName), &cls::propName)

#define XRegisterProperty_RVal(propName, cls) \
.property_readonly(XNameOf(propName), &cls::propName)

#define XRegisterMethod(methName, cls) \
.method(XNameOf(methName), &cls::methName)

///////////////////////////////////////
//-----------Class Utils-------------//
///////////////////////////////////////

#define XClassTop(name) \
public: \
RTTR_ENABLE(name)

#define XClass(name, parent) \
public: \
RTTR_ENABLE(name, parent) \
private: \
RTTR_REGISTRATION_FRIEND


///////////////////////////////////////
//-----------Components--------------//
///////////////////////////////////////

#define XProperty(type, name) \
public: \
inline type get_##name() const { return this->name; } \
inline void set_##name(type value) { this->name = value; } \
private: \
type name;

#define XProperty_G(type, name) \
public: \
inline type get_##name() const { return this->name; } \
private: \
inline void set_##name(type value) { this->name = value; } \
type name;


#define XProperty_R(type, name) \
public: \
inline type get_##name() const { return this->name; } \
private: \
const type name;

///////////////////////////////////////
//-----------Components--------------//
///////////////////////////////////////

#define EmptyCompByNameBody

#define CompByNameBody() EmptyCompByNameBody

#define XCOMP_META_REGISTER(COMP) \
{ \
	entt::meta<COMP>().type(); \
}

#define CompByName() \
	GameObjectComponent* comp = nullptr;##CompByNameBody()

#define XCOMP(cname) \
public: \
XClass(cname, GameObjectComponent) \
inline virtual std::string GetName() override { return XNameOf(cname); } \
private: \
static bool initialized;

#define XCOMP_GENERATE_DEFINITION(T) \
bool T::initialized = false;\
BOOST_CLASS_EXPORT_GUID(T, XNameOf(T))

#define XCOMP_GENERATE_CONSTRUCTOR(T) \
{ \
	if(!initialized) \
	{ \
		initialized = true; \
		XCOMP_META_REGISTER(T) \
	} \
}
