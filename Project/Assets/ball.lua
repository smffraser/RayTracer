red = gr.material({1.0, 0.0, 0.0},{0, 0, 0}, 0, 0)
other = gr.material({1.0, 1.0, 0.0}, {0, 0, 0}, 0, 0)

scene_root = gr.node('scene_root')


floor = gr.plane('floor')
scene_root:add_child(floor)
floor:set_material(other)
floor:set_texture('Assets/toy_box1.png')
floor:set_bump('Assets/toy_box_bump.png')
floor:rotate('x',-90)
floor:scale(10, 10, 10)
floor:translate(0, -1.5, 0)


wall = gr.plane('wall')
scene_root:add_child(wall)
wall:set_material(other)
wall:scale(10, 10, 10)
wall:translate(0, 0, -3)


ball = gr.sphere('ball')
scene_root:add_child(ball)
ball:set_material(red)
ball:set_bump('Assets/ball_red_bump.png')
ball:scale(1.5, 1.5, 1.5)
ball:translate(1.5, 0, 0)

require('Assets/l_blocks')
scene_root:add_child(lblocks)
lblocks:scale(0.5, 0.5, 0.5)
lblocks:translate(-2, -1.5, 0)



scene_root:rotate('x', 20)


num_renders = 4
move = 5

-- Animation
for i=0,num_renders,1 do
    white_light = gr.light({-10 + (move * i), 5, 10}, {0.5, 0.5, 0.5}, {1, 0, 0}, 0, 0)
    gr.render(scene_root, string.format('bump_%i.png', i), 500, 500, {0, 0, 7.5}, {0, 0, -1}, {0, 1, 0}, 50, {0.3, 0.3, 0.3}, {white_light}, 4, 5, 5, 0, 0, 0, 0.01, 0, 1)
end