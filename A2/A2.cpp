#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f))
    , current_state(O_ROTATE)
    , mouseLeftButtonActive(false)
    , mouseMiddleButtonActive(false)
    , mouseRightButtonActive(false)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}



//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();
    
    reset_view();
    
}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//----------------------------------------------------------------------------------------
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & v0,   // Line Start (NDC coordinate)
		const glm::vec2 & v1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = v0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = v1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

void A2::draw_item(vec3 *points, int num_points, mat4 transforms, vec3 colours)
{
    setLineColour(colours);
    vec4 p1;
    vec4 p2;
    // For every line of the item...
    for(int i=0; i < num_points; i += 2)
    {
        // Apple transformations to points 1 and 2
        p1 = transforms * vec4(points[i], 1.0f);
        p2 = transforms * vec4(points[i+1], 1.0f);
        
        // Clip line (p1 <-> p2)
        bool was_clipped = clip_line(p1, p2);
        
        if (!was_clipped){
            // Normalize (NDC)
            vec2 p1_n = vec2(p1[0]/p1[3], p1[1]/p1[3]);
            vec2 p2_n = vec2(p2[0]/p2[3], p2[1]/p2[3]);
            
            // Map to Viewport (NDC -> Screen)
            map_to_viewport(p1_n);
            map_to_viewport(p2_n);
            
            // Draw Line
            drawLine(p1_n, p2_n);
        }
    }
}

bool check_OR_zeros(int* code1, int* code2, int len){
    for(int i=0; i < len; i++){
        if (code1[i] == 1 || code2[i] == 1)
        {
            // OR operation will result in 1, thus not all zeros
            return false;
        }
    }
    return true;
}

bool check_AND_zeros(int* code1, int* code2, int len){
    for(int i=0; i < len; i++){
        if (code1[i] == 1 && code2[i] == 1)
        {
            // AND operation will result in 1, thus not all zeros
            return false;
        }
    }
    return true;
}

bool A2::clip_line(vec4 &p1, vec4 &p2){

    
    // Should the line p1 <-> p2 be included ?
    
    // Define the planes we are using to clip
    // AKA the x,y,z values for boundry codes
    float P1[6] = { p1[0], -p1[0],
                    p1[1], -p1[1],
                    p1[2], -p1[2] };
    float P2[6] = { p2[0], -p2[0],
                    p2[1], -p2[1],
                    p2[2], -p2[2] };
    
    float p1_w = p1[3];
    float p2_w = p2[3];
    
    int p1_codes[6] ;
    int p2_codes[6] ;
    
    // For each plane, test to see if points are within the bounds
    // Use boundry codes
    for (int i = 0; i < 6; i++){
        // check p1
        if (p1_w + P1[i] < 0){
            p1_codes[i] = 1;
        } else {
            p1_codes[i] = 0;
        }
        
        // check p2
        if (p2_w + P2[i] < 0){
            p2_codes[i] = 1;
        } else {
            p2_codes[i] = 0;
        }
    }
    
    // Check if trivially accept / reject
    if (check_OR_zeros(p1_codes, p2_codes, 6)){
        // All zeros, which means we can trivially accept
        return false;
    }
    
    if (!check_AND_zeros(p1_codes, p2_codes, 6)){
        // NOT all zeros, which means we can trivially reject
        return true;
    }
    
    // Now, need to go through each boundry and find where it crosses the boundry line
    // p1 OR p2 could be the point that is outside the boundry line
    
    for (int i=0; i < 6; i++){
        
        // Check each boundry
        // If one point is outside the boundry, we
        // 1) need to find the intersection point
        // 2) update the value of the outside point
        if (p1_codes[i] == 1 || p2_codes[i] == 1){
            
            //cout << "Clipping p1 before: " << p1 << endl;
            //cout << "Cliped p2 before: " << p2 << endl;
            
            // Intersection happens at point a = (w_1 + x_1)/((w_1 + x_1) - (w_2 + x_2))
            float a = (p1_w + P1[i]) / ((p1_w + P1[i]) - (p2_w + P2[i]));
            
            // Now, find new point for either p1 or p2
            if (p1_codes[i] == 1)
            {
                p1 = p1+a*(p2 - p1);
                
            } else {
                p2 = p1+a*(p2 - p1);
            }
        }
        
    }
    
    // Only time we return true is if the entire line is outside the clip
    // For lines crossing a boundry, we update the points such that it the line is only the clipped data.
    return false;
}

