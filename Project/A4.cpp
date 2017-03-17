#include <glm/ext.hpp>
#include <thread>
#include <vector>

#include "GeometryNode.hpp"
#include "PhongMaterial.hpp"
#include "Helpers.hpp"

#include "A4.hpp"

//#define SUPERSAMPLE
//#define STARS


struct Ray {
    glm::vec3 origin ;
    glm::vec3 direction ;
};

glm::vec3 transNorm(glm::mat4 M, const glm::vec3 n){
    return glm::vec3(n[0] * M[0][0] + n[1] * M[0][1] + n[2] * M[0][2],
                     n[0] * M[1][0] + n[1] * M[1][1] + n[2] * M[1][2],
                     n[0] * M[2][0] + n[1] * M[2][1] + n[2] * M[2][2]);
}

bool find_intersection(Ray r, SceneNode* node, Intersection &inter){
    // Test if ray intersects object
    
    // First, check if the ray intersects the current object
    // Then check it's children
    
    // Need to transform ray to object's model coords
    // Transform the ray to geonode's model coords using the inverse transform WCS -> MCS
    r.origin = glm::vec3(node->get_inverse() * glm::vec4(r.origin, 1.0));
    r.direction = glm::vec3(node->get_inverse() * glm::vec4(r.direction, 0.0));
    
    bool intersection_result = false;
    
    // Check if Geo Node
    if (node->m_nodeType == NodeType::GeometryNode){
        // Determine if there is an intersection with this geonode...
        GeometryNode *gnode = static_cast<GeometryNode*>(node);
        intersection_result = gnode->m_primitive->intersect(r.origin, r.direction, inter);
        if (intersection_result){
            inter.material = gnode->m_material;
            //inter.inter_point = glm::vec3(node->get_transform() * glm::vec4(inter.inter_point, 1.0));
            //inter.inter_normal = glm::vec3(glm::normalize(transNorm(node->get_inverse(), glm::vec4(inter.inter_normal, 0.0))));
            //return true;
        }
    }
    
    for(auto child: node->children){
        Intersection child_inter;
        // If the ray intersects the child object...
        if (find_intersection(r, child, child_inter)){
            // Check if the intersection point is closer than the current closest intersection (inter)
            // Note if any of the the x,y,z values of the parent intersection are infinite, then the new intersection point will be closer
            
            if (inter.inter_point[0] == INFINITY || inter.inter_point[1] == INFINITY || inter.inter_point[2] == INFINITY ||
                (glm::length(child_inter.inter_point - glm::vec3(r.origin))) < glm::length((inter.inter_point - glm::vec3(r.origin)))){
                inter = child_inter;
                //inter.inter_point = child_inter.inter_point;
                //inter.inter_normal = child_inter.inter_normal;
                //inter.material = child_inter.material;
            }
            intersection_result = true;
        }
        
    }
    
    if (intersection_result){
        inter.inter_point = glm::vec3(node->get_transform() * glm::vec4(inter.inter_point, 1.0));
        inter.inter_normal = glm::normalize(transNorm(node->get_inverse(), inter.inter_normal));
    }

    return intersection_result;
}

glm::mat4 world_pixels(size_t width, size_t height, glm::vec3 lookfrom, double fov, glm::vec3 view, glm::vec3 up){
    
    //double fov_r = fov * M_PI / 180.0;
    //double h = 2.0*d*tan(fov_r / 2.0); // height of projection plane based field of view and distance to the plane
    
    // P_world = T4R3S2T1pk
    
    // Translate point by (-nx/2, -ny/2, d) where d = depth of the view (ie. view.length) and nx = image width and ny = image height
    glm::mat4 T1 = glm::mat4();
    T1 = glm::column(T1, 3, glm::vec4((double)width*(-1)/2.0, (double)height*(-1)/2.0, glm::length(view), 1.0));
    
    //std::cout << glm::to_string(T1) << std::endl;
   
    // Scale point by (-h/ny, w/nx, 1) to preserve the aspect ratio
    glm::mat4 S2 = glm::mat4();
    
    // Height of the projection plane
    double h = 2.0 * glm::length(view) * tan(glm::radians(fov)/2.0);
    // Width of the projection plane
    double w = ((double)width/(double)height)*h;
    
    S2 = glm::column(S2, 0, glm::vec4(((-1.0f)*h/(double)height), 0.0, 0.0, 0.0));
    S2 = glm::column(S2, 1, glm::vec4(0.0, ((-1.0f)*w/(double)width), 0.0, 0.0));
    S2 = glm::column(S2, 2, glm::vec4(0.0, 0.0, 1.0, 0.0));
    
    //std::cout << glm::to_string(S2 * T1 * p) << std::endl;
    
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
    
    //std::cout << glm::to_string(R3) << std::endl;
    
    // Translate view by T
    glm::mat4 T4 = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
                             glm::vec4(0.0, 1.0, 0.0, 0.0),
                             glm::vec4(0.0, 0.0, 1.0, 0.0),
                             glm::vec4(lookfrom, 1.0));
    
    // Return matrices mult together in order specified above
    return T4*R3*S2*T1;
}

