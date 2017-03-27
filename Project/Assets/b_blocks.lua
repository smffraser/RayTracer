blocks = gr.node('root')

green = gr.material({0.0, 0.54, 0.0}, {0.0, 0.0, 0.0}, 25, 0)
red = gr.material({0.54, 0.0, 0.0}, {0.0, 0.0, 0.0}, 25, 0)
rred = gr.material({0.54, 0.0, 0.0}, {0.0, 0.0, 0.0}, 25, 0)
blue = gr.material({0.0, 0.0, 0.54}, {0.0, 0.0, 0.0}, 25, 0)


-- Rectangular Blocks
box_x = 1
box_y = 3
box_z = 1

b1 = gr.cube('b1')
blocks:add_child(b1)
b1:set_material(green)
b1:scale(box_x, box_y, box_z)
b1:translate(-1, 0, -0.25)

b2 = gr.cube('b2')
blocks:add_child(b2)
b2:set_material(red)
b2:scale(box_x, box_y, box_z)
b2:rotate('z', 90)
b2:rotate('y', 10)
b2:translate(4.5, 0, 0)


b3 = gr.cube('b3')
blocks:add_child(b3)
b3:set_material(blue)
b3:scale(box_x, box_y, box_z)
b3:rotate('z', 40)
b3:translate(1.7, 1, -0.7)

b4 = gr.cube('b4')
blocks:add_child(b4)
b4:set_material(red)
b4:scale(box_x, box_y, box_z)
b4:rotate('z', 90)
b4:rotate('y', -10)
b4:translate(2.5, 0, -1.1)

-- Cylinders
cyl_x = 0.6
cyl_y = 0.6
cyl_z = 1

c1 = gr.cylinder('c1')
blocks:add_child(c1)
c1:set_material(blue)
c1:rotate('x', -90)
c1:scale(cyl_x, cyl_z, cyl_y)
c1:translate(3.3, 1.5, 1)

c2 = gr.cylinder('c2')
blocks:add_child(c2)
c2:set_material(red)
c2:rotate('x', -90)
c2:scale(cyl_x, cyl_z, cyl_y)
c2:translate(-1.4, 1.8333, 1.5)

block4 = gr.cube('b4')
c2:add_child(block4)
block4:set_material(rred)
block4:set_texture("Assets/wood_block_B.png")
block4:scale(1/cyl_x, 1/cyl_y, 1/cyl_z)
block4:scale(1.333, 1.333, 1.333)
block4:rotate('z', 23)
block4:translate(-0.7, -1.6, -1.8)



-- Cones

cn_x = 0.75
cn_y = 0.75
cn_z = 1.5

cn1 = gr.cone('cn1')
blocks:add_child(cn1)
cn1:set_material(green)
cn1:scale(cn_x, cn_y, cn_z)
cn1:rotate('y', -90)
cn1:rotate('z', 68)
cn1:translate(0.7, 0, 1)

cn2 = gr.cone('cn1')
blocks:add_child(cn2)
cn2:set_material(green)
cn2:scale(cn_x, cn_y, cn_z)
cn2:rotate('x', -90)
cn2:translate(3.3, 3.5, 1)


-- Render

--white_light = gr.light({5, 10, 40}, {0.9, 0.9, 0.9}, {1, 0, 0}, 0, 0)

--blocks:rotate('y', -20)
--blocks:rotate('z', 90)
--blocks:rotate('x', 90)

--gr.render(blocks, string.format("blocks.png"), 350, 350, {-2, 0, 10}, {5, 8, -10}, {0, 1, 0}, 50, {0.3, 0.3, 0.3}, {white_light}, 4, 5, 5, 0, 0, 0, 1, 0)