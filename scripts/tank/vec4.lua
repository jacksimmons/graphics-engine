local v4 = {}
v4.__index = v4
function v4.new(x, y, z, w)
    return setmetatable({x = x or 0, y = y or 0, z = z or 0, w = w or 0}, v4)
end
return v4