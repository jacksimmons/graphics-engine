# Codegen for a C++ function, with pretty indentation.
def function(signature: str, contents: str):
    return \
f"""    {signature}
    {"{"}
        {contents.replace("\n", "\n\t\t")}
    {"}"}"""