-- A music box scene

-- materials
brown = gr.material({0.655, 0.494, 0.239}, {0.0, 0.0, 0.0}, 25, 0)
blue = gr.material({0.549, 0.745, 0.839}, {0.0, 0.0, 0.0}, 25, 0)
mirror_mat = gr.material({0.0, 0.0, 0.0}, {0.9, 0.9, 0.9}, 1000, 0)
gold = gr.material({0.75164, 0.60648, 0.22648}, {0.628281, 0.555802, 0.366065}, 51.2, 0)

-- Scene
scene = gr.node('root')

-- Music Box
box_scale = 2

box = gr.cube('box')
scene:add_child(box)
box:set_material(brown)
box:set_texture("../Assets/fine_wood.png")
box:scale(box_scale, box_scale*0.5, box_scale)
box:translate(-box_scale/2, -box_scale, 0)

lid = gr.cube('lid')
box:add_child(lid)
lid:set_material(brown)
box:set_texture("../Assets/fine_wood.png")
lid:scale(1, 1/box_scale*0.3, box_scale)
lid:rotate('x', 90)
lid:translate(0, box_scale*1.5, -0.15)

mirror_edge = 0.1

mirror = gr.plane('mirror')
lid:add_child(mirror)
mirror:set_material(mirror_mat)
mirror:rotate('x', -90)
mirror:translate(box_scale/4+0.065, 1.05, box_scale/4+0.05)
mirror:scale(0.9, 1, 0.9)

-- Handle
handle = gr.cylinder('handle')
box:add_child(handle)
handle:set_material(gold)
handle:scale(1/box_scale, 1/(box_scale*0.5), 1/box_scale)
handle:scale(0.05, .05, 0.2)
handle:rotate('y', 90)
handle:translate(1.05, 0.5, 0.5)

joint_1 = gr.joint('joint_1',{-30.0, 0.0, 40.0}, {-90.0, 0.0, 90.0})
handle:add_child(joint_1)
joint_1:rotate('y', -90)
joint_1:scale(1/0.05, 1/0.05, 1/0.2)



handle2 = gr.cylinder('handle2')
joint_1:add_child(handle2)
handle2:set_material(gold)
handle2:scale(0.03, 0.03, 0.5)
handle2:translate(0.08, 0, -0.25)

handle3 = gr.cylinder('handle3')
handle2:add_child(handle3)
handle3:set_material(gold)
handle3:rotate('y', 90)
handle3:scale(1/0.03, 1/0.03, 1/0.5)
handle3:scale(0.2, 0.03, 0.03)
handle3:translate(4, 0.3, -0.4)

-- Ballerina

platform = gr.sphere('platform')
box:add_child(platform)
platform:set_material(gold)
platform:scale(1/box_scale, 1/(box_scale*0.5), 1/box_scale)
platform:scale(0.9, 0.1, 0.9)
platform:translate(box_scale/4, box_scale/2 + 0.05, box_scale/4)

green = gr.material({0.0, 0.54, 0.0}, {0.9, 0.9, 0.9}, 25, 0)
fair = gr.material({0.953, 0.820, 0.725},{0, 0, 0}, 0, 0)
pink = gr.material({0.937, 0.647, 0.784}, {0, 0, 0}, 0, 0)

puppet_node = gr.node('puppet')
platform:add_child(puppet_node)
puppet_node:translate(0, box_scale+1.35, 0)
puppet_node:scale(1/0.9, 1/0.1, 1/0.9)
puppet_node:scale(0.3, 0.3, 0.3)
puppet_node:translate(0.1, 0, 0.3)



torso = gr.sphere('torso')
puppet_node:add_child(torso)
torso:set_material(pink)
t_x_scale = 0.5
t_y_scale = 1.0
t_z_scale = 0.4
torso:scale(t_x_scale, t_y_scale, t_z_scale)

