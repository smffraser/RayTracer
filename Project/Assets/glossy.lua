

-- Scene root
scene = gr.node('scene')

-- Mirror Materials
mirror0 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 0.01, 0)
mirror1 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 0.1, 0)
mirror2 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 1.0, 0)
mirror3 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 10.0, 0)
mirror4 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 100.0, 0)
mirror5 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 1000.0, 0)

-- Colours
grey = gr.material({0.74, 0.742, 0.734}, {0, 0, 0}, 0, 0)
grey_floor = gr.material({0.74, 0.742, 0.734}, {0, 0, 0}, 0, 0)
green = gr.material({0.8, 1.0, 0.8}, {0, 0, 0}, 0, 0)
pink = gr.material({1.0, 0.6, 0.8}, {0, 0, 0}, 0, 0)
blue = gr.material({0.6, 0.8, 1.0}, {0, 0, 0}, 0, 0)

-- Make Room Holding Spheres
room_width = 13.0
room_height = 13.0
room_length = 30.0
room = gr.node('room')
scene:add_child(room)

-- Room Floor
floor = gr.plane('floor')
room:add_child(floor)
floor:set_material(pink)
floor:set_texture('Assets/toy_box1.png')
floor:scale(room_width, room_length, 1)
floor:rotate('x', -90)
floor:translate(0.0, -1, 0.0)

-- Room Ceiling
ceiling = gr.plane('ceiling')
room:add_child(ceiling)
ceiling:set_material(blue)
ceiling:set_texture('Assets/wallpaper.png')
ceiling:rotate('x', 90)
ceiling:translate(0.0, room_height-2.0, 0.0)
ceiling:scale(room_width, 1, room_length)

-- Room Left Wall
left_wall = gr.plane('left_wall')
room:add_child(left_wall)
left_wall:set_material(pink)
left_wall:rotate('y', 90)
left_wall:scale(1, room_height, room_length)
--left_wall:translate(-room_width/2.0-1, room_height/2.0-1, 0.0)
left_wall:translate(-5, 3, 0.0)

-- Room Right Wall
right_wall = gr.plane('right_wall')
room:add_child(right_wall)
right_wall:set_material(pink)
right_wall:rotate('y', -90)
right_wall:scale(1, room_height, room_length)
--right_wall:translate(-room_width/2.0+1, room_height/2.0-1.0, 0.0)
right_wall:translate(5, 3, 0)


-- Room Back Wall
back_wall = gr.plane('back_wall')
room:add_child(back_wall)
back_wall:set_material(pink)
back_wall:rotate('y', 180)
back_wall:scale(room_width, room_height, 1.0)
back_wall:translate(0.0, 3, room_length/6)


-- Room Front Wall
front_wall = gr.plane('front_wall')
room:add_child(front_wall)
front_wall:set_material(pink)
front_wall:scale(room_width, room_height, 1)
front_wall:translate(0.0, 5, -room_length/2)

-- Make Spheres
radius = 1.0

s0 = gr.sphere('s0')
scene:add_child(s0)
s0:set_material(mirror5)
s0:scale(radius, radius, radius)
s0:translate(3, radius-1, -8)

s1 = gr.sphere('s1')
scene:add_child(s1)
-- mirror 1
s1:set_material(mirror1)
s1:translate(-3, radius-1, -8)
s1:scale(radius, radius, radius)

s2 = gr.sphere('s2')
scene:add_child(s2)
-- mirror 2
s2:set_material(mirror2)
s2:translate(-2, radius-1, -12)
s2:scale(radius, radius, radius)

s3 = gr.sphere('s3')
scene:add_child(s3)
--3
s3:set_material(mirror3)
s3:translate(0, radius-1, -8)
s3:scale(radius, radius, radius)

s4 = gr.sphere('s4')
scene:add_child(s4)
-- 4
s4:set_material(mirror4)
s4:translate(2, radius-1, -12)
s4:scale(radius, radius, radius)

-- Lights
light_color = {0.780131, 0.780409, 0.775833}
light_color_2 = {0.780131/2, 0.780409/2, 0.775833/2}

light1 = gr.light({0, room_height - 3.0, -3}, light_color_2, {1, 0, 0}, 0, 0)
light2 = gr.light({-2.0, room_height - 3.0, -3}, light_color_2, {1, 0, 0}, 0, 0)


-- Render Final Scene

-- threads, reflec level, refract level, glossy rays, shadow rays, AA_level, threshold, highlight, AA_on/off

--room_height/2.0-1.0

gr.render(scene,'glossy.png', 500, 500, {0, room_height/2.0-1.0, 0}, {0, -room_height, -room_length}, {0, 0, -1}, 50, {0.2,0.2,0.2}, {light1, light2}, 32, 5, 5, 32, 0, 0, 0.01, 0, 1)