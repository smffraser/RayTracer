
grey = gr.material({0.74, 0.742, 0.734}, {0, 0, 0}, 0, 0)
grey_floor = gr.material({0.74, 0.742, 0.734}, {0, 0, 0}, 0, 0)
floor_mat = gr.material({0.8, 1.0, 0.8}, {0, 0, 0}, 0, 0)
pink = gr.material({1.0, 0.6, 0.8}, {0, 0, 0}, 0, 0)
blue = gr.material({0.6, 0.8, 1.0}, {0, 0, 0}, 0, 0)


full_scene = gr.node('root')

-- Box

b_l = 20
b_w = 15
b_h = 5

-- floor plane
floor = gr.plane('floor')
full_scene:add_child(floor)
floor:set_material(floor_mat)
floor:set_texture('Assets/toy_box1.png')
floor:rotate('x', -90)
floor:scale(b_w, b_h, b_l)
--floor:translate(0, 0, b_l/2)

-- Back wall plane
bwall = gr.plane('bwall')
full_scene:add_child(bwall)
bwall:set_material(floor_mat)
--bwall:set_texture('Assets/toy_box1.png')
bwall:scale(b_w, b_h, 1)
bwall:translate(0, b_h/2, -b_l/2)

-- Front wall plane
fwall = gr.plane('fwall')
full_scene:add_child(fwall)
fwall:set_material(floor_mat)
--fwall:set_texture('Assets/toy_box1.png')
fwall:rotate('x', 180)
fwall:scale(b_w, b_h, 1)
fwall:translate(0, b_h/2, b_l/2)

-- Left Wall plane
lwall = gr.plane('lwall')
full_scene:add_child(lwall)
lwall:set_material(floor_mat)
--lwall:set_texture('Assets/toy_box1.png')
lwall:rotate('y', 90)
lwall:scale(1, b_h, b_l)
lwall:translate(-b_w/2, b_h/2, 0)

-- Right Wall plane
rwall = gr.plane('rwall')
full_scene:add_child(rwall)
rwall:set_material(floor_mat)
--rwall:set_texture('Assets/toy_box1.png')
rwall:rotate('y', -90)
rwall:scale(1, b_h, b_l)
rwall:translate(b_w/2, b_h/2, 0)


-- Objects

scene = gr.node('objects')
full_scene:add_child(scene)
scene:translate(0, 0, -b_l/4)

-- blocks
require('Assets/b_blocks')
scene:add_child(blocks)
blocks:rotate('y', 10)
blocks:scale(1.2, 1.2, 1.2)
blocks:translate(-5, 0, -3)

-- music box
require('Assets/music_box')
scene:add_child(box)
box:rotate('y', -25)
box:scale(1.5, 1.5, 1.5)
box:translate(4.5, 3, -4)

-- letter blocks
require('Assets/l_blocks')
scene:add_child(lblocks)
lblocks:scale(0.8, 0.8, 0.8)
lblocks:rotate('y', -60)
lblocks:translate(6, 0, 1)

block4 = gr.cube('b4')
--scene:add_child(block4)
block4:set_material(mat3)
block4:set_texture("Assets/wood_block_C.png")
--block1:set_bump("Assets/brick-bump.png")
block4:scale(1.6, 1.6, 1.6)
--block4:translate(-1.8, 0, 0)
--block4:rotate('y', 15)


-- Playing cards

card_w = 1.75
card_l = 2.45

c_mat = gr.material({0.6, 0.8, 1.0}, {0, 0, 0}, 0, 0)
c_mat1 = gr.material({0.6, 0.8, 1.0}, {0, 0, 0}, 0, 0)
c_mat2 = gr.material({0.6, 0.8, 1.0}, {0, 0, 0}, 0, 0)
c_mat3 = gr.material({0.6, 0.8, 1.0}, {0, 0, 0}, 0, 0)
c_mat4 = gr.material({0.6, 0.8, 1.0}, {0, 0, 0}, 0, 0)
c_mat5 = gr.material({0.6, 0.8, 1.0}, {0, 0, 0}, 0, 0)



