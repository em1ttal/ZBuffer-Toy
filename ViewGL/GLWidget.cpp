#include <QTimer>
#include <QTime>
#include "ViewGL/GLWidget.hh"


GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

    setFocusPolicy( Qt::StrongFocus );
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GLWidget::updateAnimation);
    timer->start(16);
}

GLWidget::GLWidget(const QGLFormat &glf, QWidget *parent) : QGLWidget(glf, parent) {

    setFocusPolicy( Qt::StrongFocus );
    auto cameraGPU = make_shared<GPUCamera>(this->size().width(), this->size().height());
    Controller::getInstance()->getSetUp()->setCamera(cameraGPU);
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GLWidget::updateAnimation);
    timer->start(50);
}


GLWidget::~GLWidget() {

}


/* Interacció amb GL i inicialització dels programes a la GPU */

/**
 * @brief GLWidget::initializeGL()
 */
void GLWidget::initializeGL() {

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_RGBA);
    glEnable(GL_DOUBLE);

    initShadersGPU();

    // TO DO TUTORIAL 1: Inicialitzar la llum global i passar-la a la GPU
    vec3 globalLight = vec3(0.3, 0.3, 0.3);
    Controller::getInstance()->getSetUp()->setGlobalLight(globalLight);
    Controller::getInstance()->getSetUp()->setAmbientGlobalToGPU(this->program[current_program]);
    
    std::vector<shared_ptr<GPULight>> ligths;
    auto l = GPULightFactory::getInstance().createLight(LightFactory::POINTLIGHT);
    l->setIa(vec3(0.2, 0.2, 0.2));
    l->setId(vec3(0.8, 0.8, 0.8));
    l->setIs(vec3(1.0, 1.0, 1.0));
    l->setCoeficients(vec3(0.5, 0.0, 0.01));
    l->setLightPosition(vec4(10.0, 10.0, 20.0, 1.0));
    ligths.push_back(l);
    
    auto l2 = GPULightFactory::getInstance().createLight(LightFactory::POINTLIGHT);
   l->setIa(vec3(0.2, 0.2, 0.2));
    l->setId(vec3(0.8, 0.8, 0.8));
    l->setIs(vec3(1.0, 1.0, 1.0));
    l2->setCoeficients(vec3(0.5, 0.0, 0.01));
    l2->setLightPosition(vec4(-10.0, -4.0, 5.0, 1.0));
    ligths.push_back(l2);

    auto l3 = GPULightFactory::getInstance().createLight(LightFactory::POINTLIGHT);
    l->setIa(vec3(0.2, 0.2, 0.2));
    l->setId(vec3(0.8, 0.8, 0.8));
    l->setIs(vec3(1.0, 1.0, 1.0));
    l3->setCoeficients(vec3(0.5, 0.0, 0.01));
    l3->setLightPosition(vec4(24, -36, -18.3, 1.0));
    ligths.push_back(l3);

    Controller::getInstance()->getSetUp()->setLights(ligths);
    
    // TODO TUTORIAL 1: Passar primer una llum  a la GPU i després totes
    // Posa el codi aqui
    Controller::getInstance()->getSetUp()->lightsToGPU(this->program[current_program]);


    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();
    auto scene = Controller::getInstance()->getScene();

    camera->init(this->size().width(), this->size().height(), scene->capsaMinima);
    emit ObsCameraChanged(camera);
    emit FrustumCameraChanged(camera);

    glViewport(camera->vp.pmin[0], camera->vp.pmin[1], camera->vp.a, camera->vp.h);

}

/**
 * @brief GLWidget::paintGL()
 */
void GLWidget::paintGL() {

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();
    auto scene = Controller::getInstance()->getScene();

    camera->toGPU(program[current_program]);
    scene->draw();
}

/**
 * @brief GLWidget::resizeGL()
 */
