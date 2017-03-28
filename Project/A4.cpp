#include <glm/ext.hpp>
#include <thread>
#include <vector>
#include <random>
#include <time.h>

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
    
    std::string name;
    
    bool intersection_result = false;
    
    // Check if Geo Node
    if (node->m_nodeType == NodeType::GeometryNode){
        // Determine if there is an intersection with this geonode...
        GeometryNode *gnode = static_cast<GeometryNode*>(node);
        
        //std::cout << "object: " << node->m_name << std::endl;
        
        intersection_result = gnode->m_primitive->intersect(r.origin, r.direction, inter);
        if (intersection_result){
            name = node->m_name;
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
                name = child->m_name;
                //inter.inter_point = child_inter.inter_point;
                //inter.inter_normal = child_inter.inter_normal;
                //inter.material = child_inter.material;
            }
            intersection_result = true;
        }
        
    }
    
    if (intersection_result){
        //std::cout << "intersection with object: " << name << std::endl;
        inter.inter_point = glm::vec3(node->get_transform() * glm::vec4(inter.inter_point, 1.0));
        inter.inter_normal = glm::normalize(transNorm(node->get_inverse(), inter.inter_normal));
        inter.x = glm::normalize(transNorm(node->get_inverse(), inter.x));
        inter.y = glm::normalize(transNorm(node->get_inverse(), inter.y));
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
   
    // Scale point by (-h/ny, w/nx, 1) to preserve the aspect ratio
    glm::mat4 S2 = glm::mat4();
    
    // Height of the projection plane
    double h = 2.0 * glm::length(view) * tan(glm::radians(fov)/2.0);
    // Width of the projection plane
    double w = ((double)width/(double)height)*h;
    
    S2 = glm::column(S2, 0, glm::vec4(((-1.0f)*h/(double)height), 0.0, 0.0, 0.0));
    S2 = glm::column(S2, 1, glm::vec4(0.0, ((-1.0f)*w/(double)width), 0.0, 0.0));
    S2 = glm::column(S2, 2, glm::vec4(0.0, 0.0, 1.0, 0.0));
    
    // Rotate to superimpose WCS to VCS
    // R3 = [ ux vx wx 0 ]
    //      [ uy vy wy 0 ]
    //      [ uz vz wz 0 ]
    //      [ 0  0  0  1 ]
    // where w = (lookat - lookfrom)/|lookat - lookfrom| = view
    //       u = (up x w) / |up x w|
    //       v = w x n
    
    // normalize view matrix just to be safe
    glm::vec3 w_norm = glm::normalize(view - lookfrom);
    // normalize up matrix just to be safe
    //glm::vec3 up_norm = glm::normalize(up);
    // Get u from cross of up_norm and w
    // Normalize it
    glm::vec3 u_norm = glm::normalize(glm::cross(up, w_norm));
    // Get v from cross of w and up
    glm::vec3 v_norm = glm::normalize(glm::cross(w_norm, u_norm));

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

glm::vec3 determine_lighting(Ray r, Intersection &inter, Light* light, const PhongMaterial *material, glm::vec3 light_pos){
    
    // We need to first get the vector from the intsection point to the light source
    glm::vec3 p_vec = light_pos - inter.inter_point;
    // We also need the vector from the eye to the point
    glm::vec3 e_vec = glm::vec3(r.origin) - inter.inter_point;
    
    glm::vec3 normal = material->get_bumped_normal(inter.u, inter.v, inter.x, inter.y, inter.inter_normal);
    if (inter.inter_normal.x != normal.x && inter.inter_normal.y != normal.y && inter.inter_normal.z != normal.z){
        //std::cout << "normal changed! old: " << glm::to_string(inter.inter_normal) << " to " << glm::to_string(normal) << std::endl;
    }
    
    // Colour is determined by a basic lighting model via the course notes section 16
    // 1) diffuse colour
    // 2) specular colour
    // 3) attenutation of the sum of diffuse and specular
    
    // 1) Diffuse colour
    // Determine the diffuse brightness using the p_vector and the intersection normal
    double diffuse_b = glm::dot(normal, glm::normalize(p_vec));
    
    // Check the diffuse brightness is not negative
    if (diffuse_b < 0.0){
        diffuse_b = 0.0;
    }
    
    // Using the diffuse brightness, determine the diffuse colour component
    glm::vec3 diffuse_colour = diffuse_b * material->get_kd(inter.u, inter.v) * light->colour;
    
    // 2) Specular colour
    // Now we need to determine the specular brightness
    // (dot product of e_vec and reflected)^shininess of material
    // Note, we cannot have specular brightness if the diffuse brightness is < 0
    double specular_b;
    if (diffuse_b > 0.0){
        glm::vec3 norm_neg_p_vec = (-1.0f)*glm::normalize(p_vec);
        glm::vec3 reflected_vec = glm::normalize(norm_neg_p_vec - 2*glm::dot(norm_neg_p_vec, normal)*normal);
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

double get_fresnel_R(double n1, double n2, glm::vec3 direction, glm::vec3 normal, double cosI, double cosT){
    double R0 = pow((n1-n2)/(n1+n2), 2);
    //double R0 = ((n1 - n2)*(n1 - n2))/((n1 + n2)*(n1 + n2));
    
    double R;
    if (n1 <= n2){
        R = R0 + (1-R0)*pow((1-cosI), 5);
    } else {
        R = R0 + (1-R0)*pow((1-cosT), 5);
    }
    //std::cout << "R0: " << R0 << std::endl;
    //std::cout << "R: " << R << std::endl;
    //std::cout << "cosT: " << cosT << std::endl;
    return R;
}

Ray get_refracted_ray (Ray incident, Intersection intersection, double index, bool& totalInternalReflection, double& fresnel, double& final_ni, double& final_nt) {
    double n_i, n_t; // indices of refraction for the two mediums
    double n; // will store n_i/n_t
    
    // refracted ray origin and direction
    glm::vec3 t_origin = glm::vec3(0, 0, 0);
    glm::vec3 t_direction = glm::vec3(0, 0, 0);
    
    //glm::vec3 norm_v = glm::normalize(incident.direction);
    glm::vec3 norm_v = incident.direction;
    glm::vec3 normal = intersection.inter_normal;
    
    // Check if the incident ray is entering or exiting a primitive
    // If the normal and the incident ray have a negative dot product, then the ray is entering the primitive
    double vndot = glm::dot(norm_v, normal) ;
    if (glm::dot(norm_v, normal) < 0) {
        // incident ray entered primitive
        //std::cout << "entering" << std::endl;
        n_i = 1.0;
        n_t = index;
    } else {
        // incident ray exited primitive
        //std::cout << "exiting" << std::endl;
        n_i = index;
        n_t = 1.0;
        
        // need to flip the normal
        normal = (-1) * normal;
    }
    vndot = glm::dot(norm_v, normal) < 0;
    n = n_i/n_t;
    //std::cout << "n: " << n << std::endl
    double bigugly = 1 - (pow(n, 2) * (1-pow(vndot,2)));
    if (bigugly < 0) {
        // total internal reflection
        std::cout << "total internal" << std::endl;
        totalInternalReflection = true;
        // return a dud ray
        
        return Ray{t_origin, t_direction};
    } else {
        totalInternalReflection = false;
        t_origin = intersection.inter_point + (-(1e-2)*normal);
        t_direction = ((-1)*n*vndot - sqrt(bigugly))*normal + (n*norm_v);
        //std::cout << "incident: " << glm::to_string(norm_v) << std::endl;
        //std::cout << "refracted direction: " << glm::to_string(t_direction) << std::endl;
        fresnel = get_fresnel_R(n_i, n_t, norm_v, normal, vndot, sqrt(bigugly));
        final_ni = n_i ;
        final_nt = n_t ;
        return Ray{t_origin, t_direction};
    }
}

Ray get_glossy_ray(Ray og_ray, double shiny){
    // glossy_ray = OG_ray + i*U + j*V
    // i,j = -a/2 + rand()*a
    // a = 1/(1+ks)
    // U = smallest abs val of OG_ray, setting it to zero, swapping two other values, and negating the first one
    // V = cross product of OG_ray and normalized U

    glm::vec3 direction = og_ray.direction;
    glm::vec3 U, V;
    
    // Get the basis vectors for the square of size <glossiness>x<glossiness>
    if(std::abs(direction[2]) > std::abs(direction[0]) && std::abs(direction[2]) > std::abs(direction[1])) {
        U = glm::vec3(-direction[1], direction[0], 0.0);
    }
    else if(std::abs(direction[1]) > std::abs(direction[0])) {
        U = glm::vec3(-direction[2], 0.0, direction[0]);
    }
    else{
        U = glm::vec3(0.0, -direction[2], direction[1]);
    }
    U = glm::normalize(U);
    V = glm::normalize(glm::cross(direction, U));
    
    double a = 1.0 / (1.0 + shiny);
    
    double i = -a/2.0 + ((double)rand()/(double)RAND_MAX)*a;
    double j = -a/2.0 + ((double)rand()/(double)RAND_MAX)*a;
    
    glm::vec3 glossy_direction = og_ray.direction + i*U + j*V;
    return Ray{og_ray.origin, glossy_direction};
}

glm::vec3 ray_colour(Ray r, glm::vec3 bg, SceneNode *root, const glm::vec3 & ambient, const std::list<Light *> & lights, int reflect_count, int refract_count, int glossy_rays, int soft_rays){
    
    Intersection intersection;
    if (find_intersection(r, root, intersection)){
        
        const PhongMaterial* material = dynamic_cast<const PhongMaterial*>(intersection.material);
        // Material does not emit light in this assignment so no ke
        glm::vec3 colour_vec = material->get_kd(intersection.u, intersection.v) * ambient;
        glm::vec3 p = intersection.inter_point + (1e-2)*intersection.inter_normal;
        
        if (!material->zero_kd()){
            // For all the different light sources...
            for(auto light : lights)
            {
                // Soft Shading Enabled
                if (soft_rays > 0){
                    // Base Shade_colour
                    glm::vec3 shade_colour = glm::vec3(0.0, 0.0, 0.0);
                    
                    //std::cout << "soft rays: " << soft_rays << std::endl;
                    
                    // For each shadow ray in soft_rays
                    for(int s=0; s < soft_rays; s++){
                        // 1) Create shadow ray pointing to some point of the non-zero area light
                        // 2) Get the shade colour for that shadow ray using determine_lighting
                        glm::vec3 light_pos = glm::vec3(light->position.x +((double)rand()/(double)RAND_MAX)*light->width, light->position.y, light->position.z + ((double)rand()/(double)RAND_MAX)*light->height);
                        Ray shadow_ray = {p, light_pos - p};
                        Intersection shadow_inter;
                        
                        //std::cout << "OG pos: " << glm::to_string(light->position) << std::endl;
                        //std::cout << "new pos: " << glm::to_string(light_pos) << std::endl;
                        
                        // Check if the shadow ray hits another object (ie. the light is blocked)
                        if(find_intersection(shadow_ray, root, shadow_inter) &&
                           (glm::length(shadow_inter.inter_point - shadow_ray.origin) < glm::length(light_pos - shadow_ray.origin))){
                            // Don't add the colour for this light source since it is blocked by an object in front of the light source
                            continue;
                        }
                        
                        //std::cout << "old shade_colour: " << glm::to_string(shade_colour) << std::endl;
                        shade_colour = shade_colour + determine_lighting(r, intersection, light, material, light_pos);
                        //std::cout << "new shade_colour: " << glm::to_string(shade_colour) << std::endl;
                    }
                    
                    //std::cout << "old colour_vec: " << glm::to_string(colour_vec) << std::endl;
                    // Add the final shade colour to colour vec (average the final shade colour)
                    colour_vec = colour_vec + (1.0 / lights.size()) * shade_colour * (1.0 / soft_rays);
                    //std::cout << "new colour_vec: " << glm::to_string(colour_vec) << std::endl;
                    //exit(1);
                    
                } else {
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
                    colour_vec = colour_vec + determine_lighting(r, intersection, light, material, light->position);
                }
            }
        }
        
        // Define a reflective colour. Black will suffice
        glm::vec3 reflected_colour(0.0, 0.0, 0.0);
        //std::cout << "reflect count and glossy: " << reflect_count << " " << glossy_rays << std::endl;
        // Check if we have any more reflective rays to send
        if (!material->zero_ks() && reflect_count > 0){
            
            Ray reflective_ray = {p, r.direction - 2*glm::dot(r.direction, intersection.inter_normal)*intersection.inter_normal};
            
            if (glossy_rays > 0){
                // For each glossy_rays, perturb the ray and send it out to get the colour
                // Final reflective colour is all the colours summed together and divided by glossy_rays
                int created_rays = glossy_rays;
                for (int g=0; g < glossy_rays; g++){
                    Ray glossy_ray = get_glossy_ray(reflective_ray, material->get_shininess());
                    //std::cout << "glossy ray: " << glm::to_string(glossy_ray.direction) << std::endl;
                    //std::cout << "glossy dot: " << glm::dot(glossy_ray.direction, intersection.inter_normal) << std::endl;
                    if (glm::dot(glossy_ray.direction, intersection.inter_normal) > 0){
                        reflected_colour = reflected_colour + ray_colour(glossy_ray, reflected_colour, root, ambient, lights, reflect_count - 1, refract_count, glossy_rays, soft_rays);
                    } else{
                        // Ray went below surface - so dont count it in average
                        if (created_rays > 0){
                            created_rays -- ;
                        }
                    }
                    
                }
                
                if (reflected_colour != glm::vec3(0.0, 0.0, 0.0)){
                    //std::cout << "glossy: " << created_rays << std::endl;
                    //std::cout << "reflected colour glossy: " << glm::to_string(reflected_colour) << std::endl;
                }
                
                reflected_colour = reflected_colour * (1.0 / created_rays);
            } else {
                reflected_colour = ray_colour(reflective_ray, reflected_colour, root, ambient, lights, reflect_count - 1, refract_count, glossy_rays, soft_rays);
            }
        }
        
        double fresnel_R = 1.0;
        double ni, nt ;
        glm::vec3 refracted_colour(0.0, 0.0, 0.0);
        // Refraction
        if (material->get_index() > 0.0 && refract_count > 0){
            
            /*
            glm::vec3 norm = intersection.inter_normal;
            double in_out = glm::dot(r.direction, norm);
            double n1 = 1.0;
            double n2 = 1.0;
            
            // If the dot product is positive, then exiting object
            // Else, entering
            if (in_out >= 0) {
                n1 = material->get_index();
                norm = (-1)*norm;
            } else {
                n2 = material->get_index();
                //in_out = (-1)*in_out;
            }
            
            double nr = n1/n2;
            in_out = glm::dot(r.direction, norm);
            double under_root = 1.0 - pow(nr, 2) * (1.0 - pow(in_out,2));
            
            // Check if under_root is negative. If so, then we have total internal reflection
            // and thus should not shoot a relfected ray.
            if (under_root >= 0){
                double rooted = sqrt(under_root);
                Ray refractive_ray = {intersection.inter_point - (1e-2)*norm, ((-1)*nr*in_out - rooted)*norm + nr*r.direction};
                //t_direction = ((-1)*n*vndot - sqrt(bigugly))*normal + (n*incident.direction);
                refracted_colour = ray_colour(refractive_ray, refracted_colour, root, ambient, lights, reflect_count, reflect_count - 1);
                // Get the Fresnel coefficient R to calculate the final colour
             
            }
             */
         
            bool totalInternalReflection = false;
            //std::cout << "TEST" << std::endl;
            Ray refractive_ray = get_refracted_ray(r, intersection, material->get_index(), totalInternalReflection, fresnel_R, ni, nt);
            //std::cout << "after: " << totalInternalReflection << std::endl;
            if (!totalInternalReflection) {
                //std::cout << "not it" << std::endl;
                refracted_colour = ray_colour(refractive_ray, refracted_colour, root, ambient, lights, reflect_count, refract_count - 1, glossy_rays, soft_rays);
                //colour_vec = colour_vec + refracted_colour;
                //std::cout << "fresnel: " << fresnel_R << std::endl;
            }
            //std::cout << "not it2" << std::endl;
        }
        
        // Add the reflection and refraction colour components, scaling them by the Fresnel coefficient
        // Note: If material has no shininess, ks will be 0.0 and thus no reflection/refraction will count
        // Note 2: If either reflected_colour or refracted_colour stay as 0.0 then that colour component will not be
        // in the final colour
        
        //std::cout << "colour_vec: " << glm::to_string(colour_vec) << std::endl;
        //return colour_vec + material->get_ks() * ((fresnel_R * reflected_colour) + ((1.0 - fresnel_R) * refracted_colour));
        return colour_vec + material->get_ks() * reflected_colour + refracted_colour;
        //return colour_vec + refracted_colour;
        //return colour_vec;)
        //return colour_vec + (1.0 / lights.size()) * reflected_colour * material->get_ks();
    }

    return bg;
}

bool colour_threshold(float c1, float c2, float c3, float c4, float threshold){
    
    if ((std::abs(c1 - c2) > threshold || std::abs(c1 - c3) > threshold || std::abs(c1 - c4) > threshold) ||
        (std::abs(c2 - c3) > threshold || std::abs(c2 - c4) > threshold) ||
        (std::abs(c3 - c4) > threshold)) {
        // The differerence in colour is too much
        return true ;
    } else {
        return false;
    }
}


glm::vec3 anti_alias_render(double x, double y, glm::mat4 pixel_to_world, const glm::vec3 & eye, glm::vec3 bg, SceneNode *root, const glm::vec3 & ambient, const std::list<Light *> & lights, int reflect_count, int refract_count, int glossy_rays, int soft_rays, int alias_count, double threshold, bool highlight, double pixel_size, double weight){
    
    double s_div = pixel_size * 0.25 ;
    double l_div = pixel_size * 0.75 ;

    
    if (x + l_div > x+1 || y + l_div > y+1){
        std::cout << "ERROR: Encroched on another pixel!" << std::endl;
        exit(1);
    }
    
    // Super sampling - sample four corners of pixel instead of just top left corner
    glm::vec4 pixel_tl = glm::vec4(x+s_div, y+s_div, 0.0, 1.0);
    glm::vec4 pixel_tr = glm::vec4(x+l_div, y+s_div, 0.0, 1.0);
    glm::vec4 pixel_bl = glm::vec4(x+s_div, y+l_div, 0.0, 1.0);
    glm::vec4 pixel_br = glm::vec4(x+l_div, y+l_div, 0.0, 1.0);
    
    // Get pixels in world coords
    glm::vec3 p_tl = glm::vec3(pixel_to_world * pixel_tl);
    glm::vec3 p_tr = glm::vec3(pixel_to_world * pixel_tr);
    glm::vec3 p_bl = glm::vec3(pixel_to_world * pixel_bl);
    glm::vec3 p_br = glm::vec3(pixel_to_world * pixel_br);
    
    // Create four rays
    Ray ray_tl = {eye, glm::normalize(p_tl - eye)};
    Ray ray_tr = {eye, glm::normalize(p_tr - eye)};
    Ray ray_bl = {eye, glm::normalize(p_bl - eye)};
    Ray ray_br = {eye, glm::normalize(p_br - eye)};
    
    // Get 4 colours
    glm::vec3 colour_tl = ray_colour(ray_tl, bg, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays);
    glm::vec3 colour_tr = ray_colour(ray_tr, bg, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays);
    glm::vec3 colour_bl = ray_colour(ray_bl, bg, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays);
    glm::vec3 colour_br = ray_colour(ray_br, bg, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays);
    
    // Check if r, g, or b difference is too much (over threshold)
    if ((colour_threshold(colour_tl.x, colour_tr.x, colour_bl.x, colour_br.x, threshold) ||
        colour_threshold(colour_tl.x, colour_tr.x, colour_bl.x, colour_br.x, threshold) ||
        colour_threshold(colour_tl.x, colour_tr.x, colour_bl.x, colour_br.x, threshold)) &&
        alias_count > 0)
    {
        if (highlight){
            return glm::vec3(1.0, 0.0, 0.0);
        } else {
            // Sample again
            double new_pixel_size = pixel_size / 2.0;
            glm::vec3 c1 = anti_alias_render(x, y, pixel_to_world, eye, bg, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays, alias_count-1, threshold, highlight, new_pixel_size, weight/4.0);
            glm::vec3 c2 = anti_alias_render(x+new_pixel_size, y, pixel_to_world, eye, bg, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays, alias_count-1, threshold, highlight, new_pixel_size, weight/4.0);
            glm::vec3 c3 = anti_alias_render(x, y+new_pixel_size, pixel_to_world, eye, bg, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays, alias_count-1, threshold, highlight, new_pixel_size, weight/4.0);
            glm::vec3 c4 = anti_alias_render(x+new_pixel_size, y+new_pixel_size, pixel_to_world, eye, bg, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays, alias_count-1, threshold, highlight, new_pixel_size, weight/4.0);
            return c1 + c2 + c3 + c4 ;
        }
    } else {
        //std::cout << "x, y: " << x+s_div << "," << y+s_div << " " << weight << " * " << glm::to_string(colour_tl) << " " << "x, y: " << x+l_div << "," << y+s_div << " " << weight << " * " << glm::to_string(colour_tr) << " " << "x, y: " << x+s_div << "," << y+l_div << " " << weight << " * " << glm::to_string(colour_bl) << " " << "x, y: " << x+l_div << "," << y+l_div << " " << weight << " * " << glm::to_string(colour_br);
        return weight * colour_tl + weight * colour_tr + weight * colour_bl + weight * colour_br;
    }
}

void thread_render(SceneNode *root, Image & image, const glm::vec3 & eye, const glm::vec3 & view, const glm::vec3 & up, double fovy, const glm::vec3 & ambient, const std::list<Light *> & lights, glm::mat4 pixel_to_world, int start_x, int end_x, int start_y, int end_y, int thread_num, int y_skip, int reflect_count, int refract_count, int glossy_rays, int soft_rays, int alias_count, double threshold, bool highlight, bool anti) {
    
    int total_pixels = image.width() * image.height()/y_skip;
    int current_pixel = 1;
    int current_percent = 0;
    
    srand((unsigned int)time(NULL));
    
    for (int y = start_y; y < end_y; y += y_skip) {
        for (int x = start_x; x < end_x; x++) {
            
            if (int(floorf((float)current_pixel / (float)total_pixels*100))%5 == 0){
                if (current_percent != int(floorf((float)current_pixel / (float)total_pixels*100))){
                    std::cout << "Thread " << thread_num+1 << " Progress: " << int(floorf((float)current_pixel / (float)total_pixels*100)) << "%" << std::endl;
                    current_percent = int(floorf((float)current_pixel / (float)total_pixels*100));
                }
                
            }
            
            // Background colour
            //glm::vec3 background_col = ((y) & 0x10) ? ((double)y/image.height()*glm::vec3(0.25, 0.25, 0.25)) + glm::vec3(0.68, 0.91, 0.99) : ((image.height() - (double)y)/image.height()*glm::vec3(0.25, 0.25, 0.25)) + glm::vec3(0.68, 0.91, 0.99);
            
            glm::vec3 background_col = glm::vec3(0.0, 0.0, 0.0);
            
//#ifdef SUPERSAMPLE
            
            if (anti){
                
                //std::cout << "PIXEL: " << x << " " << y << std::endl;
                glm::vec3 pixel_colour = anti_alias_render(x, y, pixel_to_world, eye, background_col, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays, alias_count, threshold, highlight, 1.0, 0.25);
                //std::cout << std::endl ;
                
                image(x, y, 0) = pixel_colour.x;
                image(x, y, 1) = pixel_colour.y;
                image(x, y, 2) = pixel_colour.z;
                
                current_pixel++;
            } else {
            
//#else
            // Pixel to World Coords
            glm::vec4 pixel = glm::vec4(x, y, 0.0, 1.0);
            glm::vec3 p = glm::vec3(pixel_to_world * pixel);
            
            //std::cout << view.length() << std::endl;
            //std::cout << "pixel: " << glm::to_string(pixel) << std::endl;
            //std::cout << "p: " << glm::to_string(p) << std::endl;
            
        
            // Create the ray with origin at the eye point
            Ray ray = {eye, glm::normalize(p - eye)};
            
            // Cast ray into the scene and get the colour returned
            glm::vec3 colour = ray_colour(ray, background_col, root, ambient, lights, reflect_count, refract_count, glossy_rays, soft_rays);
            
            if (colour != background_col){
                //exit(1);
            }
            
            image(x, y, 0) = colour[0];
            image(x, y, 1) = colour[1];
            image(x, y, 2) = colour[2];
            
            current_pixel++;
            }
//#endif
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
		const std::list<Light *> & lights,
               
        // Ray count params
        const int num_threads,
        const int reflect_rays,
        const int refract_rays,
        const int glossy_rays,
        const int soft_rays,
               
        // Adaptive AA params
        const int alias_count,
        const float threshold,
        const bool highlight,
        const bool anti
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
		  "\t" << "lights{" << std::endl <<
          "\t" << "highlight? : " << highlight << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

    glm::mat4 pixel_to_world = world_pixels(image.width(), image.height(), eye, fovy, view, up);
    
    std::cout << glm::to_string(pixel_to_world) << std::endl;
    
    if (anti){
        #define SUPERSAMPLE
    }
    
    std::vector<std::thread> threads(num_threads);
    for(int i = 0; i < num_threads; i++)
    {
        threads[i] = std::thread(thread_render, root, std::ref(image), eye, view, up, fovy, ambient, lights, pixel_to_world, 0, image.width(), i, image.height(), i, num_threads, reflect_rays, refract_rays, glossy_rays, soft_rays, alias_count, threshold, highlight, anti);
        //threads[i] = std::thread(test_func, root, std::ref(image));
        if(threads[i].get_id() == std::thread::id())
        {
            std::cerr << "Abort: Failed to create thread " << i << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    
    for(unsigned int i = 0; i < num_threads; i++) threads[i].join();
}
