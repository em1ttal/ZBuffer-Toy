#pragma once

#include <library/Common.h>
#include <memory>
using namespace Common;

#include <QOpenGLTexture>

#include "GPUConnections/GPUConnectable.hh"
#include "VW/Objects/Mesh.hh"
#include "GPUConnections/GPUMaterial.hh"

static int NUMPOINTS = 40000;

class GPUMesh : public Mesh, public GPUConnectable, public QObject
{
public:
	GPUMesh();
    GPUMesh(const QString &fileName);
    GPUMesh(const int npoints, const QString &fileName);

    ~GPUMesh();

    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;
    virtual void draw() override;
    Capsa3D calculCapsa3D();

    void setTexture(shared_ptr<QOpenGLTexture> t);

    void read(const QJsonObject &json) override;
private:
    // Estructures per passar a la GPU
    GLuint buffer;
    GLuint vao;

    // Representació explícita de l'objecte per poder ser passat a la GPU
    int   numPoints;
    vec4 *points;
    vec4 *normals;
    vec2 *textureCoords;

    // Els colors s'usen en la primera execució però després són prescindibles
    vec4 *colors;

    int Index; // index de control del numero de vèrtexs a passar a la GPU

    shared_ptr<GPUMaterial> material;
    shared_ptr<QOpenGLTexture> texture;

    void make();

    void initTexture();
};

