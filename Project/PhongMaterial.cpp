#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double ref_index )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
    , m_ref_index(ref_index)
{}

PhongMaterial::~PhongMaterial()
{}

glm::vec3 PhongMaterial::get_ks() const {
    return m_ks;
}

glm::vec3 PhongMaterial::get_kd() const {
    return m_kd;
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
    return m_kd == glm::vec3(0.0, 0.0, 0.0);
}
