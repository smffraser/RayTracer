-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.5, 0.5}, 25, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0)

scene_root = gr.node('root')

c1 = gr.nh_cone('c1', {0, 0, 0}, 100)
scene_root:add_child(c1)
c1:set_material(mat1)
c1:rotate('X', 45)
c1:rotate('Y', 120)
c1:scale(1.5, 1.5, 1.5)
c1:translate(-100, -1.5, 0)

s4 = gr.nh_cylinder('s4', {0,0,0}, 50, 100)
--s4 = gr.nh_sphere('s4', {0,0,0}, 50)
scene_root:add_child(s4)
s4:set_material(mat1)
s4:rotate('Y', 45)
s4:scale(1.5, 1.5, 1.5)
s4:translate(300, 0, -200)


white_light = gr.light({100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 150, 150)
--white_light = gr.light({0, 0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 0, 0)
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0}, 0, 0)


-- threads, reflec level, refract level, glossy rays, shadow rays
gr.render(scene_root, 'prims.png', 500, 500,
{0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
{0.3, 0.3, 0.3}, {white_light, orange_light}, 1, 5, 5, 0, 0)