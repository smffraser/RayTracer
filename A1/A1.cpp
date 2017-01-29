#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

static const size_t DIM = 16;

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col( 0 ), current_grid(Grid(DIM)), active_cell( 0 ), m_shape_rotation( 0 ), m_shape_size( 1 )
{
    colour = new float*[8];
    for (int x = 0; x < 8; x++) {
        colour[x] = new float[3];
    }
    
    // Colour 1
	colour[0][0] = 0.0f;
	colour[0][1] = 0.0f;
	colour[0][2] = 0.0f;
    
    // Colour 2
    colour[1][0] = 0.0f;
    colour[1][1] = 0.0f;
    colour[1][2] = 1.0f;
    
    // Colour 3
    colour[2][0] = 0.0f;
    colour[2][1] = 1.0f;
    colour[2][2] = 0.0f;
    
    // Colour 4
    colour[3][0] = 1.0f;
    colour[3][1] = 0.0f;
    colour[3][2] = 0.0f;
    
    // Colour 5
    colour[4][0] = 1.0f;
    colour[4][1] = 1.0f;
    colour[4][2] = 0.0f;
    
    // Colour 6
    colour[5][0] = 0.0f;
    colour[5][1]= 1.0f;
    colour[5][2]= 1.0f;
    
    // Colour 7
    colour[6][0] = 1.0f;
    colour[6][1] = 0.0f;
    colour[6][2] = 1.0f;
    
    // Colour 8
    colour[7][0] = 1.0f;
    colour[7][1] = 1.0f;
    colour[7][2] = 1.0f;
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{
    for (int x = 0; x < 8 ; x++) {
        delete[] colour[x];
    }
    
    delete[] colour;

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
		getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

    // Set up the grid, cube, and active cell objects
	initGrid();
    initCube();
    initSquare();

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt( 
		glm::vec3( 0.0f, float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	proj = glm::perspective( 
		glm::radians( 45.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );
}

void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
    // x,y,z values of the vertices
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}

// init cube instance
void A1::initCube()
{
    // Cube vertices
    size_t sz = 3 * 3 * 2 * 6;
    
    // x,y,z values of the vertices
    float verts[] = {
        0,0,0, // first triangle : begin // first square : begin
        1,0,0,
        0,0,1, // first triangle : end
        1,0,1, // second triangle : begin
        1,0,0,
        0,0,1,// second triangle : end // first square : end
        0,1,0,
        1,1,0,
        0,1,1,
        1,1,1,
        1,1,0,
        0,1,1,
        0,0,0,
        0,1,0,
        0,0,1,
        0,1,1,
        0,1,0,
        0,0,1,
        1,0,0,
        1,1,0,
        0,0,0,
        0,1,0,
        1,1,0,
        0,0,0,
        1,0,1,
        1,1,1,
        1,0,0,
        1,1,0,
        1,1,1,
        1,0,0,
        1,0,1,
        1,1,1,
        0,0,1,
        0,1,1,
        1,1,1,
        0,0,1
    };
    
    // Create the vertex array to record buffer assignments.
    glGenVertexArrays(1, &m_cube_vao);
    glBindVertexArray(m_cube_vao);
    
    // Create the cube vertex buffer
    glGenBuffers(1, &m_cube_vbo);
    glBindBuffer( GL_ARRAY_BUFFER, m_cube_vbo );
    glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
                 verts, GL_STATIC_DRAW );
    
    // Specify the means of extracting the position values properly.
    GLint posAttrib = m_shader.getAttribLocation( "position" );
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    
    // Reset state to prevent rogue code from messing with *my*
    // stuff!
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    
    CHECK_GL_ERRORS;
    
}

// init cube instance
void A1::initSquare()
{
    // Cube vertices
    size_t sz = 14 * 3;
    
    // x,y,z values of the vertices
    float verts[] = {
        0,0,0,
        0,0,1,
        0,0,1,
        1,0,1,
        1,0,1,
        1,0,0,
        1,0,0,
        0,0,0, // End of Lines
        0,0,0, // first triangle : begin
        1,0,0,
        0,0,1, // first triangle : end
        1,0,1, // second triangle : begin
        1,0,0,
        0,0,1
    };
    
    // Create the vertex array to record buffer assignments.
    glGenVertexArrays(1, &m_square_vao);
    glBindVertexArray(m_square_vao);
    
    // Create the cube vertex buffer
    glGenBuffers(1, &m_square_vbo);
    glBindBuffer( GL_ARRAY_BUFFER, m_square_vbo );
    glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
                 verts, GL_STATIC_DRAW );
    
    // Specify the means of extracting the position values properly.
    GLint posAttrib = m_shader.getAttribLocation( "position" );
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    
    // Reset state to prevent rogue code from messing with *my*
    // stuff!
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    
    CHECK_GL_ERRORS;
    
}

//----------------------------------------------------------------------------------------
/*
 * Resets the program to default settings.
 */
void A1::reset(){
    // Reset to default values
    current_grid.reset();
    m_shape_size = 1;
    m_shape_rotation = 0;
    active_cell = 0;
    current_col = 0;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for 
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
    
    
        if (ImGui::Button("Reset")){
            reset();
        }

        // 8 default colours
        // These colours can be changed by the user using the sliders.

        // Colour 1
		ImGui::PushID( 0 );
		ImGui::ColorEdit3( "##Colour", colour[0] );
		ImGui::SameLine();
		if( ImGui::RadioButton( "##Col", &current_col, 0 ) ) {
			// Select this colour.
            // If there exists a bar on the active cell, change its colour
            // to the newly selected current colour
            if (current_grid.getHeight(active_cell) > 0){
                current_grid.setColour(active_cell, 0);
            }
		}
		ImGui::PopID();
    
        // Colour 2
        ImGui::PushID( 1 );
        ImGui::ColorEdit3( "##Colour", colour[1] );
        ImGui::SameLine();
        if( ImGui::RadioButton( "##Col", &current_col, 1 ) ) {
            // Select this colour.
            // If there exists a bar on the active cell, change its colour
            // to the newly selected current colour
            if (current_grid.getHeight(active_cell) > 0){
                current_grid.setColour(active_cell, 1);
            }
        }
        ImGui::PopID();
    
        // Colour 3
        ImGui::PushID( 2 );
        ImGui::ColorEdit3( "##Colour", colour[2] );
        ImGui::SameLine();
        if( ImGui::RadioButton( "##Col", &current_col, 2 ) ) {
            // Select this colour.
            // If there exists a bar on the active cell, change its colour
            // to the newly selected current colour
            if (current_grid.getHeight(active_cell) > 0){
                current_grid.setColour(active_cell, 2);
            }
        }
        ImGui::PopID();
    
        // Colour 4
        ImGui::PushID( 3 );
        ImGui::ColorEdit3( "##Colour", colour[3] );
        ImGui::SameLine();
        if( ImGui::RadioButton( "##Col", &current_col, 3 ) ) {
            // Select this colour.
            // If there exists a bar on the active cell, change its colour
            // to the newly selected current colour
            if (current_grid.getHeight(active_cell) > 0){
                current_grid.setColour(active_cell, 3);
            }
        }
        ImGui::PopID();
    
        // Colour 5
        ImGui::PushID( 4 );
        ImGui::ColorEdit3( "##Colour", colour[4] );
        ImGui::SameLine();
        if( ImGui::RadioButton( "##Col", &current_col, 4 ) ) {
            // Select this colour.
            // If there exists a bar on the active cell, change its colour
            // to the newly selected current colour
            if (current_grid.getHeight(active_cell) > 0){
                current_grid.setColour(active_cell, 4);
            }
        }
        ImGui::PopID();
    
        // Colour 6
        ImGui::PushID( 5 );
        ImGui::ColorEdit3( "##Colour", colour[5] );
        ImGui::SameLine();
        if( ImGui::RadioButton( "##Col", &current_col, 5 ) ) {
            // Select this colour.
            // If there exists a bar on the active cell, change its colour
            // to the newly selected current colour
            if (current_grid.getHeight(active_cell) > 0){
                current_grid.setColour(active_cell, 5);
            }
        }
        ImGui::PopID();
    
        // Colour 7
        ImGui::PushID( 6 );
        ImGui::ColorEdit3( "##Colour", colour[6] );
        ImGui::SameLine();
        if( ImGui::RadioButton( "##Col", &current_col, 6 ) ) {
            // Select this colour.
            // If there exists a bar on the active cell, change its colour
            // to the newly selected current colour
            if (current_grid.getHeight(active_cell) > 0){
                current_grid.setColour(active_cell, 6);
            }
        }
        ImGui::PopID();
    
        // Colour 8
        ImGui::PushID( 7 );
        ImGui::ColorEdit3( "##Colour", colour[7] );
        ImGui::SameLine();
        if( ImGui::RadioButton( "##Col", &current_col, 7 ) ) {
            // Select this colour.
            // If there exists a bar on the active cell, change its colour
            // to the newly selected current colour
            if (current_grid.getHeight(active_cell) > 0){
                current_grid.setColour(active_cell, 7);
            }
        }
        ImGui::PopID();
    

/*
		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in 
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}
*/

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
	W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );
    W = glm::rotate(mat4(), m_shape_rotation, vec3(0,1,0))*glm::scale(mat4(), vec3(m_shape_size))*W;

	m_shader.enable();
    
		glEnable( GL_DEPTH_TEST );

		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );
    
    

		// Just draw the grid for now.
		glBindVertexArray( m_grid_vao );
		glUniform3f( col_uni, 1, 1, 1 );
		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );

    
		// Draw the cubes
        glBindVertexArray( m_cube_vao );
        mat4 m_trans;
    
        // For each cell in the grid...
        for (int row = 0; row < DIM; row++){
            for (int col = 0; col < DIM; col++ ){
                
                int cell_index = DIM * row + col;
                
                // If the current cell has > 0 cubes on it...
                if (current_grid.getHeight(cell_index) > 0){
                    
                    int cubes_col = current_grid.getColour(cell_index);
                    
                    // Get the current cell's cube colour
                    glUniform3f(col_uni, colour[cubes_col][0], colour[cubes_col][1], colour[cubes_col][2]);
                    
                    // Draw the first cube
                    glDrawArrays( GL_TRIANGLES, 0, 3 * 2 * 6 );
                    
                    // If the grid has a stack of cubes...
                    for (int up = 1; up < current_grid.getHeight(cell_index); up++){
                        // Translate UP one unit
                        m_trans = glm::translate(m_trans, vec3(0,1,0));
                        glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*m_trans));
                        
                        // Draw cube
                        glDrawArrays( GL_TRIANGLES, 0, 36 );
                    }
                    // Reverse translates to get to the y = 0 grid height
                    for (int down = 1; down < current_grid.getHeight(cell_index); down++){
                        m_trans = glm::translate(m_trans, vec3(0,-1,0));
                        glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*m_trans));
                    }
                }
                // Translate to the next cell on the grid
                m_trans = glm::translate(m_trans, vec3(1,0,0));
                glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*m_trans));
            }
            // At the end of a row, translate back to the beginning of the row (reset the x value to 0)
            for (int rev_col = 0; rev_col < DIM; rev_col++){
                m_trans = glm::translate(m_trans, vec3(-1,0,0));
                glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*m_trans));
            }
            
            // Translate down to the next column (increase the z value by 1)
            m_trans = glm::translate(m_trans, vec3(0,0,1));
            glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*m_trans));
            
        }
        // At the end of the grid, translate back to the original starting square
        for (int rev_row = 0; rev_row < DIM; rev_row++){
            m_trans = glm::translate(m_trans, vec3(0,0,-1));
            glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*m_trans));
        }

    
		// Highlight the active cell by:
        // - Outlining active cell on the grid
        // - having two squares on the grid outline representing the x and z values of
        //   the active cell
    
        glBindVertexArray( m_square_vao );
    
        // Three translations needed for the highlight goals
        mat4 outer_ring_trans_x;
        mat4 outer_ring_trans_z;
        mat4 active_trans;
    
        // Determine how much to move the two outline squares and the outline
        int move_by_x = active_cell % DIM;
        int move_by_y = current_grid.getHeight(active_cell) > 0 ? current_grid.getHeight(active_cell) : 0;
        int move_by_z = active_cell/DIM;
    
        // Create the translations
        outer_ring_trans_x = glm::translate(outer_ring_trans_x, vec3(move_by_x,0,-1));
        outer_ring_trans_z = glm::translate(outer_ring_trans_z, vec3(-1,0,move_by_z));
        active_trans = glm::translate(active_trans, vec3(move_by_x,move_by_y,move_by_z));
    
        // Draw active square x pos on outer ring
        glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*outer_ring_trans_x));
        glUniform3f(col_uni, 0, 0, 0);
        glDisable(GL_DEPTH_TEST);
        glDrawArrays( GL_TRIANGLES, 8, 6);
    
        // Reverse the translation
        glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*glm::inverse(outer_ring_trans_x)));
    
        // Draw active square z pos on outer ring
        glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*outer_ring_trans_z));
        glDrawArrays( GL_TRIANGLES, 8, 6);
        
        // Reverse the translation
        glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*glm::inverse(outer_ring_trans_z)));
    
        // Highlight boarder squares on grid for active square placement
        glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(W*active_trans));
        glDrawArrays( GL_LINE_LOOP, 0, 8);
    

    
	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
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
bool A1::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
        
        // If the mouse is being pressed...
        if (m_mouseButtonActive) {
            
            // Find the difference from the previous x value
            float diff_x = prev_x - xPos;
            
            // Update the shape rotation and scale it by 100
            m_shape_rotation += - diff_x / 100;
            
            //Update the previous x position to the current x value
            prev_x = xPos;
        }
	}
    
    return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);
    
    // If the button is pressed...
    if (actions == GLFW_PRESS) {
        if (!ImGui::IsMouseHoveringAnyWindow()) {
            // Indicate the button is being held
            m_mouseButtonActive = true;
            
            // Get the x and y value and update previous x/y variables
            glfwGetCursorPos(m_window, &prev_x, &prev_y);
        }
    }
    
    // Once the button is released, indicate the button is no longer being held
    if (actions == GLFW_RELEASE) {
        m_mouseButtonActive = false;
    }


	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);
    
	// Zoom in or out
    // If y offset is pos, then zoom in
    // If the y offset is neg, then zoom out
    // Scale by 100
    
    // Check to make sure we don't scale by a negative value
    if (m_shape_size+(yOffSet/100) < 0){
        m_shape_size = 0;
    } else {
        m_shape_size += yOffSet/100;
    }
    

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
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
        
        if (key == GLFW_KEY_R){
            // Reset to default values
            reset();
        }
        
        if (key == GLFW_KEY_SPACE) {
            cout << "Space key pressed" << endl;
            // If the height of the active cell is 0, then set the colour to the active colour
            if (current_grid.getHeight(active_cell) == 0){
                
                current_grid.setColour(active_cell, current_col);
            }
            // Increase the height of the active cell by 1
            current_grid.setHeight(active_cell,current_grid.getHeight(active_cell)+1);
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_BACKSPACE) {
            cout << "Backspace key pressed" << endl;
            
            // If the height of the active cell is greater than zero, decrease the height by 1
            // If the height is zero or less, do nothing
            if (current_grid.getHeight(active_cell) > 0) {
                current_grid.setHeight(active_cell,current_grid.getHeight(active_cell)-1);
            }
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_LEFT) {
            // If the active cell is not at a left edge, move the active cell left by 1 unit
            if (active_cell % DIM != 0 && active_cell > 0){

                active_cell -= 1;
                
                // If the shift mod is being held down, copy the current active cell's height and colour
                // to the new active cell
                if (mods == GLFW_MOD_SHIFT){
                    current_grid.setHeight(active_cell, current_grid.getHeight(active_cell+1));
                    current_grid.setColour(active_cell, current_grid.getColour(active_cell+1));
                }
            }
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_RIGHT) {
            // If the active cell is not at a right edge, move the active cell right by 1 unit
            if (active_cell == 0 || (active_cell % DIM != 15 && active_cell < 256)) {
                active_cell += 1;
                
                // If the shift mod is being held down, copy the current active cell's height and colour
                // to the new active cell
                if (mods == GLFW_MOD_SHIFT){
                    current_grid.setHeight(active_cell, current_grid.getHeight(active_cell-1));
                    current_grid.setColour(active_cell, current_grid.getColour(active_cell-1));
                }
            }
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_UP) {
            // If the active cell is not at the top of the grid, move the active cell up by one unit
            if (active_cell >= DIM){
                active_cell -= DIM;
                
                // If the shift mod is being held down, copy the current active cell's height and colour
                // to the new active cell
                if (mods == GLFW_MOD_SHIFT){
                    current_grid.setHeight(active_cell, current_grid.getHeight(active_cell+DIM));
                    current_grid.setColour(active_cell, current_grid.getColour(active_cell+DIM));
                }
            }
            
            eventHandled = true;
        }
        
        if (key == GLFW_KEY_DOWN) {
            // If the active cell is not at the bottum of the grid, move the active cell down by one unit
            if (active_cell < DIM*DIM-DIM){
                active_cell += DIM;
                
                // If the shift mod is being held down, copy the current active cell's height and colour
                // to the new active cell
                if (mods == GLFW_MOD_SHIFT){
                    current_grid.setHeight(active_cell, current_grid.getHeight(active_cell-DIM));
                    current_grid.setColour(active_cell, current_grid.getColour(active_cell-DIM));
                }
            }
            
            eventHandled = true;
        }
	}

	return eventHandled;
}
