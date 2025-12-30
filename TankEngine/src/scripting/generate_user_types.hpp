#define KC_PAIR(x) #x, GLFW_KEY_##x


// Get a lua class from name
#define GET_SOL_CLASS(name) Tank::UserTypes::classFromName(name).value()
// Declares a class base for name.
#define SOL_CLASS_BASE(name, baseName) (GET_SOL_CLASS(name)->base = #baseName/*, sol::bases<baseName>()*/)

// Declares a class field.
#define SOL_GLOBAL_FIELD(className, name, type) \
	GET_SOL_CLASS(className)->globalFields.push_back({name, type})

#define SOL_CALLABLE(relevantClass, functionsVector, funcName, retType, ...) \
	GET_SOL_CLASS(relevantClass)->functionsVector.push_back({ \
		funcName, \
		##__VA_ARGS__, \
		retType \
	})
#define SOL_METHOD(className, name, retType, ...) \
	(SOL_CALLABLE( \
		className, \
		methods, \
		name, \
		retType, \
		__VA_ARGS__ \
	), name)
#define SOL_META_METHOD(className, methodVal, retType, ...) \
	(SOL_CALLABLE( \
		className, \
		metaMethods, \
		sol::meta_function_names()[(int)methodVal], \
		retType, \
		__VA_ARGS__ \
	), methodVal)
#define SOL_STATIC_METHOD(className, name, retType, ...) \
	(SOL_CALLABLE( \
		className, \
		staticMethods, \
		name, \
		retType, \
		__VA_ARGS__ \
	), name)
#define SOL_GLOBAL_FUNCTION(relevantClass, name, retType, ...) \
	(SOL_CALLABLE( \
		relevantClass, \
		globalFunctions, \
		name, \
		retType, \
		__VA_ARGS__ \
	), name)
#define SOL_CONSTRUCTOR(className, ...) \
	SOL_STATIC_METHOD(className, "new", className, __VA_ARGS__)


namespace sol { class state; }
namespace Tank::UserTypes
{
    void generateAll(sol::state &lua);
}