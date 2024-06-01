#include "GPUMesh.hh"

GPUMesh::GPUMesh()
{
	numPoints = NUMPOINTS;
	points = new vec4[numPoints];
	normals= new vec4[numPoints];
	colors = new vec4[numPoints];
    textureCoords = new vec2[numPoints];
    material = make_shared<GPUMaterial>();    
	make();
}

GPUMesh::GPUMesh(const QString &fileName): Mesh(fileName)
{
    numPoints = NUMPOINTS;
    points = new vec4[numPoints];
    normals= new vec4[numPoints];
    colors = new vec4[numPoints];
    textureCoords = new vec2[numPoints];
    material = make_shared<GPUMaterial>();
    make();
}

GPUMesh::GPUMesh(const int npoints, const QString &fileName): Mesh(fileName)
{
    numPoints = npoints;
    points = new vec4[numPoints];
    normals= new vec4[numPoints];
    colors = new vec4[numPoints];
    textureCoords = new vec2[numPoints];
    material = make_shared<GPUMaterial>();
    make();
}

void GPUMesh::read(const QJsonObject &json) {
    numPoints = NUMPOINTS;
    points = new vec4[numPoints];
    normals= new vec4[numPoints];
    colors = new vec4[numPoints];
    textureCoords = new vec2[numPoints];
    if(json.contains("material")){
        QJsonObject materialJson = json["material"].toObject();
        material->read(materialJson);
    } else {
        material = make_shared<GPUMaterial>();
    }
    Mesh::read(json);
    if(json.contains("texFileName")){
        QString textureJson = json["texFileName"].toString();
        this->setTexture(make_shared<QOpenGLTexture>(QImage(textureJson).mirrored()));
    }
    make();
}

/**
 * @brief GPUMesh::~GPUMesh()
 */
GPUMesh::~GPUMesh() {

    if (points!= nullptr) delete points;
    if (normals!= nullptr) delete normals;
    if (colors!= nullptr) delete colors;
    if (textureCoords!= nullptr) delete textureCoords;
}

/**
 * @brief GPUMesh::toGPU
 * @param pr
 */
void GPUMesh::toGPU(shared_ptr<QGLShaderProgram> pr) {
    // TO  DO: A modificar a la fase 1 de la practica 2

    qDebug() << "Obj to GPU.....";

    program = pr;
    // Creació d'un vertex array object

    glGenVertexArrays( 1, &vao );

    // Creacio i inicialitzacio d'un vertex buffer object (VBO)
    glGenBuffers( 1, &buffer );

    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal passar les normals a la GPU
    material->toGPU(pr);
    program->setUniformValue("texMap", 0);
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*Index*3+sizeof(vec2)*Index, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*Index, points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*Index, sizeof(vec4)*Index, colors );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*Index * 2, sizeof(vec4)*Index, normals);  
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*Index * 3, sizeof(vec2)*Index, textureCoords);
    // set up vertex arrays
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(vec4)*Index));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(vec4)*Index*2));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(vec4)*Index*3));
    glEnableVertexAttribArray(3);
    // Enable de texturas 2D
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_TEXTURE_2D);
}


/**
 * Pintat en la GPU.
 * @brief GPUMesh::draw
 */
void GPUMesh::draw(){

    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object.
    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal activar també les normals  a la GPU

    if (auto gpuMaterial = dynamic_pointer_cast<GPUMaterial>(this->material)) {
        // Si el casting fue exitoso, llamamos a toGPU()
        gpuMaterial->toGPU(program);
    } else {
        // Si el casting falló, imprimir un mensaje de advertencia
        cerr << "El material no es una instancia de GPUMaterial." << endl;
        // También podrías implementar un manejo de error adecuado aquí
    }

    glBindVertexArray( vao );
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

/**
 * @brief GPUMesh::make
 */
void GPUMesh::make(){

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal calcular la normal a cada vertex a la CPU

    static vec3  base_colors[] = {
        vec3( 1.0, 0.0, 0.0 ),
        vec3( 0.0, 1.0, 0.0 ),
        vec3( 0.0, 0.0, 1.0 ),
        vec3( 1.0, 1.0, 0.0 )
    };

    Index = 0;
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]];
            colors[Index] = vec4(base_colors[j%4], 1.0);
            normals[Index] = normalsVertexs[cares[i].idxNormals[j]];
            if (!textVertexs.empty()) {
                textureCoords[Index] = textVertexs[cares[i].idxTextures[j]];
            }
            Index++;
        }
	}
}


/**
 * @brief GPUMesh::initTexture
 */
void GPUMesh::initTexture()
 {
    // TO DO: A implementar a la fase 1 de la practica 2
    // Cal inicialitzar la textura de l'objecte: veure l'exemple del CubGPUTextura
    qDebug() << "Initializing textures...";
    glActiveTexture(GL_TEXTURE0);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->bind(0);
 }

void GPUMesh::setTexture(shared_ptr<QOpenGLTexture> t){
   texture = t;
   initTexture();
}

Capsa3D GPUMesh::calculCapsa3D()
{
    vec3    pmin, pmax;
    int     i;
    Capsa3D capsa;

    pmin.x = points[0].x;
    pmin.y = points[0].y;
    pmin.z = points[0].z;
    pmax = pmin;
	for(i=1; i<Index; i++) {
        if(points[i].x <pmin[0])
            pmin[0] = points[i].x;
        if(points[i].y <pmin[1])
            pmin[1] = points[i].y;
        if(points[i].z <pmin[2])
            pmin[2] = points[i].z;

        if(points[i].x >pmax[0])
            pmax[0] = points[i].x;
        if(points[i].y >pmax[1])
            pmax[1] = points[i].y;
        if(points[i].z >pmax[2])
            pmax[2] = points[i].z;
    }
    capsa.pmin = pmin;
    capsa.a = pmax[0]-pmin[0];
    capsa.h = pmax[1]-pmin[1];
    capsa.p = pmax[2]-pmin[2];
    return capsa;
}