void A2::map_to_viewport(vec2& p){
    
    // We need to take the coords of our two points and map them so that they will fit into our viewport window
    // Steps:
    // 1) Convert point x_w to a distance from the window corner
    // 2) Scale this w distance to get a v distance (using ratio)
    // 3) Add to the viewport corner to get x_v
    
    // Need:
    // window width
    // window height
    // viewport width
    // viewport height
    
    float w_width = m_windowWidth;
    float w_height = m_windowHeight;
    float v_width = current_viewport[0];
    float v_height = current_viewport[1];
    float v_corner_x = current_viewport[2];
    float v_corner_y = current_viewport[3];
    float width_ratio = v_width / w_width ;
    float height_ratio = v_height / w_height ;
    float v_corner_x_w = (2 * v_corner_x) / w_width - 1 ;
    float v_corner_y_w = (2 * v_corner_y) / w_height - 1 ;
    
    // Step 1) Convert point x_w to a distance from the window corner
    float x_w = p[0] + 1;
    float y_w = p[1] + 1;
    
    // Step 2) Scale to get x_v
    float x_v = x_w * width_ratio ;
    float y_v = y_w * height_ratio ;
    
    // Step 3) Add to viewport corner
    p[0] = x_v + v_corner_x_w;
    p[1] = y_v + v_corner_y_w;
}