void GLWidget::resizeGL(int width, int height) {


    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->vp.a = width;
    camera->vp.h = height;
    auto scene = Controller::getInstance()->getScene();
    camera->CalculaWindow(scene->capsaMinima);
    glViewport(camera->vp.pmin[0], camera->vp.pmin[1], camera->vp.a, camera->vp.h);
    emit FrustumCameraChanged(camera);
    updateGL();
}

/**
 * @brief GLWidget::initShadersGPU
 */
void GLWidget::initShadersGPU(){
    shaders[0] = new GLShader("://resources/GPUshaders/vshader1.glsl", "://resources/GPUshaders/fshader1.glsl", program[0]);
    shaders[1] = new GLShader("://resources/GPUshaders/vColorShader.glsl", "://resources/GPUshaders/fColorShader.glsl", program[1]);
    shaders[2] = new GLShader("://resources/GPUshaders/vNormalShader.glsl", "://resources/GPUshaders/fNormalShader.glsl", program[2]);
    shaders[3] = new GLShader("://resources/GPUshaders/vGouraudPhongShader.glsl", "://resources/GPUshaders/fGouraudPhongShader.glsl", program[3]);
    shaders[4] = new GLShader("://resources/GPUshaders/vGouraudBlinnPhongShader.glsl", "://resources/GPUshaders/fGouraudBlinnPhongShader.glsl", program[4]);
    shaders[5] = new GLShader("://resources/GPUshaders/vPhongShader.glsl", "://resources/GPUshaders/fPhongShader.glsl", program[5]);
    shaders[6] = new GLShader("://resources/GPUshaders/vBlinnPhongShader.glsl", "://resources/GPUshaders/fBlinnPhongShader.glsl", program[6]);
    shaders[7] = new GLShader("://resources/GPUshaders/vToonShader.glsl", "://resources/GPUshaders/fToonShader.glsl", program[7]);
    shaders[8] = new GLShader("://resources/GPUshaders/vTextureShader.glsl", "://resources/GPUshaders/fTextureShader.glsl", program[8]);
    shaders[9] = new GLShader("://resources/GPUshaders/vEjercicio1Shader.glsl", "://resources/GPUshaders/fEjercicio1Shader.glsl", program[9]);
    shaders[10] = new GLShader("://resources/GPUshaders/vEjercicio2Shader.glsl", "://resources/GPUshaders/fEjercicio2Shader.glsl", program[10]);
    
    current_program = 0;
    program[current_program]->link();
    program[current_program]->bind();
}

