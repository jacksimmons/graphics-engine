local v3 = {}
v3.__index = v3
function v3.new(x, y, z)
    return setmetatable({x=x or 0, y=y or 0, z=z or 0}, v3)
end
function v3.one()
    return v3.new(1, 1, 1)
end
function v3:clone()
    return v3.new(self.x, self.y, self.z)
end
return v3