void A2::reset_view()
{
    // Reset the view back to default settings
    current_state = O_ROTATE;
    current_fov_a = 30.0f;
    current_near = 1.0f;
    current_far = 20.0f;
    current_aspect = 1.0f;
    
    current_model_rotation[0] = 0.0f;
    current_model_rotation[1] = 0.0f;
    current_model_rotation[2] = 0.0f;
    current_model_translation[0] = 0.0f;
    current_model_translation[1] = 0.0f;
    current_model_translation[2] = 0.0f;
    current_model_scale[0] = 1.0f;
    current_model_scale[1] = 1.0f;
    current_model_scale[2] = 1.0f;
    
    current_view_rotation[0] = 0.0f;
    current_view_rotation[1] = 0.0f;
    current_view_rotation[2] = 0.0f;
    current_view_translation[0] = 0.0f;
    current_view_translation[1] = 0.0f;
    current_view_translation[2] = 0.0f;
    
    current_perspective[0] = current_fov_a;
    current_perspective[1] = current_near;
    current_perspective[2] = current_far;
    
    // Viewport width - init 5% margin of window size on both sides
    // Thus 90% of the window width
    current_viewport[0] = (m_windowWidth * 0.9f) ;
    // Viewport height - init 5% margin of window size on both side
    // Thus 90% of the window height
    current_viewport[1] = (m_windowHeight * 0.9f);
    // Viewport left bottom corner x - init 5% margin from window
    current_viewport[2] = (m_windowWidth * 0.05f);
    // Viewport left bottom corner y - init 5% margin
    current_viewport[3] = (m_windowHeight * 0.05f);
    
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();
    
    // Draw Viewport
    vec3 viewport[8] = {
        // Bottom Left Corner -> Top Left Corner
        vec3(-1,-1,1), vec3(-1,1,1),
        // Top Left Corner -> Top Right Corner
        vec3(-1,1,1), vec3(1,1,1),
        // Top Right Corner -> Bottom Right Corner
        vec3(1,1,1), vec3(1,-1,1),
        // Bottom Right Corner -> Bottom Left Corner
        vec3(1,-1,1), vec3(-1,-1,1)
    };
    
    // Need to take cube verts in 3D and transform to 2D
    // - Morph by Model Matrix
    // - Morph by View Matrix
    // - Morph by Projection Matrix
    // p' = P*V*M*p
    // - then clip
    // - normalize
    // - viewport
    
    // Model Changes
    
    model_mat = translate(vec3(current_model_translation[0],current_model_translation[1],current_model_translation[2])) * rotate_x(current_model_rotation[0]) * rotate_y(current_model_rotation[1]) * rotate_z(current_model_rotation[2]) * scale(vec3(current_model_scale[0],current_model_scale[1],current_model_scale[2]));
    
    mat4 gnomon_model_mat = translate(vec3(current_model_translation[0],current_model_translation[1],current_model_translation[2])) * rotate_x(current_model_rotation[0]) * rotate_y(current_model_rotation[1]) * rotate_z(current_model_rotation[2]);
    
    // View Changes
    
    // Default Eye position
    vec3 view_pos = vec3(0.0f, 0.0f, -15.0f);
    
    view_mat = translate(vec3(current_view_translation[0],current_view_translation[1],current_view_translation[2])) * rotate_x(current_view_rotation[0]) * rotate_y(current_view_rotation[1]) * rotate_z(current_view_rotation[2]) * translate(view_pos);
    
    // Projection Changes
    projection_mat = perspective((double)current_perspective[0], (double)current_aspect, (double)current_perspective[2], (double)current_perspective[1]);
    
    // Draw cube (white lines)
    draw_item(cube, 24, projection_mat * view_mat * model_mat, vec3(1.0f,1.0f,1.0f));
    
    // Draw model gnomon
    draw_item(gnomon_X, 2, projection_mat * view_mat * gnomon_model_mat, vec3(1.0f,0.0f,0.0f));
    draw_item(gnomon_Y, 2, projection_mat * view_mat * gnomon_model_mat, vec3(1.0f,1.0f,0.0f));
    draw_item(gnomon_Z, 2, projection_mat * view_mat * gnomon_model_mat, vec3(0.0f,1.0f,1.0f));
    
    // Draw world gnomon
    draw_item(gnomon_X, 2, projection_mat * view_mat, vec3(1.0f,0.0f,0.5f));
    draw_item(gnomon_Y, 2, projection_mat * view_mat, vec3(0.0f,1.0f,0.5f));
    draw_item(gnomon_Z, 2, projection_mat * view_mat, vec3(0.5f,0.5f,1.0f));
    
    // Draw viewport border
    draw_item(viewport, 8, mat4(), vec3(1.0,1.0,1.0));
    
    
    
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
    
        if( ImGui::Button( "Reset" )) {
            reset_view();
        }
    
        ImGui::RadioButton("Rotate View", &current_state, O_ROTATE );
        ImGui::RadioButton("Translate View", &current_state, N_TRANSLATE );
        ImGui::RadioButton("Perspective", &current_state, P_PERSPECTIVE);
        ImGui::RadioButton("Rotate Model", &current_state, R_ROTATE);
        ImGui::RadioButton("Translate Model", &current_state, T_TRANSLATE);
        ImGui::RadioButton("Scale Model", &current_state, S_SCALE);
        ImGui::RadioButton("Viewport", &current_state, V_VIEW);

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
        ImGui::Text("Far: %.1f ", current_perspective[2]);
        ImGui::Text("Near: %.1f ", current_perspective[1]);

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);
    
    
    
    

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Translate 3D
 */
mat4 A2::translate(vec3 const& v)
{
    mat4 matrix;
    
    matrix[0] = vec4(1,0,0,0);
    matrix[1] = vec4(0,1,0,0);
    matrix[2] = vec4(0,0,1,0);
    matrix[3] = vec4(v[0],v[1],v[2],1);
    
    return matrix;
}


//----------------------------------------------------------------------------------------
/*
 * Scale 3D
 */