shoulder = gr.sphere('shoulder')
torso:add_child(shoulder)
shoulder:set_material(pink)
shoulder:scale(1/t_x_scale, 1/t_y_scale, 1/t_z_scale)
sh_x_scale = 0.8
sh_y_scale = 0.15
sh_z_scale = 0.4
shoulder:scale(sh_x_scale, sh_y_scale, sh_z_scale)
shoulder:translate(0, 1 , 0)

-- Neck and Head

neck = gr.sphere('neck')
shoulder:add_child(neck)
neck:set_material(fair)
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
head:set_material(fair)
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
left_s_joint:rotate('x', 180)
left_s_joint:rotate('z', 40)
left_s_joint:scale(1/sh_x_scale, 1/sh_y_scale, 1/sh_z_scale)
left_s_joint:translate(0, 2.5, 0)

left_upper_arm = gr.sphere('left_upper_arm')
left_s_joint:add_child(left_upper_arm)
left_upper_arm:set_material(pink)
ua_x_scale = 0.14
ua_y_scale = 0.4
ua_z_scale = 0.2
left_upper_arm:scale(ua_x_scale, ua_y_scale, ua_z_scale)
left_upper_arm:translate(-0.65, -0.45, 0.0)

right_s_joint = gr.joint('right_s_joint', {-200.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
shoulder:add_child(right_s_joint)
right_s_joint:rotate('x', 180)
right_s_joint:rotate('z', -40)
right_s_joint:scale(1/sh_x_scale, 1/sh_y_scale, 1/sh_z_scale)
right_s_joint:translate(0, 2.5, 0)

right_upper_arm = gr.sphere('right_upper_arm')
right_s_joint:add_child(right_upper_arm)
right_upper_arm:set_material(pink)
right_upper_arm:scale(ua_x_scale, ua_y_scale, ua_z_scale)
right_upper_arm:translate(0.65, -0.45, 0.0)

left_elbow = gr.joint('left_elbow', {-100.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
left_upper_arm:add_child(left_elbow)
left_elbow:rotate('z', 75)
left_elbow:scale(1/ua_x_scale, 1/ua_y_scale, 1/ua_z_scale)
left_elbow:translate(0.0, -0.68, 0.0)

left_lower_arm = gr.sphere('left_lower_arm')
left_elbow:add_child(left_lower_arm)
left_lower_arm:set_material(fair)
la_x_scale = 0.14
la_y_scale = 0.3
la_z_scale = 0.2
left_lower_arm:scale(la_x_scale, la_y_scale, la_z_scale)
left_lower_arm:translate(0.0, -0.4, 0.0)

right_elbow = gr.joint('right_elbow', {-100.0, -50.0, 0.0}, {0.0, 0.0, 0.0})
right_upper_arm:add_child(right_elbow)
right_elbow:rotate('z', -75)
right_elbow:scale(1/ua_x_scale, 1/ua_y_scale, 1/ua_z_scale)
right_elbow:translate(0.0, -0.68, 0.0)

right_lower_arm = gr.sphere('right_lower_arm')
right_elbow:add_child(right_lower_arm)
right_lower_arm:set_material(fair)
right_lower_arm:scale(la_x_scale, la_y_scale, la_z_scale)
right_lower_arm:translate(0.0, -0.4, 0.0)

left_wrist = gr.joint('left_wrist', {-80.0, 0.0, 25.0}, {0.0, 0.0, 0.0})
left_lower_arm:add_child(left_wrist)
left_wrist:scale(1/la_x_scale, 1/la_y_scale, 1/la_z_scale)
left_wrist:translate(0.0, -0.8, 0.0)

left_hand = gr.sphere('left_hand')
left_wrist:add_child(left_hand)
left_hand:set_material(fair)
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
right_hand:set_material(fair)
right_hand:scale(h_x_scale, h_y_scale, h_z_scale)
right_hand:translate(0.0, -0.2, 0.0)

-- Lower Limbs

hips = gr.sphere('hips')
torso:add_child(hips)
hips:set_material(pink)
hips:scale(1/t_x_scale, 1/t_y_scale, 1/t_z_scale)
hp_x_scale = 1.2
hp_y_scale = 0.15
hp_z_scale = 1.2
hips:scale(hp_x_scale, hp_y_scale, hp_z_scale)
hips:translate(0, -0.85 , 0)

left_h_joint = gr.joint('left_h_joint', {-150.0, 0.0, 20.0}, {0.0, 0.0, 0.0})
hips:add_child(left_h_joint)
left_h_joint:rotate('x', -48)
left_h_joint:rotate('y', -90)
left_h_joint:rotate('z', -10)
left_h_joint:scale(1/hp_x_scale, 1/hp_y_scale, 1/hp_z_scale)
left_h_joint:translate(0, 0, 0.4)


left_upper_leg = gr.sphere('left_upper_leg')
left_h_joint:add_child(left_upper_leg)
left_upper_leg:set_material(fair)
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
right_upper_leg:set_material(fair)
right_upper_leg:scale(ul_x_scale, ul_y_scale, ul_z_scale)
right_upper_leg:translate(0.4, -0.55, 0.0)

left_knee = gr.joint('left_knee', {0.0, 0.0, 120.0}, {0.0, 0.0, 0.0})
left_upper_leg:add_child(left_knee)
left_knee:rotate('x', 100)
left_knee:scale(1/ul_x_scale, 1/ul_y_scale, 1/ul_z_scale)
left_knee:translate(0.0, -0.8, 0.0)

left_lower_leg = gr.sphere('left_lower_leg')
left_knee:add_child(left_lower_leg)
left_lower_leg:set_material(fair)
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
right_lower_leg:set_material(fair)
right_lower_leg:scale(ll_x_scale, ll_y_scale, ll_z_scale)
right_lower_leg:translate(0.0, -0.4, 0.0)

left_ankle = gr.joint('left_ankle', {-60.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
left_lower_leg:add_child(left_ankle)
left_ankle:rotate('x', 90)
left_ankle:scale(1/ll_x_scale, 1/ll_y_scale, 1/ll_z_scale)
left_ankle:translate(0.0, -1.4, 1.5)

left_foot = gr.sphere('left_foot')
left_ankle:add_child(left_foot)
left_foot:set_material(pink)
f_x_scale = 0.15
f_y_scale = 0.15
f_z_scale = 0.25
left_foot:scale(f_x_scale, f_y_scale, f_z_scale)
left_foot:translate(0.0, -0.3, 0.1)

right_ankle = gr.joint('right_ankle', {-60.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
right_lower_leg:add_child(right_ankle)
right_ankle:rotate('x', 90)
right_ankle:scale(1/ll_x_scale, 1/ll_y_scale, 1/ll_z_scale)
right_ankle:translate(0.0, -1.4, 1.5)

right_foot = gr.sphere('right_foot')
right_ankle:add_child(right_foot)
right_foot:set_material(pink)
right_foot:scale(f_x_scale, f_y_scale, f_z_scale)
right_foot:translate(0.0, -0.3, 0.1)


white_light = gr.light({5, 10, 10}, {0.9, 0.9, 0.9}, {1, 0, 0}, 8, 8)
scene:rotate('y', -20)
scene:rotate('x', 20)


num_renders = 40
degree = 360 / num_renders

-- Animation
for i=1,num_renders,1 do
    puppet_node:translate(-0.1, 0, -0.3)
    puppet_node:rotate('y', degree)
    puppet_node:translate(0.1, 0, 0.3)
    joint_1:rotate('z', degree)
    gr.render(scene, string.format("animation-%i.png", i), 500, 500, {-0.5, -1, 5}, {0, 0, -800}, {0, 1, 0}, 50, {0.3, 0.3, 0.3}, {white_light}, 8, 5, 5, 0, 32, 1, 0.1, 0)
end