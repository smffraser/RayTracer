-- instance.lua
-- Thomas Pflaum 1996

stone = gr.material({0.0, 0.0, 0.0}, {0.25, 0.25, 0.25}, 100000, 1.52)
grass = gr.material({0.86, 0.82, 0.71}, {0.0, 0.0, 0.0}, 0, 0)
blue = gr.material({0.68, 0.91, 0.99}, {0.0, 0.0, 0.0}, 0, 0)

-- ##############################################
-- the scene
-- ##############################################
scene = gr.node('scene')

-- the floor
plane = gr.plane('floor')
scene:add_child(plane)
plane:set_material(grass)
plane:set_texture('Assets/wood_floor.png')
plane:rotate('x', -90)
plane:scale(20, 20, 10)

plane2 = gr.plane('wall')
scene:add_child(plane2)
plane2:set_material(blue)
plane2:set_texture('Assets/brick.png')
--plane2:rotate('y', 180)
plane2:scale(20, 15, 10)
plane2:translate(0, 7.5, -5)

-- sphere
sphere = gr.sphere('sphere')
scene:add_child(sphere)
sphere:set_material(stone)
sphere:scale(2.0, 2.0, 2.0)
sphere:translate(2, 2.0, 1.5)

-- cube
cube = gr.cylinder('cube')
scene:add_child(cube)
cube:set_material(stone)
cube:rotate('x', 90)
cube:scale(2, 2, 2)
cube:translate(-3, 1.1, -1)

--scene:rotate('y', 90)
--scene:rotate('x', )

white_light = gr.light({5, 20, 15}, {0.9, 0.9, 0.9}, {1, 0, 0}, 0, 0)

-- render it!
gr.render(scene,
'refract.png', 512, 512,
{0, 4.5, 12}, {0, 0, -31}, {0, 1, 0}, 50,
{0.4, 0.4, 0.4}, {white_light}, 1, 5, 5, 0, 0)