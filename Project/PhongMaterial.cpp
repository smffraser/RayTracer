#include "PhongMaterial.hpp"
#include <iostream>

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double ref_index )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
    , m_ref_index(ref_index)
   , m_texture(Image())
   , m_bump(Image())
{}

PhongMaterial::~PhongMaterial()
{}

glm::vec3 PhongMaterial::get_ks() const {
    return m_ks;
}

glm::vec3 PhongMaterial::get_kd(double u, double v) const{
    // Change this!
    
    if (m_texture.has_data()) {
        
        //std::cout << "u: " << u << " v: " << v << std::endl;
        
        // This means that the material has a texture and the diffuse
        // values need to be pulled from the texture data
        
        // Find d_i and d_j for u, v values
        double d_i = (double)(m_texture.width() - 1)*u;
        //double d_j = (double)(m_texture.height() - 1) - (double)(m_texture.height() - 1)*v;
        double d_j = (double)(m_texture.height() - 1)*v;
        
        // Get the u, v values for the pixel
        int i = (int)d_i;
        int j = (int)d_j;
        float u_p = d_i - i;
        float v_p = d_j - j;
        
        // Wrap around to other side if the coordinates are out of bounds of the texture file
        i = (i >= m_texture.width()) ? 0 : i;
        j = (j >= m_texture.height()) ? 0 : j;
        int i_1 = ((i+1) >= m_texture.width()) ? 0 : i+1;
        int j_1 = ((j+1) >= m_texture.height()) ? 0 : j+1;
        
        // Interpolate the colours
        glm::vec3 colour_00 = glm::vec3(m_texture(i, j, 0), m_texture(i, j, 1), m_texture(i, j, 2));
        glm::vec3 colour_01 = glm::vec3(m_texture(i, j_1, 0), m_texture(i, j_1, 1), m_texture(i, j_1, 2));
        glm::vec3 colour_10 = glm::vec3(m_texture(i_1, j, 0), m_texture(i_1, j, 1), m_texture(i_1, j, 2));
        glm::vec3 colour_11 = glm::vec3(m_texture(i_1, j_1, 0), m_texture(i_1, j_1, 1), m_texture(i_1, j_1, 2));
        
        return colour_00*(1-u_p)*(1-v_p) + colour_01*(1-u_p)*(v_p) + colour_10*(u_p)*(1-v_p) + colour_11*u_p*v_p;
        
    } else {
        return m_kd;
    }
}

double PhongMaterial::B(double u, double v) const{
    // Find d_i and d_j for u, v values
    double d_i = (double)(m_bump.width() - 1)*u;
    double d_j = (double)(m_bump.height() - 1)*v;
    
    // Get the u, v values for the pixel
    int i = (int)d_i;
    int j = (int)d_j;
    float u_p = d_i - i;
    float v_p = d_j - j;
    
    // Wrap around to other side if the coordinates are out of bounds of the bump file
    i = (i >= m_bump.width()) ? 0 : i;
    j = (j >= m_bump.height()) ? 0 : j;
    int i_1 = ((i+1) >= m_bump.width()) ? 0 : i+1;
    int j_1 = ((j+1) >= m_bump.height()) ? 0 : j+1;
    
    // Interpolate the disturbance
    //std::cout << "its gonna be here. " << std::endl;
    //double colour_00 = (m_bump(i, j, 0) + m_bump(i, j, 1) + m_bump(i, j, 2)) / 3.0 ;
    //double colour_01 = (m_bump(i, j_1, 0), m_bump(i, j_1, 1), m_bump(i, j_1, 2)) / 3.0;
    //double colour_10 = (m_bump(i_1, j, 0), m_bump(i_1, j, 1), m_bump(i_1, j, 2)) / 3.0;
    //double colour_11 = (m_bump(i_1, j_1, 0), m_bump(i_1, j_1, 1), m_bump(i_1, j_1, 2)) / 3.0;
    //std::cout << "its gonna be before this. " << std::endl;
    //std::cout << "bump width and height: " << bump_width << " " << bump_width << std::endl;
    //std::cout << "i, j, i_1, j_1: " << i << " " << j << " " << i_1 << " " << j_1 << std::endl;
    
    double colour_00 = m_bump(i, j, 0);
    double colour_01 = m_bump(i, j_1, 0);
    double colour_10 = m_bump(i_1, j, 0);
    double colour_11 = m_bump(i_1, j_1, 0);

    //std::cout << "00: " << colour_00 << " 01: " << colour_01 << " 10: " << colour_10 << " 11: " << colour_11 << std::endl;
    
    return colour_00*(1-u_p)*(1-v_p) + colour_01*(1-u_p)*(v_p) + colour_10*(u_p)*(1-v_p) + colour_11*u_p*v_p;
}

glm::vec3 PhongMaterial::get_bumped_normal(double u, double v, glm::vec3 x, glm::vec3 y, glm::vec3 og_normal) const{
    
    if (!m_bump.has_data()){
        return og_normal;
    }
    
    float e = 1.0f/512.0f;
    //double e = std::numeric_limits<double>::epsilon();
    
    //std::cout << "u: " << u << " v: " << v << std::endl;
    //std::cout << "B(u+e, v): " << B(u+e, v) << " B(u-e, v): " << B(u-e, v) << std::endl;
    //std::cout << "B(u, v+e): " << B(u, v+e) << " B(u, v-e): " << B(u, v-e) << std::endl;
    
    
    float B_u = (B(u+e,v) - B(u-e, v)) / (2*e) ;
    float B_v = (B(u,v+e) - B(u, v-e)) / (2*e) ;
    
    //std::cout << "B_u: " << B_u << " B_v: " << B_v << std::endl;
    
    glm::vec3 D = 0.05f*(B_v * glm::cross(og_normal, x) + B_u * glm::cross(og_normal, y)) ;
    
    return glm::normalize(og_normal + D) ;
}

double PhongMaterial::get_shininess() const {
    return m_shininess;
}

bool PhongMaterial::zero_ks() const {
    return m_ks == glm::vec3(0.0, 0.0, 0.0);
}

double PhongMaterial::get_index() const {
    return m_ref_index;
}

bool PhongMaterial::zero_kd() const {
    // Only have zero_kd when kd is zero vector AND no texture is set
    return m_kd == glm::vec3(0.0, 0.0, 0.0) && !m_texture.has_data();
}

void PhongMaterial::set_texture(Image texture){
    m_texture = texture ;
}

void PhongMaterial::set_bump(Image bump){
    m_bump = bump ;
}
