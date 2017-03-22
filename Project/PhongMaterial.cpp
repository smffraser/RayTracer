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

glm::vec3 PhongMaterial::get_kd() const {
    // Change this!
    
    if (m_texture.has_data()) {
        // This means that the material has a texture and the diffuse
        // values need to be pulled from the texture data
        return m_kd;
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

