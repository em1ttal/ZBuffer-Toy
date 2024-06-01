#pragma once

#include <library/Common.h>
#include <QGLShaderProgram>

#include "GPUConnections/GPULightFactory.hh"
#include "GPUConnections/GPUCamera.hh"
#include "DataService/SetUp.hh"

using namespace std;
#define MAX_LIGHTS 5
class GPUSetUp : public SetUp
{
public:
    GPUSetUp();

    shared_ptr<GPUCamera>             getCamera();
    std::vector<shared_ptr<GPULight>> getLights();
    vec3                              getGlobalLight();
    float getTime();


    void setCamera(shared_ptr<GPUCamera> cam);
    void setGlobalLight(vec3 globalLight);
    void setLights(std::vector<shared_ptr<GPULight> > lights);
    void setTime(float time);

    virtual void read (const QJsonObject &json) override;
    virtual void write (QJsonObject &json) const override;
    virtual void print (int indentation) const override;

    virtual ~GPUSetUp() {};

    // TODO Pràctica 2: FASE 1: Enviar les llums a la GPU

    void lightsToGPU(shared_ptr<QGLShaderProgram> program);
    void setAmbientGlobalToGPU(shared_ptr<QGLShaderProgram> program);
    void setTimeToGPU(shared_ptr<QGLShaderProgram> program, float t);

private:

    // Camera
    shared_ptr<GPUCamera> camera;

    // TODO Pràctica 2: FASE 1: Afegir llums a l'escena
    // Vector de llums de l'escena
    std::vector<shared_ptr<GPULight>> lights;
    // Llum global
    vec3 globalLight;
    float time;

    shared_ptr<GPULight> getLightActual();
    void setLightActual(shared_ptr<GPULight> l);
    void addLight(shared_ptr<GPULight> l);



};

