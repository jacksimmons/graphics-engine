function Update()
    local displace = Vec3:new()
    local rotate = Vec3:new()

    if (node.key_input:get_key_state(KeyCode.W) == KeyState.Held) then displace.z = 1 end
    if (node.key_input:get_key_state(KeyCode.S) == KeyState.Held) then displace.z = -1 end
    -- Model is rotated 180 on z-axis
    if (node.key_input:get_key_state(KeyCode.A) == KeyState.Held) then displace.x = 1 end
    if (node.key_input:get_key_state(KeyCode.D) == KeyState.Held) then displace.x = -1 end

    if (node.key_input.get_key_state(KeyCode.J) == KeyState.Held) then rotate.z = -1 end
    if (node.key_input.get_key_state(KeyCode.L) == KeyState.Held) then rotate.z = 1 end

    print("HI")
    local pos = node.transform.translation
    local rot = node.transform.rotation
    pos = pos + displace
    rot = rot + rotate

    local cam = Scene:current():active_camera()
    print("HI")
    print(cam.position)
    cam.position = pos - Vec3:new(10, -10, 30)

    node.transform.translation = pos
end