void GLWidget::updateShader() {
    Controller::getInstance()->getSetUp()->lightsToGPU(program[current_program]);
    Controller::getInstance()->getScene()->toGPU(program[current_program]);
    updateGL();
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

/** Gestio de les animacions i la gravació d'imatges ***/

void GLWidget::setCurrentFrame(){

    auto scene = Controller::getInstance()->getScene();
    scene->update(currentFrame);
    updateGL();
    this->saveFrame();
    currentFrame++;

    if (currentFrame == MAXFRAMES)
        timer->stop();

}

void GLWidget::saveFrame(){
    this->grabFrameBuffer().save("screen_output_"+QVariant(currentFrame).toString()+".jpg");
}

void GLWidget::saveImage(){
    this->grabFrameBuffer().save("screen_output_"+QVariant(currentImage).toString()+".jpg");
    currentImage++;
}



/** Metodes SLOTS que serveixen al builder per a actualitzar l'escena i els objectes */
void GLWidget::updateObject(shared_ptr<GPUMesh> obj) {

    obj->toGPU(program[current_program]);
    updateGL();
}

void GLWidget::updateScene() {
    // Control de la nova escena a la GPU
    auto sc = Controller::getInstance()->getScene();
    sc->toGPU(program[current_program]);
    Controller::getInstance()->setScene(sc);
    // Podeu utilitzar l'altre constructor de Camera per inicialitzar els
    // parametres de la camera.
    auto camera = make_shared<GPUCamera>(this->size().width(), this->size().height());
    Controller::getInstance()->getSetUp()->setCamera(camera);

    camera->actualitzaCamera(sc->capsaMinima);

    emit ObsCameraChanged(camera);
    emit FrustumCameraChanged(camera);

    updateGL();

}

/** Metodes que es criden des dels menús */

void GLWidget::saveAnimation() {
    // Comença el timer de l'animació
    timer = new QTimer(this);
    currentFrame=0;
    currentImage=0;
    connect(timer, SIGNAL(timeout()), this, SLOT(setCurrentFrame()));
    timer->start(16);

}

void GLWidget::updateAnimation() {
    // Actualitza la posició de l'objecte a cada frame
    float currentTime = static_cast<float>(QTime::currentTime().msec()) / 1000.0f;
    Controller::getInstance()->getSetUp()->setTime(currentTime);
    Controller::getInstance()->getSetUp()->setTimeToGPU(program[current_program], currentTime);

    updateGL();
}

void GLWidget::activaColorShader() {
    //TO DO: Pràctica 2: A implementar a la fase 1
    qDebug()<<"Estic a Color Shader";
    current_program = 1;
    program[current_program]->link();
    program[current_program]->bind();
    updateShader();
}


void GLWidget::activaNormalShader() {
    //TO DO: Pràctica 2: A implementar a la fase 1
    qDebug()<<"Estic a Normal Shader";
    current_program = 2;
    program[current_program]->link();
    program[current_program]->bind();
    updateShader();
}


void GLWidget::activaGouraudBlinnShader() {
    //TO DO: Pràctica 2:  implementar a la fase 1
    qDebug()<<"Estic a Gouraud - Blinn-Phong shader";
    //current_program = 3;   // Phong version
    current_program = 4;   // Blinn-Phong version
    program[current_program]->link();
    program[current_program]->bind();
    updateShader();
}

void GLWidget::activaBlinnPhongShader() {
    //TO DO: Pràctica 2:  implementar a la fase 1
    qDebug()<<"Estic a Phong - Blinn-Phong Shader";
    //current_program = 5;   // Phong version
    current_program = 6;   // Blinn-Phong version
    program[current_program]->link();
    program[current_program]->bind();
    updateShader();
}

void GLWidget::activaToonShader() {
    //TO DO: Pràctica 2:  implementar a la fase 1
    qDebug()<<"Estic a Toon";
    current_program = 7;
    program[current_program]->link();
    program[current_program]->bind();
    updateShader();
}

void GLWidget::activaTextureShader() {
    //TO DO: Pràctica 2:  implementar a la fase 2
    qDebug()<<"Afegint Textura";
    current_program = 8;
    program[current_program]->link();
    program[current_program]->bind();
    updateShader();
}

void GLWidget::activaReflection() {
   //TO DO: Pràctica 2:  implementar a la fase 2
     qDebug()<<"Estic a Reflection";
}

void GLWidget::activaEnvMapping() {
    //TO DO: Pràctica 2:  implementar a la fase 2
    qDebug()<<"Estic a Environmental Mapping";
}

void GLWidget::activaTransparency() {
    //TO DO: Pràctica 2:  implementar a la fase 2
    qDebug()<<"Estic a Transparencia";
}

void GLWidget::activaExercici1() {
    //TO DO: Pràctica 2:  implementar a la fase 2
    qDebug()<<"Estic a Exercici 1";
    addElementsEx1();
    activateEx1Shader();
}

void GLWidget::activaExercici2() {
    //TO DO: Pràctica 2:  implementar a la fase 2
    qDebug()<<"Estic a Exercici 2";
    addElementsEx2();
    activateEx2Shader();
}

void GLWidget::addElementsEx1() {
    //TO DO: Pràctica 2:  añadir elementos a la escena para ex1
    qDebug()<<"Afegint elements a l'escena";
    Controller::getInstance()->createScene(GPUSceneFactory::SCENE_TYPES::VIRTUALWORLD, "://resources/scenes/ex1.json");
    shared_ptr<GPUCamera> camera = dynamic_pointer_cast<GPUCamera>(Controller::getInstance()->getSetUp()->getCamera());
    camera->actualitzaCamera(Controller::getInstance()->getScene()->capsaMinima);
}

void GLWidget::addElementsEx2() {
    //TO DO: Pràctica 2:  añadir elementos a la escena para ex2
    qDebug()<<"Afegint elements a l'escena";
    Controller::getInstance()->createScene(GPUSceneFactory::SCENE_TYPES::VIRTUALWORLD, "://resources/scenes/ex2.json");
    shared_ptr<GPUCamera> camera = dynamic_pointer_cast<GPUCamera>(Controller::getInstance()->getSetUp()->getCamera());
    camera->actualitzaCamera(Controller::getInstance()->getScene()->capsaMinima);
}

void GLWidget::activateEx1Shader() {
    //TO DO: Pràctica 2:  activar shader de ex 1
    qDebug()<<"Activant Exercici 1";
    current_program = 9;
    program[current_program]->link();
    program[current_program]->bind();
    updateShader();
}

void GLWidget::activateEx2Shader() {
    //TO DO: Pràctica 2:  activar shader de ex 2
    qDebug()<<"Activant Exercici 2";
    current_program = 10;
    program[current_program]->link();
    program[current_program]->bind();
    updateShader();
}


void GLWidget::setPerspective(float nearPlane, float farPlane)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->setFrustum(camera->vfovRadians DEGREES, nearPlane, farPlane, TipProj::PERSPECTIVA);

    updateGL();
}

