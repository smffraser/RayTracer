red = gr.material({1.0, 0.0, 0.0},{0, 0, 0}, 100, 0)
crappy_mirror = gr.material({0,0,0}, {1, 1, 1}, 100, 0)

scene_root = gr.node('scene_root')

p1 = gr.plane('p1')
scene_root:add_child(p1)
p1:set_material(red)
--p1:set_texture('Assets/brickwork.png')
p1:set_texture('Assets/wood-flooring.png')
--p1:set_bump('Assets/brick-bump.png')
--p1:rotate('x', 90)
--p1:rotate('y', 180)
p1:scale(4, 4, 4)
p1:translate(0, 0, -1)

white_light = gr.light({0, 5, 5}, {0.9, 0.9, 0.9}, {1, 0, 0}, 0, 0)

gr.render(scene_root, 'bump_test.png', 800, 800,
{0, 0, 5}, {0, 0, -1}, {0, 1, 0}, 50,
{0.3, 0.3, 0.3}, {white_light}, 1, 5, 5, 0, 0)