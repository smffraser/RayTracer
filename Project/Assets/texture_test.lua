mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 25, 0)
mat2 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 25, 0)
mat3 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 25, 0)

scene = gr.node('root')

block1 = gr.cube('b1')
scene:add_child(block1)
block1:set_material(mat1)
block1:set_texture("Assets/wood_block_A.png")
block1:set_bump("Assets/wood_block_A_bump-1.png")
block1:scale(2, 2, 2)
block1:translate(0, 2, 0)

block2 = gr.cube('b2')
scene:add_child(block2)
block2:set_material(mat2)
block2:set_texture("Assets/wood_block_B.png")
--block1:set_bump("Assets/brick-bump.png")
block2:scale(2, 2, 2)
block2:translate(1.8, 0, 0)
block2:rotate('y', -15)

block3 = gr.cube('b3')
scene:add_child(block3)
block3:set_material(mat3)
block3:set_texture("Assets/wood_block_C.png")
--block1:set_bump("Assets/brick-bump.png")
block3:scale(2, 2, 2)
block3:translate(-1.8, 0, 0)
block3:rotate('y', 15)


white_light = gr.light({200, 130, 3}, {0.9, 0.9, 0.9}, {1, 0, 0}, 0, 0)

scene:rotate('y', 30)
scene:rotate('x', 20)

-- threads, reflec level, refract level, glossy rays, shadow rays
gr.render(scene, 'texture.png', 500, 500,
{1.5, 1, 10}, {0, 0, -800}, {0, 1, 0}, 50,
{0.3, 0.3, 0.3}, {white_light}, 16, 5, 5, 0, 0, 5, 0.0001, 1)