void GLWidget::setLookAt(const QVector3D &eye, const QVector3D &center, const QVector3D& up)
{
    vec4 lookfrom(eye[0], eye[1], eye[2], 1.0);
    vec4 lookat(center[0], center[1], center[2], 1.0);
    vec4 vup(up[0], up[1], up[2], 1.0);

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->setObservador(lookfrom, lookat, vup);

    updateGL();
}

void GLWidget::setLighting(const QVector3D &lightPos, const QVector3D &Ia, const QVector3D &Id,
                           const QVector3D &Is, const QVector3D &coefs)
{
    vec4 lightPosition(lightPos[0],lightPos[1], lightPos[2], 1.0) ;
    vec3 intensityA( Ia[0], Ia[1], Ia[2]);
    vec3 intensityD( Id[0], Id[1], Id[2]);
    vec3 intensityS( Is[0], Is[1], Is[2]);

    auto lights = Controller::getInstance()->getSetUp()->getLights();
    lights[0]->setIa(intensityA);
    lights[0]->setId(intensityD);
    lights[0]->setIs(intensityS);
    lights[0]->setLightPosition(lightPosition);
    updateGL();
}


/**  Mètodes d'interacció amb el ratolí */

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
           setXRotation(dy);
           setYRotation(dx);
        } else if(lastPos.y()!= event->y()) {// rotar la camera
            setXRotation(dy);
        } else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }

    } else if (event->buttons() & Qt::RightButton) {
       // Zoom: canviar la mida de la window
        if(lastPos.y()> event->y())
             Zoom(-1);
        else
             Zoom(1);
    }

    lastPos = event->pos();
}


void GLWidget::setXRotation(int angle)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    if (angle >0) {
        camera->angX += 5;
    } else if (angle<0)
        camera->angX -= 5;
    qNormalizeAngle(camera->angX);

    camera->rotaCamera();

    emit ObsCameraChanged(camera);

    updateGL();
}

void GLWidget::setYRotation(int angle)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    if (angle >0) {
        camera->angY += 5;
    } else if (angle<0)
        camera->angY-= 5;
    qNormalizeAngle(camera->angY);

    camera->rotaCamera();

    emit ObsCameraChanged(camera);

    updateGL();
}

void GLWidget::qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360*16;
    while (angle > 360)
        angle -= 360*16;
}

void GLWidget::Zoom (int positiu) {

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->AmpliaWindow(positiu*(0.005));

    emit FrustumCameraChanged(camera);

    updateGL();
}
