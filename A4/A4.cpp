#include <glm/ext.hpp>

#include "GeometryNode.hpp"

#include "A4.hpp"


struct Ray {
    glm::vec4 origin ;
    glm::vec4 direction ;
};

struct Colour {
    float r;
    float g;
    float b;
};


bool intersection(Ray r, SceneNode* node){
    // Test if ray intersecs objects
    
    // Need to transform ray to object's model coords
    r.origin = node->invtrans * r.origin;
    r.direction = node->invtrans * r.direction;
    
    bool intersection = false;
    
    // Check if Geo Node
    if (node->m_nodeType == NodeType::GeometryNode){
        GeometryNode *gnode = static_cast<GeometryNode*>(node);
        intersection = gnode->m_primitive->intersect(r.origin, r.direction);
    }
    
    return intersection;
    
    //Intersection k;
    //bool intersects = m_primitive->intersect(r, k);
    //if(intersects)
    //{
    //    i.q = m_trans * k.q;
    //    i.n = transNorm(m_invtrans, k.n).normalized();
    //    i.m = m_material;
    //}
    
    //return (intersects || SceneNode::intersect(ray, i));
}



glm::mat4 world_pixels(size_t width, size_t height, glm::vec3 lookfrom, double fov, glm::vec3 view, glm::vec3 up){
    
    //double fov_r = fov * M_PI / 180.0;
    //double h = 2.0*d*tan(fov_r / 2.0); // height of projection plane based field of view and distance to the plane
    
    // P_world = T4R3S2T1pk
    glm::vec4 p = glm::vec4(0.0, 0.0, 0.0, 1.0);
    
    // Translate point by (-nx/2, -ny/2, d) where d = depth of the view (ie. view.length) and nx = image width and ny = image height
    glm::mat4 T1 = glm::mat4();
    T1 = glm::column(T1, 3, glm::vec4((double)width*(-1)/2.0, (double)height*(-1)/2.0, view.length(), 1.0));
    
    std::cout << glm::to_string(T1) << std::endl;
    std::cout << glm::to_string(T1 * p) << std::endl;
   
    // Scale point by (-h/ny, w/nx, 1) to preserve the aspect ratio
    glm::mat4 S2 = glm::mat4();
    
    // Height of the projection plane
    double h = 2.0 * view.length() * tan(glm::radians(fov)/2.0);
    // Width of the projection plane
    double w = ((double)width/(double)height)*h;
    
    S2 = glm::column(S2, 0, glm::vec4((-h/(double)height), 0.0, 0.0, 0.0));
    S2 = glm::column(S2, 1, glm::vec4(0.0, (w/(double)width), 0.0, 0.0));
    S2 = glm::column(S2, 2, glm::vec4(0.0, 0.0, 1.0, 0.0));
    
    std::cout << glm::to_string(S2 * T1 * p) << std::endl;
    
    // Rotate to superimpose WCS to VCS
    // R3 = [ ux vx wx 0 ]
    //      [ uy vy wy 0 ]
    //      [ uz vz wz 0 ]
    //      [ 0  0  0  1 ]
    // where w = (lookat - lookfrom)/|lookat - lookfrom| = view
    //       u = (up x w) / |up x w|
    //       v = w x n
    
    // normalize view matrix just to be safe
    glm::vec3 w_norm = glm::normalize(view);
    // normalize up matrix just to be safe
    glm::vec3 up_norm = glm::normalize(up);
    // Get u from cross of up_norm and w
    // Normalize it
    glm::vec3 u_norm = glm::normalize(glm::cross(up_norm, w_norm));
    // Get v from cross of w and up
    glm::vec3 v_norm = glm::cross(w_norm, u_norm);
    
    
    // Calculate the basis for the view coordinate system
    //view = glm::normalize(view);
    //up = glm::normalize(up);
    //glm::vec3 u = glm::cross(up,view);
    //u = glm::normalize(u);
    //glm::vec3 v = glm::cross(view, u);
    //v = glm::normalize(v);

    glm::mat4 R3 = glm::mat4(glm::vec4(u_norm, 0.0),
                             glm::vec4(v_norm, 0.0),
                             glm::vec4(w_norm, 0.0),
                             glm::vec4(0.0, 0.0, 0.0, 1.0));
    
    std::cout << glm::to_string(R3) << std::endl;
    
    // Translate view by T
    glm::mat4 T4 = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                             glm::vec4(0.0, 1.0, 0.0, 0.0),
                             glm::vec4(0.0, 0.0, 1.0, 0.0),
                             glm::vec4(lookfrom, 1.0));
    
    // Return matrices mult together in order specified above
    return T4*R3*S2*T1;
}

Colour ray_colour(Ray r, Colour bg, SceneNode *root){
    
    for(auto child : root->children){
        if (intersection(r, child)){
            std::cout << "child: " << child->m_name << std::endl;
            Colour c = {1.0, 1.0, 1.0};
            return c ;
        }
    }
    
    return bg;
}


void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {

  // Fill in raytracing code here...

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;
/*
	size_t h = image.height();
	size_t w = image.width();

	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			// Red: increasing from top to bottom
			image(x, y, 0) = (double)y / h;
			// Green: increasing from left to right
			image(x, y, 1) = (double)x / w;
			// Blue: in lower-left and upper-right corners
			image(x, y, 2) = ((y < h/2 && x < w/2)
						  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
		}
	}
 */
    glm::mat4 pixel_to_world = world_pixels(image.width(), image.height(), eye, fovy, view, up);

    for (int y = 0; y < image.height(); y += 1) {
        for (int x = 0; x < image.width(); x++) {
            // Pixel to World Coords
            glm::vec4 pixel = glm::vec4(x, y, 0.0, 1.0);
            glm::vec4 p = pixel_to_world * pixel;
        
            //std::cout << view.length() << std::endl;
            //std::cout << glm::to_string(pixel) << std::endl;
            //std::cout << glm::to_string(p) << std::endl;
            
            // Create the ray with origin at the eye point
            Ray ray = {glm::vec4(eye, 1.0), p - glm::vec4(eye, 1.0)};
            
            // Background colour (for now its just white)
            Colour background_col = {0.0, 0.0, 0.0};
            
            // Cast ray into the scene and get the colour returned
            Colour colour = ray_colour(ray, background_col, root);//, d.root, d.lights, d.ambient, bg, 1);
            
            image(x, y, 0) = colour.r;
            image(x, y, 1) = colour.g;
            image(x, y, 2) = colour.b;
        }
    }
}