glm::vec3 determine_lighting(Ray r, Intersection &inter, Light* light, const PhongMaterial *material){
    
    // We need to first get the vector from the intsection point to the light source
    glm::vec3 p_vec = light->position - inter.inter_point;
    // We also need the vector from the eye to the point
    glm::vec3 e_vec = glm::vec3(r.origin) - inter.inter_point;
    
    // Colour is determined by a basic lighting model via the course notes section 16
    // 1) diffuse colour
    // 2) specular colour
    // 3) attenutation of the sum of diffuse and specular
    
    // 1) Diffuse colour
    // Determine the diffuse brightness using the p_vector and the intersection normal
    double diffuse_b = glm::dot(inter.inter_normal, glm::normalize(p_vec));
    
    // Check the diffuse brightness is not negative
    if (diffuse_b < 0.0){
        diffuse_b = 0.0;
    }
    
    // Using the diffuse brightness, determine the diffuse colour component
    glm::vec3 diffuse_colour = diffuse_b * material->get_kd() * light->colour;
    
    // 2) Specular colour
    // Now we need to determine the specular brightness
    // (dot product of e_vec and reflected)^shininess of material
    // Note, we cannot have specular brightness if the diffuse brightness is < 0
    double specular_b;
    if (diffuse_b > 0.0){
        glm::vec3 norm_neg_p_vec = (-1.0f)*glm::normalize(p_vec);
        glm::vec3 reflected_vec = glm::normalize(norm_neg_p_vec - 2*glm::dot(norm_neg_p_vec, inter.inter_normal)*inter.inter_normal);
        specular_b = glm::dot(glm::normalize(e_vec), reflected_vec);
        // Check if negative and if so, set to zero
        if (specular_b < 0.0){
            specular_b = 0.0;
        }
        // Add "shininess factor"
        specular_b = std::pow(specular_b, material->get_shininess());
    } else {
        specular_b = 0.0;
    }
    
    // Using the specular brightness, determine the specular colour component
    glm::vec3 specular_colour = specular_b * material->get_ks() * light->colour;
    
    // 3) Attenuation of the sum of d and s
    // Get the attenuation factor using the light fall off field
    double attenuation = 1.0f / (light->falloff[0] + light->falloff[1]*glm::length(p_vec) + light->falloff[2]*(glm::length(p_vec)*glm::length(p_vec)));
    if (attenuation != 1){
        std::cout << attenuation << std::endl;
    }
    
    // The final colour is attenuation * (diffuse + specular)
    return attenuation * (diffuse_colour + specular_colour);
}

