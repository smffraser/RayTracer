
jack = gr.node('jack')

--silver = gr.material({0.50754, 0.50754, 0.50754}, {0.0, 0.0, 0.0}, 51.2, 0)
silver = gr.material({0.50754, 0.50754, 0.50754}, {0.4, 0.4, 0.4}, 100000000, 0)

scale_amt = 0.2
s_scale_amt = 0.22
scale_nb = 0.15


--- Side 1

side1 = gr.node('side1')
jack:add_child(side1)

cone_z = 1.3
cyl_z = 0.5

c1 = gr.cone('c1')
side1:add_child(c1)
c1:set_material(silver)
c1:scale(scale_amt, scale_amt, cone_z)
c1:translate(0, 0, cone_z+(cyl_z/2))


c2 = gr.cone('c2')
side1:add_child(c2)
c2:set_material(silver)
c2:rotate('x', 180)
c2:scale(scale_amt, scale_amt, cone_z)
c2:translate(0, 0, -(cone_z+(cyl_z/2)))


cl1 = gr.cylinder('cl1')
side1:add_child(cl1)
cl1:set_material(silver)
cl1:scale(scale_amt, scale_amt, cyl_z)

s1 = gr.sphere('s1')
side1:add_child(s1)
s1:set_material(silver)
s1:scale(s_scale_amt, s_scale_amt, s_scale_amt)
s1:translate(0, 0, -cone_z)

s2 = gr.sphere('s2')
side1:add_child(s2)
s2:set_material(silver)
s2:scale(s_scale_amt, s_scale_amt, s_scale_amt)
s2:translate(0, 0, cone_z)


-- Side 2


side2 = gr.node('side2')
jack:add_child(side2)
side2:rotate('y', 90)

c3 = gr.cone('c3')
side2:add_child(c3)
c3:set_material(silver)
c3:scale(scale_amt, scale_amt, cone_z)
c3:translate(0, 0, cone_z+(cyl_z/2))


c4 = gr.cone('c4')
side2:add_child(c4)
c4:set_material(silver)
c4:rotate('x', 180)
c4:scale(scale_amt, scale_amt, cone_z)
c4:translate(0, 0, -(cone_z+(cyl_z/2)))


cl2 = gr.cylinder('cl2')
side2:add_child(cl2)
cl2:set_material(silver)
cl2:scale(scale_amt, scale_amt, cyl_z)

s3 = gr.sphere('s3')
side2:add_child(s3)
s3:set_material(silver)
s3:scale(s_scale_amt, s_scale_amt, s_scale_amt)
s3:translate(0, 0, -cone_z)

s4 = gr.sphere('s4')
side2:add_child(s4)
s4:set_material(silver)
s4:scale(s_scale_amt, s_scale_amt, s_scale_amt)
s4:translate(0, 0, cone_z)


-- Side 3


side3 = gr.node('side3')
jack:add_child(side3)
side3:rotate('x', 90)

c5 = gr.cone('c5')
--side3:add_child(c5)
c5:set_material(silver)
c5:scale(scale_amt, scale_amt, cone_z)
c5:translate(0, 0, cone_z+(cyl_z/2))


c6 = gr.cone('c6')
--side3:add_child(c6)
c6:set_material(silver)
c6:rotate('x', 180)
c6:scale(scale_amt, scale_amt, cone_z)
c6:translate(0, 0, -(cone_z+(cyl_z/2)))


cl3 = gr.cylinder('cl3')
side3:add_child(cl3)
cl3:set_material(silver)
cl3:scale(scale_nb, scale_nb, cyl_z+((cone_z-scale_amt*2)*2))

s5 = gr.sphere('s5')
--side3:add_child(s5)
s5:set_material(silver)
s5:scale(scale_amt, scale_amt, scale_amt)
s5:translate(0, 0, -cone_z)

s6 = gr.sphere('s6')
--side3:add_child(s6)
s6:set_material(silver)
s6:scale(scale_amt, scale_amt, scale_amt)
s6:translate(0, 0, cone_z)
