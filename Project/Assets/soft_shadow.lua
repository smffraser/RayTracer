other = gr.material({1.0, 1.0, 0.0}, {0, 0, 0}, 0, 0)

scene_root = gr.node('scene_root')


floor = gr.plane('floor')
scene_root:add_child(floor)
floor:set_material(other)
floor:set_texture('Assets/toy_box1.png')
floor:rotate('x',-90)
floor:scale(10, 10, 10)
floor:translate(0, -1.5, 0)


wall = gr.plane('wall')
scene_root:add_child(wall)
wall:set_material(other)
wall:scale(10, 10, 10)
wall:translate(0, 0, -3)

require('Assets/jack')


scene_root:add_child(jack)
silver = gr.material({0.50754, 0.50754, 0.50754}, {0.3, 0.3, 0.3}, 100000000, 0)
jack:scale(1.2, 1.2, 1.2)
jack:rotate('y', 45)
jack:rotate('z', 50)
jack:rotate('y', 45)
jack:translate(0, -0.4, 0)

scene_root:rotate('x', 20)

white_light = gr.light({-17, 17, 12}, {0.8, 0.8, 0.8}, {1, 0, 0}, 6, 6)
white_light_2 = gr.light({5, 5, 5}, {0.5, 0.5, 0.5}, {1, 0, 0}, 1.5, 1.5)

gr.render(scene_root, 'soft_shadow.png', 500, 500, {0, 0, 6}, {0, 0, -1}, {0, 1, 0}, 50, {0.3, 0.3, 0.3}, {white_light, white_light_2}, 16, 5, 5, 0, 8, 0, 0.01, 0, 0)