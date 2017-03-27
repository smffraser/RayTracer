mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25,0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25,0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.0, 0.0, 0.0}, 25,0)
mat4 = gr.material({0.0, 0.7, 0.0}, {0.0, 0.0, 0.0}, 25,0)
glass = gr.material({0.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, 25,0)

red = gr.material({1, 0, 0}, {0, 0, 0}, 25,0)
blue = gr.material({0, 0, 1}, {0, 0, 0}, 25,0)
green = gr.material({0, 1, 0}, {0, 0, 0}, 25,0)
yellow = gr.material({1, 1, 0}, {0, 0, 0}, 25,0)

scene_root = gr.node('root')
scene_root:translate(0, 0, 0)
scene_root:rotate('z', 0)
scene_root:scale(2, 2, 2)

corner = gr.node('corner')
scene_root:add_child(corner)

p1 = gr.mesh( 'plane', 'Assets/plane.obj' )
corner:add_child(p1)
p1:set_material(red)
p1:rotate('x', 90)
p1:scale(10, 10, 10)
p1:translate(0, 0, -10)

p2 = gr.mesh( 'plane', 'Assets/plane.obj' )
corner:add_child(p2)
p2:set_material(blue)
p2:rotate('z', -90)
p2:scale(10.2, 10.2, 10.2)
p2:translate(-10, 0, 0)

p3 = gr.mesh( 'plane', 'Assets/plane.obj' )
corner:add_child(p3)
p3:set_material(green)
p3:scale(10.3, 10.3, 13)
p3:translate(0, -5, 0)

corner:rotate('y', -30)

s1 = gr.nh_box('s1', {0, 0, 0}, 1, 1)
corner:add_child(s1)
s1:rotate('y', -90)
s1:rotate('z', -90)
s1:scale(2, 2, 2)
s1:translate(-3, -2, 0)

s1:set_material(glass)

s2 = gr.nh_sphere('s2', {0, 0, 0}, 1)
--s1:add_child(s2)
s2:scale(1/4, 1/4, 1/4)
s2:scale(2, 2, 2)
s2:rotate('z', 90)
s2:translate(-2, 0, 5)
s2:set_material(mat2)

white_light = gr.light({5, 10, 10}, {0.9, 0.9, 0.9}, {1, 0, 0}, 0, 0)
--orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'refract_corner.png', 500, 500,
{0, 5, 40}, {0, 0, -800}, {0, 1, 0}, 50,
{0.3, 0.3, 0.3}, {white_light}, 4, 5, 5, 0, 0)