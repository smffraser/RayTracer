-- test for hierarchical ray-tracers.
-- Thomas Pflaum 1996

gold = gr.material({0.9, 0.8, 0.4}, {0.0, 0.0, 0.0}, 25, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0, 0)
blue = gr.material({0.7, 0.6, 1}, {0.0, 0.0, 0.0}, 25, 0)

scene = gr.node('scene')
scene:rotate('X', 23)
scene:translate(6, -2, -15)

-- the floor
plane = gr.plane('plane')
scene:add_child(plane)
plane:set_material(grass)
plane:set_texture("Assets/small_checks.png")
plane:rotate('x', -90)
plane:rotate('y', -50)
plane:scale(100, 100, 100)
--plane:translate(0, 0, -50)

-- Cube
poly = gr.cube('poly')
scene:add_child(poly)
poly:scale(3, 3, 3)
poly:rotate('y', 30)
poly:translate(-11, 0, -4)
poly:set_material(blue)
poly:set_texture('Assets/wood_block_C.png')
poly:set_bump('Assets/wood_block_C_bump.png')

-- Jack
require('Assets/jack')
scene:add_child(jack)
jack:rotate('y', 45)
jack:rotate('z', 50)
jack:rotate('y', 45)
jack:scale(0.8, 0.8, 0.8)
jack:translate(-4, 0.75, 0)

-- The lights
l1 = gr.light({200,200,400}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0, 0)
l2 = gr.light({0, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0}, 0, 0)

gr.render(scene, 'AA.png', 500, 500,
{0, 0, 0,}, {0, 0, -800}, {0, 1, 0}, 50,
{0.4, 0.4, 0.4}, {l1, l2}, 16, 5, 5, 0, 0, 3, 0, 0, 1)