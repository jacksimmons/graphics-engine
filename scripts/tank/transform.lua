local Transform = {}
function Transform.set_translation(_transform, translation_vec3)
    _transform.translation = translation_vec3
end

function Transform.get_translation(_transform)
    return _transform.translation
end


function Transform.set_rotation(_transform, rotation_vec3)
    _transform.rotation = rotation_vec3
end

function Transform.get_rotation(_transform)
    return _transform.rotation
end


function Transform.set_scale(_transform, scale_vec3)
    _transform.scale = scale_vec3
end

function Transform.get_scale(_transform)
    return _transform.scale
end
return Transform