card1 = gr.plane('cd1')
scene:add_child(card1)
card1:set_material(c_mat)
card1:set_texture('Assets/card_back.png')
card1:rotate('x', -90)
card1:rotate('y',-20)
card1:scale(card_w, 1, card_l)
card1:translate(-6.5, 0.09, 2.8)

card3 = gr.plane('cd3')
card1:add_child(card3)
card3:set_material(c_mat2)
card3:set_texture('Assets/card_back.png')
card3:rotate('z', 20)
card3:translate(0.3, 0.3, -0.01)

card4 = gr.plane('cd4')
scene:add_child(card4)
card4:set_material(c_mat3)
card4:set_texture('Assets/4_hearts.png')
card4:rotate('x', -90)
card4:rotate('y',-45)
card4:scale(card_w, 1, card_l)
card4:translate(-1, 0.09, 1)

card5 = gr.plane('cd5')
scene:add_child(card5)
card5:set_material(c_mat4)
card5:set_texture('Assets/card_back.png')
card5:rotate('x', -90)
card5:rotate('y',30)
card5:scale(card_w, 1, card_l)
card5:translate(-1, 0.1, 2)

card6 = gr.plane('cd6')
scene:add_child(card6)
card6:set_material(c_mat5)
card6:set_texture('Assets/ace_clubs.png')
card6:rotate('x', -90)
card6:rotate('y',-90)
card6:scale(card_w, 1, card_l)
card6:translate(2.5, 0.09, 2)

card2 = gr.plane('cd2')
scene:add_child(card2)
card2:set_material(c_mat1)
card2:set_texture('Assets/8_spades.png')
card2:rotate('y', 90)
card2:rotate('z', 20)
card2:scale(1, card_l, card_w)
card2:translate(-7.17, card_l/2, 2.5)

-- Jacks

j_s = 0.35

require('Assets/jack')
jack1 = gr.node('jack1')
scene:add_child(jack1)
jack1:add_child(jack)
jack1:rotate('y', 45)
jack1:rotate('z', -40)
jack1:scale(j_s, j_s, j_s)
jack1:translate(4, 0.34, 3.5)

jack2 = gr.node('jack2')
scene:add_child(jack2)
jack2:add_child(jack)
jack2:rotate('y', 45)
jack2:rotate('z', 40)
jack2:scale(j_s, j_s, j_s)
jack2:translate(-4.7, 0.34, 0.1)

jack3 = gr.node('jack3')
scene:add_child(jack3)
jack3:add_child(jack)
jack3:rotate('y', 45)
jack3:rotate('z', 40)
jack3:rotate('y', 20)
jack3:scale(j_s, j_s, j_s)
jack3:translate(0, 0.34, -1)

-- Marbles

m_s = 0.3

glass_t = gr.material({0.0, 0.0, 0.0}, {0.25, 0.25, 0.25}, 100000, 1.52)
--glass_t = gr.material({1.0, 0.0, 1.0}, {0.25, 0.25, 0.25}, 100000, 0)

m1 = gr.sphere('m1')
scene:add_child(m1)
m1:set_material(glass_t)
m1:scale(m_s, m_s, m_s)
m1:translate(-5, 0.3, 1)

m2 = gr.sphere('m2')
scene:add_child(m2)
m2:set_material(glass_t)
m2:scale(m_s, m_s, m_s)
m2:translate(-2.5, 0.3, 3)

m3 = gr.sphere('m3')
scene:add_child(m3)
m3:set_material(glass_t)
m3:scale(m_s, m_s, m_s)
m3:translate(3.9, 0.3, 1.5)

m4 = gr.sphere('m4')
scene:add_child(m4)
m4:set_material(glass_t)
m4:scale(m_s, m_s, m_s)
m4:translate(1.5, 0.3, -3.5)



-- Render

full_scene:rotate('y', 2)
full_scene:rotate('x', 10)

--


white_light = gr.light({-8, 8, 10}, {0.7, 0.7, 0.7}, {1, 0, 0}, 0, 0)

gr.render(full_scene, string.format("final.png"), 1000, 500, {-3, 6, 5.5}, {0, 0, -10}, {0, 1, 0}, 50, {0.3, 0.3, 0.3}, {white_light}, 8, 5, 5, 0, 0, 0, 1, 0)