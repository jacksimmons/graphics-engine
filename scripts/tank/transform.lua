local vec3 = require("tank.vec3")

local Transform = transform
Transform.__index = Transform
Transform.translation = vec3.new()
Transform.rotation = vec3.new()
Transform.scale = vec3.one()

function Transform:set_translation(translation_vec3)
    self.translation = translation_vec3
end
function Transform:set_rotation(rotation_vec3)
    self.rotation = rotation_vec3
end
function Transform:set_scale(scale_vec3)
    self.scale = scale_vec3
end

return Transform