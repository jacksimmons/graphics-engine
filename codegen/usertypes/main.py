import os
import sys
import json
from cpp import function
from sol import declare_usertype

if __name__ != "__main__":
    raise ImportError("Don't use this as a module!")


# Globals
out_path = "TankEngine/src/scripting/generate_user_types.cpp"
usertypes_path = "TankEngine/data/usertypes"
ext = ".json"


# Read all usertypes in usertypes_path folder
usertype_filenames = os.listdir(usertypes_path)
usertype_paths = [os.path.join(usertypes_path, f) for f in usertype_filenames]
usertypes = {}
for i in range(len(usertype_filenames)):
    usertype_name = usertype_filenames[i].split(ext)[0]
    usertype_path = usertype_paths[i]
    with open(usertype_path, "r") as usertype_file:
        usertypes[usertype_name] = json.load(usertype_file)


# Codegen
usertype_funcs = ""
usertype_func_calls = ""
for usertype_name in usertypes.keys():
    func_name = f"generate{usertype_name}"
    usertype_funcs += function(
        f"static void {func_name}(sol::state &lua)",
        declare_usertype(usertype_name, usertypes[usertype_name])
    ) + "\n\n"
    usertype_func_calls += f"{func_name}(lua);\n"
usertype_func_calls = usertype_func_calls[:-1]

code = f"""// CODEGEN FROM {sys.argv[0]}
#include \"generate_user_types.hpp\"
#include \"user_types.hpp\""
"""
for key in usertypes.keys():
    code += f"{usertypes[key]["include"] if "include" in usertypes[key] else ""}"
code += f"""
namespace Tank
{"{"}
{usertype_funcs}
{function(
    "void generateAll(sol::state &lua)",
    usertype_func_calls
)}
{"}"}"""

# Output codegen
with open(out_path, "w") as file:
    file.write("")
    file.write(code)
    