from typing import Union
from tank import CLASSES_VEC
from metamethods import determine_method, get_meta_method_rval


# Map of cpp to lua primitive types.
cpp_to_lua = {
    "int": "number",
    "float": "number",
    "double": "number",
    "std::string": "string",
    "bool": "boolean"
}

lua_class = "lc"


def __pad_if_not_empty(string: str):
    if len(string) == 0: return string
    return f"{string}\n"


# Declare a sol usertype.
def declare_usertype(name: str, usertype_json: dict):
    constructor_data = __get_constructors(
        name,
        usertype_json["cpp_type"],
        usertype_json["constructors"]
    ) if "constructors" in usertype_json else {"cpp": "", "lua": ""}

    fields = __get_fields(
        usertype_json["cpp_type"],
        usertype_json["fields"]
    ) if "fields" in usertype_json else {"ut": "", "luals": ""}

    properties = __get_properties(
        usertype_json["cpp_type"],
        usertype_json["properties"]
    ) if "properties" in usertype_json else {"ut": "", "luals": ""}

    methods = __get_methods(
        usertype_json["cpp_type"],
        usertype_json["methods"]
    ) if "methods" in usertype_json else {"ut": "", "luals": ""}

    mms = __get_meta_methods(
        usertype_json["cpp_type"],
        usertype_json["metamethods"]
    ) if "metamethods" in usertype_json else {"ut": "", "luals": ""}

    # Usertype definition
    usertype = f"""// Define Usertype
auto ut = lua.new_usertype<{usertype_json["cpp_type"]}>(
    "{name}"{f",\n{constructor_data["cpp"]}" if "constructors" in usertype_json else ""}
);
""" +\
    __pad_if_not_empty(fields["ut"]) +\
    __pad_if_not_empty(properties["ut"]) +\
    __pad_if_not_empty(mms["ut"])
    
    luals_codegen = f"""// LuaLS Codegen
LuaClass {lua_class};
""" +\
    __pad_if_not_empty(constructor_data["lua"]) +\
    __pad_if_not_empty(fields["luals"]) +\
    __pad_if_not_empty(properties["luals"]) +\
    f"{CLASSES_VEC}.push_back({lua_class});"
    
    return usertype + "\n" + luals_codegen


# Converts cpp types (e.g. (float, float, float))
# to lua types (e.g. { "param1", "number" }, { "param2", "number" }, { "param3", "number" })
def __params_to_lua(cpp_params: list[str]):
    print(str(cpp_params))
    lua_params = "{"
    param_num = 1
    for param in cpp_params:
        if len(param) == 0:
            continue
        if param not in cpp_to_lua.keys():
            raise TypeError(f"No lua type for {param}.")
        
        lua_params += "{" + f"\"param{param_num}\", \"{cpp_to_lua[param]}\"" + "},"
        param_num += 1

    if len(lua_params) > 1:
        lua_params = lua_params[:-1]
    lua_params += "}"

    return lua_params


def __get_constructors(usertype_name: str, cpp_type: str, cpp_usertype_constructors: list[str]):
    # sol::constructors<CPP_TYPE(), CPP_TYPE(float)>
    cpp_constructors_type = ""
    # SOL_CONSTRUCTOR(USERTYPE_NAME, {})
    # SOL_CONSTRUCTOR(USERTYPE_NAME, { "x", "float" })
    # Concatenate all constructors with newlines
    lua_constructors = ""
    for cpp_constructor_type in cpp_usertype_constructors:
        # Get a list of all params in the constructor
        cpp_param_list = [x for x in cpp_constructor_type.replace("(", "").replace(")", "").replace(" ", "").split(",")]
        cpp_constructors_type += f"{cpp_type}{cpp_constructor_type},"
        lua_constructors += f"SOL_CONSTRUCTOR(\"{usertype_name}\",{__params_to_lua(cpp_param_list)});\n"
    if len(cpp_usertype_constructors) > 0:
        cpp_constructors_type = cpp_constructors_type[:-1]
        lua_constructors = lua_constructors[:-1]
    
    return {
        "cpp": f"sol::constructors<{cpp_constructors_type}>()",
        "lua": lua_constructors
    }


# Codegen to define LuaLS representation of a field.
def __luals_field(name: str, type: str):
    return f"{lua_class}.fields.push_back({"{"}\"{name}\", \"{type.replace(" ", "")}\"{"}"});\n"


def __return_ut_and_luals(ut: str, luals: str):
    return {
        "ut": ut[:-1] if len(ut) > 0 else ut,
        "luals": luals[:-1] if len(luals) > 0 else luals
    }


# Codegen to define all fields of a user type.
def __get_fields(cpp_type: str, fields: list[dict]):
    ut = ""
    luals = ""
    for field in fields:
        name, type = field["lua"].split(":")
        ut += f"ut[\"{name}\"] = &{cpp_type}::{field["cpp"]};\n"
        luals += __luals_field(name, type)
    
    return __return_ut_and_luals(ut, luals)


# Codegen to define all properties of a user type.
def __get_properties(cpp_type: str, props: list[dict]):
    ut = ""
    luals = ""
    for prop in props:
        name, type = prop["lua"].split(":")
        ut += f"ut[\"{name}\"] = sol::property(&{cpp_type}::{prop["get"]}{f", &{cpp_type}::{prop["set"]}" if "set" in prop else ""});\n"
        luals += __luals_field(name, type)
    
    return __return_ut_and_luals(ut, luals)


def __get_meta_methods(cpp_type: str, mms: list[Union[str, list[str]]]):
    ut = ""
    luals = ""
    for mm in mms:
        mm_name = mm
        mm_types = [cpp_type, cpp_type]
        if type(mm_name) != str:
            mm_name = mm[0]
            mm_types = [cpp_type]
            mm_types.extend(mm[1:])

        sol_mm_type = determine_method(mm_name)
        ut += f"ut[sol::meta_method::{sol_mm_type}] = {get_meta_method_rval(sol_mm_type, mm_types)};\n"

    return __return_ut_and_luals(ut, luals)


# Codegen to define all methods of a user type.
def __get_methods(cpp_type: str, methods: list[dict]):
    ut = ""
    luals = ""
    for method in methods:
        ut += f"ut[\"{method["lua"].split(":")[0]}\"] = &{cpp_type}::{method["cpp"]};\n"
        luals += f"{lua_class}.fields.push_back({"{"}\"{field["lua"].split(":")[0]}\", \"{field["lua"].split(": ")[1]}\"{"}"});\n"