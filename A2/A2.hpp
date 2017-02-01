#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
	VertexData();

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};


class A2 : public CS488Window {
public:
	A2();
	virtual ~A2();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();

	void initLineData();

	void setLineColour(const glm::vec3 & colour);

	void drawLine (
			const glm::vec2 & v0,
			const glm::vec2 & v1
	);
    
    // Matrix Manipulation Functions
    glm::mat4 translate(glm::vec3 const& v);
    glm::mat4 scale(glm::vec3 const& s);
    glm::mat4 rotate_z(float r);
    glm::mat4 rotate_x(float r);
    glm::mat4 rotate_y(float r);
    glm::mat4 perspective(double a, double aspect, double far, double near);
    
    // Manipulation Fields
    enum States {
        O_ROTATE,
        N_TRANSLATE,
        P_PERSPECTIVE,
        R_ROTATE,
        S_SCALE,
        T_TRANSLATE,
        V_VIEW
    };
    int current_state;
    
    // Mouse Fields
    
    bool mouseLeftButtonActive;
    bool mouseMiddleButtonActive;
    bool mouseRightButtonActive;
    double prev_x;
    double prev_y;
    
    // Current Model/View/Perspective Fields
    float current_model_rotation[3] = {0.0f,0.0f,0.0f};
    float current_model_translation[3] = {0.0f,0.0f,0.0f};
    float current_model_scale[3] = {1.0f,1.0f,1.0f};
    float current_view_rotation[3] = {0.0f,0.0f,0.0f};
    float current_view_translation[3] = {0.0f,0.0f,0.0f};
    float current_perspective[3] = {0.0f,0.0f,0.0f};
    float current_fov_a;
    float current_near;
    float current_far;
    float current_aspect;
    
    // Current Viewport Fields
    float current_viewport[4] = {0.0f,0.0f,0.0f,0.0f};
    
    // Transformation Matrices
    glm::mat4 projection_mat;
    glm::mat4 view_mat;
    glm::mat4 model_mat;
    
    // Default Vertices for Cube & Gnomons
    glm::vec3 cube[24] = {
        glm::vec3(1.0, 1.0, 1.0), glm::vec3(-1.0, 1.0, 1.0), // Cube Front
        glm::vec3(-1.0, 1.0, 1.0), glm::vec3(-1.0, -1.0, 1.0),
        glm::vec3(-1.0, -1.0, 1.0), glm::vec3(1.0, -1.0, 1.0),
        glm::vec3(1.0, -1.0, 1.0), glm::vec3(1.0, 1.0, 1.0),
        
        glm::vec3(1.0, 1.0, -1.0), glm::vec3(-1.0, 1.0, -1.0), // Cube Back
        glm::vec3(-1.0, 1.0, -1.0), glm::vec3(-1.0, -1.0, -1.0),
        glm::vec3(-1.0, -1.0, -1.0), glm::vec3(1.0, -1.0, -1.0),
        glm::vec3(1.0, -1.0, -1.0), glm::vec3(1.0, 1.0, -1.0),
        
        glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, -1.0), // Cube Sides
        glm::vec3(1.0, -1.0, 1.0), glm::vec3(1.0, -1.0, -1.0),
        glm::vec3(-1.0, 1.0, 1.0), glm::vec3(-1.0, 1.0, -1.0),
        glm::vec3(-1.0, -1.0, 1.0), glm::vec3(-1.0, -1.0, -1.0)
    };
    glm::vec3 gnomon_X[2] = {
        glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)
    };
    glm::vec3 gnomon_Y[2] = {
        glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0)
    };
    glm::vec3 gnomon_Z[2] = {
        glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 0.0)
    };
    
    // Additional Functions for Transforming Points
    void draw_item(glm::vec3 *points, int num_points, glm::mat4 transforms, glm::vec3 colours);
    void reset_view();
    bool clip_line(glm::vec4 &p1, glm::vec4 &p2);
    void map_to_viewport(glm::vec2& p);
    
    
    // Other fields
	ShaderProgram m_shader;

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

};
