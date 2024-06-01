#include "GPUConnections/GPUPointLight.hh"

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): PointLight(posicio, Ia, Id, Is, a, b, c) {
    // TO DO Pràctica 2: Fase 1: Pas 2: enviar les llums a la GPU
    GPULight::setIa(Ia);
    GPULight::setId(Id);
    GPULight::setIs(Is);
    GPULight::setLightPosition(posicio);
    GPULight::setCoeficients(vec3(a, b, c));
}

void GPUPointLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: Pas 2: enviar les llums a la GPU
    GPULight::toGPU(p);
    vec3 coefficients = GPULight::getCoeficients();
    vec4 position = GPULight::getLightPosition();

    GLuint coe = p->uniformLocation(QString("light[%1].coefficients").arg(getVecPos()));
    GLuint pos = p->uniformLocation(QString("light[%1].position").arg(getVecPos()));

    glUniform3fv(coe, 1, coefficients);
    glUniform4fv(pos, 1, position);
}

vec3 GPUPointLight::vectorL(vec3 point) {
    return PointLight::vectorL(point);
}

float GPUPointLight::attenuation(vec3 point) {
    return PointLight::attenuation(point);
}

float GPUPointLight::distanceToLight(vec3 point) {
    return PointLight::distanceToLight(point);
}