glm::vec3 ray_colour(Ray r, glm::vec3 bg, SceneNode *root, const glm::vec3 & ambient, const std::list<Light *> & lights, int reflect_count){
    
    Intersection intersection;
    if (find_intersection(r, root, intersection)){
        
        const PhongMaterial* material = dynamic_cast<const PhongMaterial*>(intersection.material);
        // Material does not emit light in this assignment so no ke
        glm::vec3 colour_vec = material->get_kd() * ambient;
        glm::vec3 p = intersection.inter_point + (1e-2)*intersection.inter_normal;
        
        // For all the different light sources...
        for(auto light : lights)
        {
            // Shoot shadow ray to the light source to determine amount of colour is "shown" via the light source
            // Shadow ray origin is p and direction is light's position - p
            Ray shadow_ray = {p, light->position - p};
            // Create new intersection object for the shadow ray
            Intersection shadow_inter;
            
            // Make sure to check if intersection point is before light source
            
            // Check if the shadow ray hits another object (ie. the light is blocked)
            if(find_intersection(shadow_ray, root, shadow_inter) &&
               (glm::length(shadow_inter.inter_point - shadow_ray.origin) < glm::length(light->position - shadow_ray.origin))){
                // Don't add the colour for this light source since it is blocked by an object in front of the light source
                continue;
            }
            
            // "Add" the colour for this light source to the object
            colour_vec = colour_vec + determine_lighting(r, intersection, light, material);
        }
        
        // Define a reflective colour. White will suffice
        glm::vec3 reflected_colour(0.0, 0.0, 0.0);
        // Check if we have any more reflective rays to send
        if (!material->zero_ks() && reflect_count > 0){
            Ray reflective_ray = {p, r.direction - 2*glm::dot(r.direction, intersection.inter_normal)*intersection.inter_normal};
            reflected_colour = ray_colour(reflective_ray, reflected_colour, root, ambient, lights, reflect_count--);
        }
        
        return colour_vec + (1.0 / lights.size()) * reflected_colour * material->get_ks();
    }

    return bg;
}

void thread_render(SceneNode *root, Image & image, const glm::vec3 & eye, const glm::vec3 & view, const glm::vec3 & up, double fovy, const glm::vec3 & ambient, const std::list<Light *> & lights, glm::mat4 pixel_to_world, int start_x, int end_x, int start_y, int end_y, int thread_num, int y_skip) {
    
    int total_pixels = image.width() * image.height()/y_skip;
    int current_pixel = 1;
    int current_percent = 0;
    
    for (int y = start_y; y < end_y; y += y_skip) {
        for (int x = start_x; x < end_x; x++) {
            
            if (int(floorf((float)current_pixel / (float)total_pixels*100))%1 == 0){
                if (current_percent != int(floorf((float)current_pixel / (float)total_pixels*100))){
                    std::cout << "Thread " << thread_num+1 << " Progress: " << int(floorf((float)current_pixel / (float)total_pixels*100)) << "%" << std::endl;
                    current_percent = int(floorf((float)current_pixel / (float)total_pixels*100));
                }
                
            }
            
            // Background colour
            glm::vec3 background_col = ((y) & 0x10) ? ((double)y/image.height()*glm::vec3(0.25, 0.25, 0.25)) + glm::vec3(0.68, 0.91, 0.99) : ((image.height() - (double)y)/image.height()*glm::vec3(0.25, 0.25, 0.25)) + glm::vec3(0.68, 0.91, 0.99);
            
            // Pixel to World Coords
            glm::vec4 pixel = glm::vec4(x, y, 0.0, 1.0);
            glm::vec3 p = glm::vec3(pixel_to_world * pixel);
            
            //std::cout << view.length() << std::endl;
            //std::cout << "pixel: " << glm::to_string(pixel) << std::endl;
            //std::cout << "p: " << glm::to_string(p) << std::endl;
            
            // Create the ray with origin at the eye point
            Ray ray = {eye, p - eye};
            
            // Cast ray into the scene and get the colour returned
            glm::vec3 colour = ray_colour(ray, background_col, root, ambient, lights, 5);
            
            image(x, y, 0) = colour[0];
            image(x, y, 1) = colour[1];
            image(x, y, 2) = colour[2];
            
            current_pixel++;
        }
    }
}

void test_func(SceneNode * root, Image & image){
    std::cout << "test " << image.width() << " " << image.height() << std::endl;
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

    glm::mat4 pixel_to_world = world_pixels(image.width(), image.height(), eye, fovy, view, up);

    // x is width, y is height
    int num_threads = 4;
    
    std::vector<std::thread> threads(num_threads);
    for(int i = 0; i < num_threads; i++)
    {
        threads[i] = std::thread(thread_render, root, std::ref(image), eye, view, up, fovy, ambient, lights, pixel_to_world, 0, image.width(), i, image.height(), i, num_threads);
        //threads[i] = std::thread(test_func, root, std::ref(image));
        if(threads[i].get_id() == std::thread::id())
        {
            std::cerr << "Abort: Failed to create thread " << i << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    
    for(unsigned int i = 0; i < num_threads; i++) threads[i].join();
}
