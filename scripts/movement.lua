-- Properties this script can access and modify
-- transform

-- Start is called when assigned node is instantiated 
-- function start()
-- end

-- Update is called once per frame
function Update()
    print("HI")
    transform.translation.x = transform.translation.x + 0.001
end

-- Finish is called when assigned node is destroyed
-- function finish()
-- end
