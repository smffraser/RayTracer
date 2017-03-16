#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);
  virtual ~PhongMaterial();
    glm::vec3 get_ks() const;
    glm::vec3 get_kd() const;
    double get_shininess() const;
    bool zero_ks() const;

private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;

};
