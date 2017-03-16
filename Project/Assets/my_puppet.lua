red = gr.material({0.545, 0.0, 0.0}, {0.9, 0.7, 0.9}, 25)
shiny_red = gr.material({0.7, 0.0, 0.0}, {0.9, 0.7, 0.9}, 25)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 0.54, 0.0}, {0.9, 0.9, 0.9}, 25)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
yellow = gr.material({0.96, 0.91, 0.86}, {0.1, 0.1, 0.1}, 100)
sunshine = gr.material({1.0, 0.83, 0.0}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
sand = gr.material({0.86, 0.82, 0.71}, {0.0, 0.0, 0.0}, 0)
water_col = gr.material({0.0, 0.41, 0.58}, {0.5, 0.5, 0.5}, 10)

rootnode = gr.node('root')
rootnode:rotate('X', 20)
rootnode:translate(0, 0, -8.0)

-- the floor

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
rootnode:add_child(plane)
plane:set_material(sand)
plane:scale(8, 8, 8)
plane:translate(0.0, -1.01, 0.0)

water = gr.mesh( 'plane', 'Assets/plane.obj' )
rootnode:add_child(water)
water:set_material(water_col)
water:scale(8, 8, 8)
water:translate(-10.3, -1.0, 0.0)

-- sun

sun = gr.mesh('sun', 'Assets/icosa.obj')
rootnode:add_child(sun)
sun:set_material(sunshine)
--sun:scale(1, 1.5, 1.5)
sun:translate(-5, 2, -10)

-- sand castle
middle_sand = gr.cube('middle sand')
rootnode:add_child(middle_sand)
middle_sand:set_material(sand)
middle_sand:scale(0.3, 0.3, 0.3)
middle_sand:translate(1, -1, 2)

left_sand = gr.cube('left sand')
middle_sand:add_child(left_sand)
left_sand:set_material(sand)
left_sand:scale(1/0.3, 1/0.3, 1/0.3)
left_sand:scale(0.4, 0.5, 0.3)
left_sand:translate(-1.33, 0, 0)

right_sand = gr.cube('right sand')
middle_sand:add_child(right_sand)
right_sand:set_material(sand)
right_sand:scale(1/0.3, 1/0.3, 1/0.3)
right_sand:scale(0.4, 0.5, 0.3)
right_sand:translate(1, 0, 0)

little_box = gr.cube('box')
little_box:set_material(sand)

for i = 1, 3 do
   a_box = gr.node('box' .. tostring(i))
   a_box:scale(1/0.4, 1/0.5, 1/0.3)
   a_box:scale(0.07, 0.13, 0.3)
   a_box:translate(0 + (i-1)/(2.5), 1, 0)
   left_sand:add_child(a_box)
   a_box:add_child(little_box)
end

for i = 1, 3 do
   a_box = gr.node('box' .. tostring(i))
   a_box:scale(1/0.4, 1/0.5, 1/0.3)
   a_box:scale(0.07, 0.1, 0.3)
   a_box:translate(0 + (i-1)/(2.5), 1, 0)
   right_sand:add_child(a_box)
   a_box:add_child(little_box)
end

-- bucket
bucket = gr.mesh('bucket', 'Assets/cylinder.obj')
rootnode:add_child(bucket)
bucket:set_material(green)
bucket:scale(0.17, 0.3, 0.17)
bucket:translate(0.4, -1, 2.75)

-- umbrella
pole = gr.mesh('bucket', 'Assets/cylinder.obj')
rootnode:add_child(pole)
pole:set_material(white)
pole:scale(0.1, 2, 0.1)
pole:translate(2, -1, -2.75)

umbrella = gr.sphere('umbrella')
pole:add_child(umbrella)
umbrella:set_material(green)
umbrella:scale(1/0.1, 1/3, 1/0.1)
umbrella:rotate('Z', -10)
umbrella:rotate('X', -5)
umbrella:scale(1.5, 0.1, 1.5)
umbrella:translate(0, 1, 0)

-- ball
ball = gr.sphere('ball')
rootnode:add_child(ball)
ball:set_material(shiny_red)
ball:scale(0.3, 0.3, 0.3)
ball:translate(2.8, -0.8, -1.5)

-- man

puppet_node = gr.node('puppet')
rootnode:add_child(puppet_node)
puppet_node:rotate('Y', -30)
puppet_node:scale(0.5, 0.5, 0.5)
puppet_node:translate(-0.8, 0.37, 1.0)

torso = gr.sphere('torso')
puppet_node:add_child(torso)
torso:set_material(red)
t_x_scale = 0.5
t_y_scale = 1.0
t_z_scale = 0.4
torso:scale(t_x_scale, t_y_scale, t_z_scale)

shoulder = gr.sphere('shoulder')
torso:add_child(shoulder)
shoulder:set_material(red)
shoulder:scale(1/t_x_scale, 1/t_y_scale, 1/t_z_scale)
sh_x_scale = 0.8
sh_y_scale = 0.15
sh_z_scale = 0.4
shoulder:scale(sh_x_scale, sh_y_scale, sh_z_scale)
shoulder:translate(0, 1 , 0)

-- Neck and Head

neck = gr.sphere('neck')
shoulder:add_child(neck)
neck:set_material(yellow)
n_x_scale = 0.2
n_y_scale = 0.2
n_z_scale = 0.2
neck:scale(1/sh_x_scale, 1/sh_y_scale, 1/sh_z_scale)
neck:scale(n_x_scale, n_y_scale, n_z_scale)
neck:translate(0.0, 0.25, 0.0)

upper_neck = gr.joint('upper_neck', {-30.0, 0.0, 40.0}, {-90.0, 0.0, 90.0})
neck:add_child(upper_neck)
upper_neck:scale(1/n_x_scale, 1/n_y_scale, 1/n_z_scale)

head = gr.sphere('head')
upper_neck:add_child(head)
head:set_material(yellow)
h_x_scale = 0.45
h_y_scale = 0.45
h_z_scale = 0.45
head:scale(h_x_scale, h_y_scale, h_z_scale)
head:translate(0.0, 0.55, 0.0)

left_eye = gr.sphere('left_eye')
head:add_child(left_eye)
left_eye:set_material(green)
left_eye:scale(1/h_x_scale, 1/h_y_scale, 1/h_z_scale)
e_x_scale = 0.05
e_y_scale = 0.05
e_z_scale = 0.05
left_eye:scale(e_x_scale, e_y_scale, e_z_scale)
left_eye:translate(-0.25, 0.2, 0.9)

right_eye = gr.sphere('right_eye')
head:add_child(right_eye)
right_eye:set_material(green)
right_eye:scale(1/h_x_scale, 1/h_y_scale, 1/h_z_scale)
right_eye:scale(e_x_scale, e_y_scale, e_z_scale)
right_eye:translate(0.25, 0.2, 0.9)

-- Upper Limbs
left_s_joint = gr.joint('left_s_joint', {-200.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
shoulder:add_child(left_s_joint)
left_s_joint:scale(1/sh_x_scale, 1/sh_y_scale, 1/sh_z_scale)

left_upper_arm = gr.sphere('left_upper_arm')
left_s_joint:add_child(left_upper_arm)
left_upper_arm:set_material(red)
ua_x_scale = 0.14
ua_y_scale = 0.4
ua_z_scale = 0.2
left_upper_arm:scale(ua_x_scale, ua_y_scale, ua_z_scale)
left_upper_arm:translate(-0.65, -0.45, 0.0)

right_s_joint = gr.joint('right_s_joint', {-200.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
shoulder:add_child(right_s_joint)
right_s_joint:scale(1/sh_x_scale, 1/sh_y_scale, 1/sh_z_scale)

right_upper_arm = gr.sphere('right_upper_arm')
right_s_joint:add_child(right_upper_arm)
right_upper_arm:set_material(red)
right_upper_arm:scale(ua_x_scale, ua_y_scale, ua_z_scale)
right_upper_arm:translate(0.65, -0.45, 0.0)

left_elbow = gr.joint('left_elbow', {-100.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
left_upper_arm:add_child(left_elbow)
left_elbow:scale(1/ua_x_scale, 1/ua_y_scale, 1/ua_z_scale)
left_elbow:translate(0.0, -0.68, 0.0)

left_lower_arm = gr.sphere('left_lower_arm')
left_elbow:add_child(left_lower_arm)
left_lower_arm:set_material(yellow)
la_x_scale = 0.14
la_y_scale = 0.3
la_z_scale = 0.2
left_lower_arm:scale(la_x_scale, la_y_scale, la_z_scale)
left_lower_arm:translate(0.0, -0.4, 0.0)

right_elbow = gr.joint('right_elbow', {-100.0, -50.0, 0.0}, {0.0, 0.0, 0.0})
right_upper_arm:add_child(right_elbow)
right_elbow:scale(1/ua_x_scale, 1/ua_y_scale, 1/ua_z_scale)
right_elbow:translate(0.0, -0.68, 0.0)

right_lower_arm = gr.sphere('right_lower_arm')
right_elbow:add_child(right_lower_arm)
right_lower_arm:set_material(yellow)
right_lower_arm:scale(la_x_scale, la_y_scale, la_z_scale)
right_lower_arm:translate(0.0, -0.4, 0.0)

left_wrist = gr.joint('left_wrist', {-80.0, 0.0, 25.0}, {0.0, 0.0, 0.0})
left_lower_arm:add_child(left_wrist)
left_wrist:scale(1/la_x_scale, 1/la_y_scale, 1/la_z_scale)
left_wrist:translate(0.0, -0.8, 0.0)

left_hand = gr.sphere('left_hand')
left_wrist:add_child(left_hand)
left_hand:set_material(yellow)
h_x_scale = 0.15
h_y_scale = 0.15
h_z_scale = 0.15
left_hand:scale(h_x_scale, h_y_scale, h_z_scale)
left_hand:translate(0.0, -0.2, 0.0)

right_wrist = gr.joint('right_wrist', {-80.0, 0.0, 25.0}, {0.0, 0.0, 0.0})
right_lower_arm:add_child(right_wrist)
right_wrist:scale(1/la_x_scale, 1/la_y_scale, 1/la_z_scale)
right_wrist:translate(0.0, -0.8, 0.0)

right_hand = gr.sphere('right_hand')
right_wrist:add_child(right_hand)
right_hand:set_material(yellow)
right_hand:scale(h_x_scale, h_y_scale, h_z_scale)
right_hand:translate(0.0, -0.2, 0.0)

-- Lower Limbs

hips = gr.sphere('hips')
torso:add_child(hips)
hips:set_material(blue)
hips:scale(1/t_x_scale, 1/t_y_scale, 1/t_z_scale)
hp_x_scale = 0.65
hp_y_scale = 0.15
hp_z_scale = 0.4
hips:scale(hp_x_scale, hp_y_scale, hp_z_scale)
hips:translate(0, -0.85 , 0)

left_h_joint = gr.joint('left_h_joint', {-150.0, 0.0, 20.0}, {0.0, 0.0, 0.0})
hips:add_child(left_h_joint)
left_h_joint:scale(1/hp_x_scale, 1/hp_y_scale, 1/hp_z_scale)

left_upper_leg = gr.sphere('left_upper_leg')
left_h_joint:add_child(left_upper_leg)
left_upper_leg:set_material(blue)
ul_x_scale = 0.17
ul_y_scale = 0.45
ul_z_scale = 0.2
left_upper_leg:scale(ul_x_scale, ul_y_scale, ul_z_scale)
left_upper_leg:translate(-0.4, -0.55, 0.0)

right_h_joint = gr.joint('right_h_joint', {-150.0, 0.0, 20.0}, {0.0, 0.0, 0.0})
hips:add_child(right_h_joint)
right_h_joint:scale(1/hp_x_scale, 1/hp_y_scale, 1/hp_z_scale)

right_upper_leg = gr.sphere('right_upper_leg')
right_h_joint:add_child(right_upper_leg)
right_upper_leg:set_material(blue)
right_upper_leg:scale(ul_x_scale, ul_y_scale, ul_z_scale)
right_upper_leg:translate(0.4, -0.55, 0.0)

left_knee = gr.joint('left_knee', {0.0, 0.0, 120.0}, {0.0, 0.0, 0.0})
left_upper_leg:add_child(left_knee)
left_knee:scale(1/ul_x_scale, 1/ul_y_scale, 1/ul_z_scale)
left_knee:translate(0.0, -0.8, 0.0)

left_lower_leg = gr.sphere('left_lower_leg')
left_knee:add_child(left_lower_leg)
left_lower_leg:set_material(blue)
ll_x_scale = 0.14
ll_y_scale = 0.375
ll_z_scale = 0.2
left_lower_leg:scale(ll_x_scale, ll_y_scale, ll_z_scale)
left_lower_leg:translate(0.0, -0.4, 0.0)

right_knee = gr.joint('right_knee', {0.0, 0.0, 120.0}, {0.0, 0.0, 0.0})
right_upper_leg:add_child(right_knee)
right_knee:scale(1/ul_x_scale, 1/ul_y_scale, 1/ul_z_scale)
right_knee:translate(0.0, -0.8, 0.0)

right_lower_leg = gr.sphere('right_lower_leg')
right_knee:add_child(right_lower_leg)
right_lower_leg:set_material(blue)
right_lower_leg:scale(ll_x_scale, ll_y_scale, ll_z_scale)
right_lower_leg:translate(0.0, -0.4, 0.0)

left_ankle = gr.joint('left_ankle', {-60.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
left_lower_leg:add_child(left_ankle)
left_ankle:scale(1/ll_x_scale, 1/ll_y_scale, 1/ll_z_scale)
left_ankle:translate(0.0, -0.4, 0.25)

left_foot = gr.sphere('left_foot')
left_ankle:add_child(left_foot)
left_foot:set_material(white)
f_x_scale = 0.15
f_y_scale = 0.15
f_z_scale = 0.25
left_foot:scale(f_x_scale, f_y_scale, f_z_scale)
left_foot:translate(0.0, -0.3, 0.1)

right_ankle = gr.joint('right_ankle', {-60.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
right_lower_leg:add_child(right_ankle)
right_ankle:scale(1/ll_x_scale, 1/ll_y_scale, 1/ll_z_scale)
right_ankle:translate(0.0, -0.4, 0.25)

right_foot = gr.sphere('right_foot')
right_ankle:add_child(right_foot)
right_foot:set_material(white)
right_foot:scale(f_x_scale, f_y_scale, f_z_scale)
right_foot:translate(0.0, -0.3, 0.1)

-- The lights
l1 = gr.light({-3.85, 2, -13}, {1.0, 1.0, 1.0}, {1, 0, 0})
l2 = gr.light({-150, 50, 400}, {0.8, 0.8, 0.8}, {1, 0, 0})

gr.render(rootnode, 'sample.png', 500, 500,
{0, 0, 0,}, {0, 0, -1}, {0, 1, 0}, 50,
{0.4, 0.4, 0.4}, {l1, l2})
