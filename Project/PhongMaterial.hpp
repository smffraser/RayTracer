#pragma once

#include <glm/glm.hpp>
#include "Image.hpp"

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double ref_index);
  virtual ~PhongMaterial();
    glm::vec3 get_ks() const;
    glm::vec3 get_kd(double u, double v) const;
    double get_shininess() const;
    double get_index() const;
    bool zero_ks() const;
    bool zero_kd() const;
    void set_texture(Image texture);

private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;
  Image m_texture;

  double m_shininess;
  double m_ref_index;

};
