

-- Scene root
scene = gr.node('scene')

-- Mirror Materials
mirror0 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 0.1, 0)
mirror1 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 1.0, 0)
mirror2 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 10.0, 0)
mirror3 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 100.0, 0)
mirror4 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 1000.0, 0)
mirror5 = gr.material({0, 0, 0}, {1.0, 1.0, 1.0}, 10000.0, 0)

-- Colours
grey = gr.material({0.74, 0.742, 0.734}, {0, 0, 0}, 0, 0)
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
floor = gr.mesh('floor', 'Assets/plane.obj')
room:add_child(floor)
floor:set_material(grey)
floor:translate(0.0, -1.0, 0.0)
floor:scale(room_width, 1, room_length)

-- Room Ceiling
ceiling = gr.mesh('ceiling','Assets/plane.obj')
room:add_child(ceiling)
ceiling:set_material(grey)
ceiling:translate(0.0, room_height-2.0, 0.0)
--ceiling:rotate('Z', 180.0)
ceiling:scale(room_width, 1, room_length)

-- Room Left Wall
left_wall = gr.mesh('left_wall', 'Assets/plane.obj')
room:add_child(left_wall)
left_wall:set_material(green)
left_wall:scale(room_height, 1.0, room_length)
left_wall:rotate('Z', 90)
left_wall:translate(room_width/2.0-1, room_height/2.0-1, 0.0)

-- Room Right Wall
right_wall = gr.mesh('right_wall', 'Assets/plane.obj')
room:add_child(right_wall)
right_wall:set_material(pink)
right_wall:scale(room_height, 1.0, room_length)
right_wall:rotate('Z', -90)
right_wall:translate(-room_width/2.0+1, room_height/2.0-1.0, 0.0)


-- Room Back Wall
back_wall = gr.mesh('back_wall', 'Assets/plane.obj')
room:add_child(back_wall)
back_wall:set_material(grey)
back_wall:scale(room_width, 1.0, room_height)
back_wall:rotate('X', -90.0)
back_wall:translate(0.0, room_height/2.0-1.0, room_length/6)


-- Room Front Wall
front_wall = gr.mesh('front_wall', 'Assets/plane.obj')
room:add_child(front_wall)
front_wall:set_material(blue)
front_wall:scale(room_width, 1.0, room_height)
front_wall:rotate('x', 90.0)
front_wall:translate(0.0, room_height/2.0-1.0, -room_length/2)

-- Make Spheres
radius = 1.0

s0 = gr.sphere('s0')
scene:add_child(s0)
s0:set_material(mirror5)
s0:scale(radius, radius, radius)
s0:translate(3, radius-1, -8)

s1 = gr.sphere('s1')
scene:add_child(s1)
s1:set_material(mirror1)
s1:translate(-3, radius-1, -8)
s1:scale(radius, radius, radius)

s2 = gr.sphere('s2')
scene:add_child(s2)
s2:set_material(mirror2)
s2:translate(-2, radius-1, -12)
s2:scale(radius, radius, radius)

s3 = gr.sphere('s3')
scene:add_child(s3)
s3:set_material(mirror3)
s3:translate(0, radius-1, -8)
s3:scale(radius, radius, radius)

s4 = gr.sphere('s4')
scene:add_child(s4)
s4:set_material(mirror4)
s4:translate(2, radius-1, -12)
s4:scale(radius, radius, radius)

-- Lights
light_color = {0.780131, 0.780409, 0.775833}
light_color_2 = {0.780131/2, 0.780409/2, 0.775833/2}

light1 = gr.light({0, room_height - 3.0, -3}, light_color_2, {1, 0, 0})
light2 = gr.light({-2.0, room_height - 3.0, -3}, light_color_2, {1, 0, 0})


-- Render Final Scene
gr.render(scene,'glossy.png', 512, 512, {0, room_height/2.0-1.0, 0}, {0, -room_height, -room_length}, {0, 0, -1}, 50, {0.2,0.2,0.2}, {light1, light2}, 8, 4, 1, 1)