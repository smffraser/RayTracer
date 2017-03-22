#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double ref_index )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
    , m_ref_index(ref_index)
   , m_texture(Image())
{}

PhongMaterial::~PhongMaterial()
{}

glm::vec3 PhongMaterial::get_ks() const {
    return m_ks;
}

glm::vec3 PhongMaterial::get_kd(double u, double v) const{
    // Change this!
    
    if (m_texture.has_data()) {
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