mat4 A2::scale(vec3 const& s)
{
    mat4 matrix;
    
    matrix[0] = vec4(s[0],0,0,0);
    matrix[1] = vec4(0,s[1],0,0);
    matrix[2] = vec4(0,0,s[2],0);
    matrix[3] = vec4(0,0,0,1);
    
    return matrix;
}

//----------------------------------------------------------------------------------------
/*
 * Rotate About Z 3D
 */
mat4 A2::rotate_z(float r)
{
    mat4 matrix;
    
    matrix[0] = vec4(cos(r),sin(r),0,0);
    matrix[1] = vec4(-sin(r),cos(r),0,0);
    matrix[2] = vec4(0,0,1,0);
    matrix[3] = vec4(0,0,0,1);
    
    return matrix;
}

//----------------------------------------------------------------------------------------
/*
 * Rotate About X 3D
 */
mat4 A2::rotate_x(float r)
{
    glm::mat4 matrix;
    
    matrix[0] = vec4(1,0,0,0);
    matrix[1] = vec4(0,cos(r),sin(r),0);
    matrix[2] = vec4(0,-sin(r),cos(r),0);
    matrix[3] = vec4(0,0,0,1);
    
    return matrix;
}

//----------------------------------------------------------------------------------------
/*
 * Rotate About Y 3D
 */
mat4 A2::rotate_y(float r)
{
    mat4 matrix;
    
    matrix[0] = vec4(cos(r),0,-sin(r),0);
    matrix[1] = vec4(0,1,0,0);
    matrix[2] = vec4(sin(r),0,cos(r),0);
    matrix[3] = vec4(0,0,0,1);
    
    return matrix;
}

float cot(float angle)
{
    return 1/tan(angle);
}

//----------------------------------------------------------------------------------------
/*
 * Perspective 3D
 */
mat4 A2::perspective(double a, double aspect, double far, double near)
{
    mat4 matrix;
    
    matrix[0] = vec4((cot(radians(a)/2.0f)/aspect), 0, 0, 0);
    matrix[1] = vec4(0, cot(radians(a)/2.0f), 0, 0);
    matrix[2] = vec4(0, 0, (-1.0f*((far + near)/(far - near))), -1.0f);
    matrix[3] = vec4(0, 0, ((-2.0f*far*near)/(far - near)), 0);
    
    return matrix;
}



