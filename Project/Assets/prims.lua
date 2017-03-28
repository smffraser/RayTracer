other = gr.material({1.0, 1.0, 0.0}, {0, 0, 0}, 0, 0)

require('Assets/b_blocks')

scene = gr.node('root')
scene:add_child(blocks)


floor = gr.plane('floor')
scene:add_child(floor)
floor:set_material(other)
floor:set_texture('Assets/toy_box1.png')
floor:rotate('x',-90)
floor:scale(15, 15, 15)
floor:translate(0, 0, 0)


wall = gr.plane('wall')
scene:add_child(wall)
wall:set_material(other)
wall:scale(15, 15, 15)
wall:translate(0, 0, -5)




-- Render

white_light = gr.light({-10, 10, 15}, {0.9, 0.9, 0.9}, {1, 0, 0}, 0, 0)

--blocks:rotate('y', -20)
--blocks:rotate('z', 90)
scene:rotate('x', 20)

gr.render(scene, string.format("primitives.png"), 500, 500, {1.6, 1.4, 10}, {0, 0, -20}, {0, 1, 0}, 50, {0.3, 0.3, 0.3}, {white_light}, 8, 5, 5, 0, 0, 3, 0.1, 0, 1)