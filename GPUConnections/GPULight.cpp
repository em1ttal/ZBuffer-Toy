#include "GPUConnections/GPULight.hh"

GPULight::GPULight(vec3 Ia, vec3 Id, vec3 Is): Light(Ia, Id, Is){
}

GPULight::GPULight() {
}

void GPULight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: enviar les propietats de Ia, Id i Is a la GPU
    struct lightProperties {
        GLuint Ia;
        GLuint Id;
        GLuint Is;
    };
    lightProperties properties;
    properties.Ia = p->uniformLocation(QString("light[%1].Ia").arg(getVecPos()));
    properties.Id = p->uniformLocation(QString("light[%1].Id").arg(getVecPos()));
    properties.Is = p->uniformLocation(QString("light[%1].Is").arg(getVecPos()));
    glUniform3fv(properties.Ia, 1, getIa());
    glUniform3fv(properties.Id, 1, getId());
    glUniform3fv(properties.Is, 1, getIs());
}
