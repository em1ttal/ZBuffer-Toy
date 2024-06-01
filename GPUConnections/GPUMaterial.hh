#pragma once

#include "DataService/Serializable.hh"
#include "DataService/SetUp.hh"

#include "GPUConnections/GPUConnectable.hh"
#include "VW/Materials/Material.hh"
using namespace std;

class GPUMaterial: public Material, public GPUConnectable
{
public:

    GPUMaterial();
    GPUMaterial(vec3 d);
    GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess);
    GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess, float opacity);
    ~GPUMaterial();

    void toGPU(shared_ptr<QGLShaderProgram> p) override;

    virtual bool scatter(const Ray& r_in, int t, vec3& color, Ray & r_out) const {
        return false; // Por ahora devuelve false
    }

    virtual vec3 getDiffuse(vec2 point) const {
        return vec3(0.0); // Por ahora color arbitrario
    }

private:
    struct gl_material{
        GLuint Ka;
        GLuint Kd;
        GLuint Ks;
        GLuint shininess;
        GLuint opacity;
    };

    gl_material gl_materials;
};