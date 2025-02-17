local Camera = {}
function Camera.translate(translate_vec3)
    _camera_translate(translate_vec3.x, translate_vec3.y, translate_vec3.z)
end
function Camera.rotate(rotate_vec3)
    _camera_rotate(rotate_vec3.x, rotate_vec3.y, rotate_vec3.z)
end
return Camera