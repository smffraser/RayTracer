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
    /*, current_view_rotation{0.0f,0.0f,0.0f}
    , current_view_translation(vec3(0,0,0))
    , current_fov_a(0.0f)
    , current_near(0.0f)
    , current_far(0.0f)
    , current_aspect(1.0f)
    , current_model_rotation{0.0f,0.0f,0.0f}
    , current_model_translation(vec3(0,0,0))
    , current_model_scale{0.0f,0.0f,0.0f}*/
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
	m_vertexData.numVertices = 4;
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
        
        // Normalize (NDC)
        
        // Map to Viewport (NDC -> Screen)
        
        // Draw Line
        drawLine(vec2(p1[0]/p1[3], p1[1]/p1[3]), vec2(p2[0]/p2[3], p2[1]/p2[3]));
        
    }
}

void A2::reset_view()
{
    // Reset the view back to default settings
    current_state = O_ROTATE;
    /*current_fov_a = 30.0f;
    current_near = 1.0f;
    current_far = 10.f;
    current_aspect = 1.0f;
    */
    current_model_rotation[0] = 0.0f;
    current_model_rotation[1] = 0.0f;
    current_model_rotation[2] = 0.0f;
    current_model_translation[0] = 0.0f;
    current_model_translation[1] = 0.0f;
    current_model_translation[2] = 0.0f;
    current_model_scale[0] = 1.0f;
    current_model_scale[1] = 1.0f;
    current_model_scale[2] = 1.0f;
    
    // Set Model Matrix back to identity
    model_mat = glm::mat4();
    //model_mat = rotate_x(radians(90.0f));
    //model_mat = glm::rotate(model_mat, radians(90.0f), vec3(1,0,0));
    
    // Set View Matrix to Default View Pos
    vec3 view_pos = vec3(0.0f, 0.0f, -5.0f);
    view_mat = glm::mat4() * translate(view_pos);
    //view_mat = glm::translate(view_mat,vec3(0.0f, 0.0f, 5.0f));
    
    // Set Perspective Matrix to Default
    projection_mat = perspective(30.0, 1.0, 10.0, 1.0);
    //projection_mat = glm::perspective(30.0, 1.0, 1.0, 10.0);

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
    
    // Need to take cube verts in 3D and transform to 2D
    // - Morph by Model Matrix
    // - Morph by View Matrix
    // - Morph by Projection Matrix
    // p' = P*V*M*p
    // - then clip
    // - normalize
    // - viewport
    
    model_mat = translate(vec3(current_model_translation[0],current_model_translation[1],current_model_translation[2])) * rotate_x(current_model_rotation[0]) * rotate_y(current_model_rotation[1]) * rotate_z(current_model_rotation[2]) * scale(vec3(current_model_scale[0],current_model_scale[1],current_model_scale[2]));
    
    mat4 gnomon_model_mat = translate(vec3(current_model_translation[0],current_model_translation[1],current_model_translation[2])) * rotate_x(current_model_rotation[0]) * rotate_y(current_model_rotation[1]) * rotate_z(current_model_rotation[2]);
    
    // Draw cube (white lines)
    draw_item(cube, 24.0f, projection_mat * view_mat * model_mat, vec3(1.0f,1.0f,1.0f));
    
    // Draw model gnomon
    draw_item(gnomon_X, 2.0f, projection_mat * view_mat * gnomon_model_mat, vec3(1.0f,0.0f,0.0f));
    draw_item(gnomon_Y, 2.0f, projection_mat * view_mat * gnomon_model_mat, vec3(0.0f,1.0f,0.0f));
    draw_item(gnomon_Z, 2.0f, projection_mat * view_mat * gnomon_model_mat, vec3(0.0f,0.0f,1.0f));
    
    // Draw world gnomon
    draw_item(gnomon_X, 2.0f, projection_mat * view_mat, vec3(1.0f,0.5f,0.5f));
    draw_item(gnomon_Y, 2.0f, projection_mat * view_mat, vec3(0.5f,1.0f,0.5f));
    draw_item(gnomon_Z, 2.0f, projection_mat * view_mat, vec3(0.5f,0.5f,1.0f));
    
    
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
    
    
    //matrix[0] = vec4((cot(rad_a/2.0f)/aspect), 0, 0, 0);
    //matrix[1] = vec4(0, cot(rad_a/2.0f), 0, 0);
    //matrix[2] = vec4(0, 0, (near+far)/near, -far);
    //matrix[3] = vec4(0, 0, 1/near, 0);
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

	// Fill in with event handling code...

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
