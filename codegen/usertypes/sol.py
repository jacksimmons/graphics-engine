from tank import CLASSES_VEC


# Map of cpp to lua primitive types.
cpp_to_lua = {
    "int": "number",
    "float": "number",
    "double": "number",
    "std::string": "string",
    "bool": "boolean"
}

lua_class = "lc"


# Declare a sol usertype.
def declare_usertype(name: str, usertype_json: dict):
    constructor_data = __get_constructors(
        name,
        usertype_json["cpp_type"],
        usertype_json["constructors"]
    ) if "constructors" in usertype_json else {"cpp": "", "lua": ""}

    fields = __get_fields(
        name,
        usertype_json["cpp_type"],
        usertype_json["fields"]
    ) if "fields" in usertype_json else {"ut": "", "luals": ""}

    # Usertype definition
    usertype = f"""// Define Usertype
auto ut = lua.new_usertype<{usertype_json["cpp_type"]}>(
    "{name}"{f"," if "constructors" in usertype_json else ""}
    {constructor_data["cpp"]}
);
{fields["ut"]}
"""
    
    luals_codegen = f"""// LuaLS Codegen
LuaClass {lua_class};
{constructor_data["lua"]}
{fields["luals"]}
{CLASSES_VEC}.push_back({lua_class});"""
    
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


def __get_fields(name: str, cpp_type: str, fields: list[dict]):
    ut = ""
    luals = ""
    for field in fields:
        ut += f"ut[\"{field["lua"].split(":")[0]}\"] = &{cpp_type}::{field["cpp"]};\n"
        luals += f"{lua_class}.fields.push_back({"{"}\"{field["lua"].split(":")[0]}\", \"{field["lua"].split(": ")[1]}\"{"}"});\n"
    
    return {
        "ut": ut[:-1] if len(ut) > 0 else ut,
        "luals": luals[:-1] if len(luals) > 0 else ut
    }