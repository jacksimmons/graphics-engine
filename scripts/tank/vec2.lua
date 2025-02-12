local v2 = {}
v2.__index = v2
function v2.new(x, y)
    return setmetatable({x=x or 0, y=y or 0}, v2)
end
return v2