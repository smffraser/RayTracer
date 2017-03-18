-- A simple test scene featuring some spherical cows grazing
-- around Stonehenge.  "Assume that cows are spheres..."

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0, 0)

-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:rotate('X', 23)

-- the floor

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)

-- Construct a central altar in the shape of a buckyball.  The
-- buckyball at the centre of the real Stonehenge was destroyed
-- in the great fire of 733 AD.

buckyball = gr.mesh( 'buckyball', 'Assets/buckyball.obj' )
scene:add_child(buckyball)
buckyball:set_material(stone)
buckyball:scale(1.5, 1.5, 1.5)
buckyball:translate(0, 8, 10)

gr.render(scene,
'simple-a.png', 512, 512,
{0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
{0.4, 0.4, 0.4}, {gr.light({20, 20, 43}, {0.8, 0.8, 0.8}, {1, 0, 0}, 5.0, 5.0)}, 8, 5, 1, 0, 180)