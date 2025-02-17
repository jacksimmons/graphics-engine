local transform = require "tank.transform"
local vec3      = require "tank.vec3"
local camera    = require "tank.camera"
-- Any underscore-prefixed parameters required in
-- modules are globals and can be passed.

-- Start is called when assigned node is instantiated 
function Startup()
    camera.translate(vec3.new(-2281, 79, 50))
    camera.rotate(vec3.new(0, 0, 90))
end

-- Update is called once per frame
function Update()
    -- local cam_transform = camera.get_transform(_camera)
    -- local cam_trans = transform.get_translation(cam_transform)
    -- cam_trans.x = cam_trans.x + 0.01
    -- transform.set_translation(cam_transform, cam_trans)
end

-- Finish is called when assigned node is destroyed
-- function Shutdown()
-- end
