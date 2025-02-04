#pragma once

#include <library/Common.h>
#include <QGLShaderProgram>
#include <memory>
#include "GPUConnections/GPUConnectable.hh"
#include "VW/Lights/Light.hh"

using namespace std;

class GPULight: public Light, public GPUConnectable {

public:
    /*
     * Constructor de la classe Light.
     * param Ia: component ambient de la llum.
     * param Id: component difosa de la llum.
     * param Is: component especular de la llum.
     * */
    GPULight(vec3 Ia, vec3 Id, vec3 Is);
    GPULight();
    virtual ~GPULight() {};

    //Calcula el factor d'atenuacio de la llum al punt passat per parametre
    virtual float attenuation(vec3 point) override = 0;
    //Calcula el vector L amb origen el punt passat per parametre
    virtual vec3 vectorL(vec3 point) override = 0;
    //Calcula la distancia del punt a la llum
    virtual float distanceToLight(vec3 point) override = 0;

    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;

    // Getter for vec_pos
    int getVecPos() const { return vec_pos; }

    // Setter for vec_pos
    void setVecPos(int pos) { vec_pos = pos; }

    vec4 getLightPosition() const { return lightPosition; }
    void setLightPosition(vec4 pos) { lightPosition = pos; }

    vec3 getCoeficients() const { return coeficients; }
    void setCoeficients(vec3 coef) { coeficients = coef; }

private:
    int vec_pos;
    vec4 lightPosition;
    vec3 coeficients;
};

