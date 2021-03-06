mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 25, 0)
mat2 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 25, 0)
mat3 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 25, 0)

lblocks = gr.node('root')

block1 = gr.cube('b1')
lblocks:add_child(block1)
block1:set_material(mat1)
block1:set_texture("Assets/wood_block_A.png")
block1:set_bump("Assets/wood_block_A_bump.png")
block1:scale(2, 2, 2)
block1:translate(0, 2, 0)

block2 = gr.cube('b2')
lblocks:add_child(block2)
block2:set_material(mat2)
block2:set_texture("Assets/wood_block_B.png")
block2:set_bump("Assets/wood_block_B_bump.png")
block2:scale(2, 2, 2)
block2:translate(1.8, 0, 0)
block2:rotate('y', -15)

block3 = gr.cube('b3')
lblocks:add_child(block3)
block3:set_material(mat3)
block3:set_texture("Assets/wood_block_C.png")
block3:set_bump("Assets/wood_block_C_bump.png")
block3:scale(2, 2, 2)
block3:translate(-1.8, 0, 0)
block3:rotate('y', 15)