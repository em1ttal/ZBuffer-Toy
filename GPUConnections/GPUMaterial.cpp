#include "GPUConnections/GPUMaterial.hh"

GPUMaterial::GPUMaterial(){
    Ka = vec3(0.2, 0.2, 0.2);
    Kd = vec3(0.8, 0.5, 0.4);
    Ks = vec3(1.0, 1.0, 1.0);
    shininess = 20.0f;
    this->opacity = 1.0f;
}

GPUMaterial::~GPUMaterial()
{}

GPUMaterial::GPUMaterial(vec3 d):Material(d){
    this->Kd = d;
    Ka = vec3(0.2, 0.2, 0.2);
    Ks = vec3(1.0, 1.0, 1.0);
    shininess = 20.0f;
    this->opacity = 1.0f;
}

GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shin):Material(a,d,s,shin) {
    this->Ka = a;
    this->Kd = d;
    this->Ks = s;
    this->shininess = shin;
    this->opacity = 1.0f;
}

GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shin, float opac):Material(a,d,s,shin,opac) {
    this->Ka = a;
    this->Kd = d;
    this->Ks = s;
    this->shininess = shin;
    this->opacity = opac;
}

void GPUMaterial::toGPU(shared_ptr<QGLShaderProgram> p) {
    gl_materials.Ka = p->uniformLocation("material.Ka");
    gl_materials.Kd = p->uniformLocation("material.Kd");
    gl_materials.Ks = p->uniformLocation("material.Ks");
    gl_materials.opacity = p->uniformLocation("material.opacity");
    gl_materials.shininess = p->uniformLocation("material.shininess");

    //valores enviados a la gpu
    glUniform3fv(gl_materials.Ka, 1, this->Ka);
    glUniform3fv(gl_materials.Kd, 1, this->Kd);
    glUniform3fv(gl_materials.Ks, 1, this->Ks);

    glUniform1f(gl_materials.opacity, this->opacity);
    glUniform1f(gl_materials.shininess, this->shininess);

}