//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);
    
    // If the mouse leaves the window while dragging a viewport, clamp at the
    if (entered == 0 && current_state == V_VIEW){
        mouseLeftButtonActive = false;
    }

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
    bool eventHandled(false);
    
    if (!ImGui::IsMouseHoveringAnyWindow()) {
    
        switch (current_state) {
                
            case O_ROTATE:
            {
                float diff_x = prev_x - xPos;
                
                if (mouseRightButtonActive) {
                    // Rotate cube on z axis
                    current_view_rotation[2] += - diff_x / 360;
                }
                if (mouseMiddleButtonActive) {
                    // Rotate cube on y axis
                    current_view_rotation[1] += - diff_x / 360;
                }
                if (mouseLeftButtonActive){
                    // Rotate cube on x axis
                    current_view_rotation[0] += - diff_x / 360;
                    
                }
                
                if (mouseLeftButtonActive || mouseMiddleButtonActive || mouseRightButtonActive){
                    //Update the previous x position to the current x value
                    prev_x = xPos;
                }
                
                break;
            }
                
            case N_TRANSLATE:
            {
                float diff_x = prev_x - xPos;
                
                if (mouseRightButtonActive) {
                    // Translate cube on z axis
                    current_view_translation[2] += - diff_x / 100;
                }
                if (mouseMiddleButtonActive) {
                    // Translate cube on y axis
                    current_view_translation[1] += - diff_x / 100;
                }
                if (mouseLeftButtonActive){
                    // Translate cube on x axis
                    current_view_translation[0] += - diff_x / 100;
                    
                }
                
                if (mouseLeftButtonActive || mouseMiddleButtonActive || mouseRightButtonActive){
                    //Update the previous x position to the current x value
                    prev_x = xPos;
                }
                
            }
                
            case P_PERSPECTIVE:
            {
                float diff_x = prev_x - xPos;
                
                if (mouseRightButtonActive) {
                    // Change far value
                    current_perspective[2] += - diff_x / 100;
                }
                if (mouseMiddleButtonActive) {
                    // Change near value
                    current_perspective[1] += - diff_x / 100;
                }
                if (mouseLeftButtonActive){
                    
                    
                    // Change FOV angle value (5 <= a <= 160)
                    
                    if ((current_perspective[0] + -diff_x/ 10 >= 5) &&
                        (current_perspective[0] + -diff_x/ 10 <= 160))
                    {
                        current_perspective[0] += -diff_x / 10;
                    }
                    
                }
                
                if (mouseLeftButtonActive || mouseMiddleButtonActive || mouseRightButtonActive){
                    //Update the previous x position to the current x value
                    prev_x = xPos;
                }
                
            }
            
            case R_ROTATE:
            {
                float diff_x = prev_x - xPos;
                
                if (mouseRightButtonActive) {
                    // Rotate cube on z axis
                    current_model_rotation[2] += - diff_x / 100;
                }
                if (mouseMiddleButtonActive) {
                    // Rotate cube on y axis
                    current_model_rotation[1] += - diff_x / 100;
                }
                if (mouseLeftButtonActive){
                    // Rotate cube on x axis
                    current_model_rotation[0] += - diff_x / 100;

                }
                
                if (mouseLeftButtonActive || mouseMiddleButtonActive || mouseRightButtonActive){
                    //Update the previous x position to the current x value
                    prev_x = xPos;
                }
                
                break;
            }
                
            case T_TRANSLATE:
            {
                float diff_x = prev_x - xPos;
                
                if (mouseRightButtonActive) {
                    // Rotate cube on z axis
                    current_model_translation[2] += - diff_x / 100;
                }
                if (mouseMiddleButtonActive) {
                    // Rotate cube on y axis
                    current_model_translation[1] += - diff_x / 100;
                }
                if (mouseLeftButtonActive){
                    // Rotate cube on x axis
                    current_model_translation[0] += - diff_x / 100;
                    
                }
                
                if (mouseLeftButtonActive || mouseMiddleButtonActive || mouseRightButtonActive){
                    //Update the previous x position to the current x value
                    prev_x = xPos;
                }
                
                break;
            }
            case S_SCALE:
            {
                float diff_x = prev_x - xPos;
                
                if (mouseRightButtonActive) {
                    // Rotate cube on z axis
                    if ((current_model_scale[2] + -diff_x / 100) >= 0){
                        current_model_scale[2] += - diff_x / 100;
                    }
                }
                if (mouseMiddleButtonActive) {
                    // Rotate cube on y axis
                    if ((current_model_scale[1] + -diff_x / 100) >= 0){
                        current_model_scale[1] += - diff_x / 100;
                    }
                }
                if (mouseLeftButtonActive){
                    // Rotate cube on x axis
                    if ((current_model_scale[0] + -diff_x / 100) >= 0){
                        current_model_scale[0] += - diff_x / 100;
                    }
                }
                
                if (mouseLeftButtonActive || mouseMiddleButtonActive || mouseRightButtonActive){
                    //Update the previous x position to the current x value
                    prev_x = xPos;
                }
                
                break;
            }
                
            case V_VIEW:
            {
                if (mouseLeftButtonActive){
                    
                    // Get the difference in the x & y coords
                    float diff_x = xPos - prev_x;
                    float diff_y = (m_windowHeight - yPos) - prev_y;
                    
                    // If the click was the bottom left viewport corner, then the diff in x and y will both be positive
                    if (diff_x >= 0 && diff_y >= 0){
                        current_viewport[0] += diff_x;
                        current_viewport[1] += diff_y;
                    } else if (diff_x >= 0 && diff_y < 0){
                        // Click was the top left corner dragging down - diff x pos and diff y neg
                        // So the x value of the origin stays the same
                        // But the y value has to change
                        // Both height and width are still changing
                        current_viewport[0] += diff_x;
                        current_viewport[1] += (-1)*diff_y;
                        current_viewport[3] = m_windowHeight - yPos;
                    } else if (diff_x < 0 && diff_y >= 0){
                        // Click was the bottom right corner - diff x neg and diff y pos
                        // x value or the origin changes while y stays the same
                        // height and width still increase
                        current_viewport[0] += (-1)*diff_x;
                        current_viewport[1] += diff_y;
                        current_viewport[2] = xPos;
                    } else {
                        // Click was the top right corner - both diffs are negative
                        // everything changes
                        current_viewport[0] += (-1)*diff_x;
                        current_viewport[1] += (-1)*diff_y;
                        current_viewport[2] = xPos;
                        current_viewport[3] = m_windowHeight - yPos;
                    }
                    
                    prev_x = xPos;
                    prev_y = m_windowHeight - yPos;
                    
                    
                }
                break;
            }
                
            
            default:
                break;
        }
        
    }
    return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods) {

    bool eventHandled(false);
    
    // If the button is pressed...
    if (actions == GLFW_PRESS) {
        if (!ImGui::IsMouseHoveringAnyWindow()) {
            
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                cout << "left mouse" << endl;
                // Indicate the button is being held
                mouseLeftButtonActive = true;
                // Get the x and y value and update previous x/y variables
                glfwGetCursorPos(m_window, &prev_x, &prev_y);
                
                if (current_state == V_VIEW){
                    
                    prev_y = m_windowHeight - prev_y;
                    current_viewport[2] = prev_x;
                    current_viewport[3] = prev_y;
                    current_viewport[0] = 0;
                    current_viewport[1] = 0;
                    // w, h, x, y
                }
            
            }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                mouseMiddleButtonActive = true;
                // Get the x and y value and update previous x/y variables
                glfwGetCursorPos(m_window, &prev_x, &prev_y);
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                cout << "right mouse" << endl;
                mouseRightButtonActive = true;
                // Get the x and y value and update previous x/y variables
                glfwGetCursorPos(m_window, &prev_x, &prev_y);
            }
        }
    }
    
    // Once the button is released, indicate the button is no longer being held
    if (actions == GLFW_RELEASE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            // Indicate the button is being held
            mouseLeftButtonActive = false;
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            mouseMiddleButtonActive = false;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            mouseRightButtonActive = false;
        }
    }
    
    
    return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

    // Fill in with event handling code...
    if( action == GLFW_PRESS ) {
        // Respond to some key events.
        if (key == GLFW_KEY_Q) {
            // Quit Application
            cout << "Q key pressed" << endl;
            glfwSetWindowShouldClose(m_window, GL_TRUE);
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_A){
            // Reset to default values
            cout << "A key pressed... resetting..." << endl;
            reset_view();
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_O){
            // Reset to default values
            cout << "O key pressed" << endl;
            current_state = O_ROTATE;
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_N){
            // Reset to default values
            cout << "N key pressed" << endl;
            current_state = N_TRANSLATE;
            
            eventHandled = true;
        }
        if (key == GLFW_KEY_P){
            // Reset to default values
            cout << "P key pressed" << endl;
            current_state = P_PERSPECTIVE;
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_R){
            // Reset to default values
            cout << "R key pressed" << endl;
            current_state = R_ROTATE;
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_T){
            // Reset to default values
            cout << "T key pressed" << endl;
            current_state = T_TRANSLATE;
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_S){
            // Reset to default values
            cout << "S key pressed" << endl;
            current_state = S_SCALE;
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_V){
            // Reset to default values
            cout << "V key pressed" << endl;
            current_state = V_VIEW;
            
            eventHandled = true;
        }
    }
	return eventHandled;
}
