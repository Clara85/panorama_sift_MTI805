//  Couet Julien pour Luc Duong ETS Génie Technologie de l'Information
//  Projet cour MGR8800, Vision 3D



                        /** *****       *************************      **** **/
                        /**                                                 **/
                        /**                Classe _cvImage                  **/
                        /**                                                 **/
                        /**         1. Structure de l'application           **/
                        /**          2. Construction Interface UI           **/
                        /**    3. Récupération des Evenements Utilisateurs  **/
                        /**                                                 **/
                        /** *****       ************************       **** **/


#include "glwidget.h"

const double Pi     = 3.14159265358979323846;



/** *****       ************************       **** **/
/**                                                 **/
/** *****     1.Constructeur & Destructeur     **** **/
/**                                                 **/
/** *****       ************************       **** **/

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    _inRGB          = false;
    _inHSV          = false;
    _inHSL          = false;
    _refListId      = 0;
    _xRot           = 0;
    _yRot           = 0;
    _zRot           = 0;
    _sizeCubeRVBRef = 5;
    _sizeHSVRef     = 6;
}

GLWidget::~GLWidget() {
    makeCurrent();
    glDeleteLists(_refListId, 1);
    glDeleteLists(_refPointListId, 2);
}




/** *****       ************************       **** **/
/**                                                 **/
/** *****       2. OpenGl Initialisation     **** **/
/**                                                 **/
/** *****       ************************       **** **/

void GLWidget::initializeGL() {
    glPointSize (4);
    glEnable(GL_NORMALIZE);
    glEnable (GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.8f, 0.8f, 0.8f, 0.2f);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glRotated(_xRot / 16.0, 1.0, 0.0, 0.0);
        glRotated(_yRot / 16.0, 0.0, 1.0, 0.0);
        glRotated(_zRot / 16.0, 0.0, 0.0, 1.0);
        drawScene(_refListId, 0, 0, 0, 0);
        drawScene(_refPointListId, 0, 0, 0, 0);
    glPopMatrix();
  }

void GLWidget::resizeGL(GLint width, GLint height) {
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}

void GLWidget::drawScene(GLuint gear, GLdouble angle, GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
        glTranslatef(x, y, z);
        glRotated(angle, 0.0, 0.0, 1.0);

        if ( _inRGB == true) {
            glColor3f(0.0, 0.0, 0.0);
            this->renderText(-_sizeHSVRef,  -_sizeHSVRef,   -_sizeHSVRef,   "Noir",     QFont());
            glColor3f(1.0, 0.0, 0.0);
            this->renderText(_sizeHSVRef,   -_sizeHSVRef,   -_sizeHSVRef,   "Rouge",    QFont());
            glColor3f(0.0, 1.0, 0.0);
            this->renderText(-_sizeHSVRef,  _sizeHSVRef,    -_sizeHSVRef,   "Vert",     QFont());
            glColor3f(0.0, 0.0, 1.0);
            this->renderText(-_sizeHSVRef,  -_sizeHSVRef,   _sizeHSVRef,    "Bleu",     QFont());
            glColor3f(1.0, 1.0, 1.0);
            this->renderText(_sizeHSVRef,   _sizeHSVRef,    _sizeHSVRef,    "Blanc",    QFont());
        }
        else if(_inHSV == true){
            glColor3f(0.0, 0.0, 0.0);
             this->renderText(0, -_sizeHSVRef, 0, "Noir", QFont());
            glColor3f(1.0, 0.0, 0.0);
            this->renderText(5, _sizeHSVRef, 0, "Rouge", QFont());
            glColor3f(0.0, 1.0, 0.0);
            this->renderText(_sizeHSVRef*cos(2*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(2*Pi/3), "Vert", QFont());
            glColor3f(0.0, 0.0, 1.0);
            this->renderText(_sizeHSVRef*cos(4*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(4*Pi/3), "Bleu", QFont());
            glColor3f(1.0, 1.0, 1.0);
            this->renderText(0, _sizeHSVRef, 0, "Blanc", QFont());
        }

        else if(_inHSL == true){
            glColor3f(0.0, 0.0, 0.0);
             this->renderText(0, -_sizeHSVRef - 1, 0, "Noir", QFont());
            glColor3f(1.0, 0.0, 0.0);
            this->renderText((_sizeHSVRef+1)*cos(0), 0, (_sizeHSVRef+1)*sin(0), "Rouge", QFont());
            glColor3f(0.0, 1.0, 0.0);
            this->renderText((_sizeHSVRef+1)*cos(2*Pi/3), 0, (_sizeHSVRef+1)*sin(2*Pi/3), "Vert", QFont());
            glColor3f(0.0, 0.0, 1.0);
            this->renderText((_sizeHSVRef+1)*cos(4*Pi/3), 0, (_sizeHSVRef+1)*sin(4*Pi/3), "Bleu", QFont());
            glColor3f(1.0, 1.0, 1.0);
            this->renderText(0, _sizeHSVRef +1 , 0, "Blanc", QFont());
        }

        glCallList(gear);
    glPopMatrix();
}



/** *****       ************************       **** **/
/**                                                 **/
/** *****     3. Construction de la scene      **** **/
/**                                                 **/
/** *****       ************************       **** **/

// Cube RBG
GLuint GLWidget::make3DRGBHistogram( ) {
    GLuint list  = glGenLists(1);
     glNewList(list, GL_COMPILE);

        this->makeRGBCubeRef();

    glEndList();
    return list;
}

void GLWidget::makeRGBCubeRef() {

 // RGB cube
 glBegin(GL_LINE_LOOP);
 glColor3ub(0,0,0);
  glVertex3d(-_sizeCubeRVBRef,-_sizeCubeRVBRef,-_sizeCubeRVBRef);
  glColor3ub(255,0,0);
  glVertex3d(_sizeCubeRVBRef,-_sizeCubeRVBRef,-_sizeCubeRVBRef);
  glColor3ub(255,255,0);
  glVertex3d(_sizeCubeRVBRef,_sizeCubeRVBRef,-_sizeCubeRVBRef);
  glColor3ub(0,255,0);
  glVertex3d(-_sizeCubeRVBRef,_sizeCubeRVBRef,-_sizeCubeRVBRef);
 glEnd();

 glBegin(GL_LINE_LOOP);
  glColor3ub(0,0,255);
  glVertex3d(-_sizeCubeRVBRef,-_sizeCubeRVBRef,_sizeCubeRVBRef);
  glColor3ub(255,0,255);
  glVertex3d(_sizeCubeRVBRef,-_sizeCubeRVBRef,_sizeCubeRVBRef);
  glColor3ub(255,255,255);
  glVertex3d(_sizeCubeRVBRef,_sizeCubeRVBRef,_sizeCubeRVBRef);
  glColor3ub(0,255,255);
  glVertex3d(-_sizeCubeRVBRef,_sizeCubeRVBRef,_sizeCubeRVBRef);
 glEnd();

 glBegin(GL_LINES);
  glColor3ub(0,0,0);
  glVertex3d(-_sizeCubeRVBRef,-_sizeCubeRVBRef,-_sizeCubeRVBRef);
  glColor3ub(0,0,255);
  glVertex3d(-_sizeCubeRVBRef,-_sizeCubeRVBRef,_sizeCubeRVBRef);
  glColor3ub(255,0,0);
  glVertex3d(_sizeCubeRVBRef,-_sizeCubeRVBRef,-_sizeCubeRVBRef);
  glColor3ub(255,0,255);
  glVertex3d(_sizeCubeRVBRef,-_sizeCubeRVBRef,_sizeCubeRVBRef);
  glColor3ub(255,255,0);
  glVertex3d(_sizeCubeRVBRef,_sizeCubeRVBRef,-_sizeCubeRVBRef);
  glColor3ub(255,255,255);
  glVertex3d(_sizeCubeRVBRef,_sizeCubeRVBRef,_sizeCubeRVBRef);
  glColor3ub(0,255,0);
  glVertex3d(-_sizeCubeRVBRef,_sizeCubeRVBRef,-_sizeCubeRVBRef);
  glColor3ub(0,255,255);
  glVertex3d(-_sizeCubeRVBRef,_sizeCubeRVBRef,_sizeCubeRVBRef);
 glEnd();
}

GLuint GLWidget::makeColorRGBSpheres(vector<vector<double> > colorArray,  double sizeImage) {
    double channel1     = 300, channel2 = 300, channel3 = 300;
    GLuint list         = glGenLists(2);

    glNewList(list, GL_COMPILE);
    glPushMatrix();

    for (int i = 0; i < sizeImage; i+=5) {
        if ( (colorArray[i][0] != channel1) && (colorArray[i][1] != channel2) && (colorArray[i][2] != channel3)) {
            glPushMatrix();
                glBegin (GL_POINTS);
                    glColor4f((float)(colorArray[i][2]/255), (float)(colorArray[i][1]/255), (float)(colorArray[i][0]/255), 1);

                    glVertex3f( (GLfloat)(- _sizeCubeRVBRef  +  ((colorArray[i][2]*_sizeCubeRVBRef)/(256/2))),
                                (GLfloat)(- _sizeCubeRVBRef +  ((colorArray[i][1]*_sizeCubeRVBRef)/(256/2))),
                                (GLfloat)(- _sizeCubeRVBRef +  ((colorArray[i][0]*_sizeCubeRVBRef)/(256/2))));
                glEnd();
            glPopMatrix();
            channel1    = colorArray[i][0];
            channel2    = colorArray[i][1];
            channel3    = colorArray[i][2];
        }
    }
    glPopMatrix();
    glEndList();

    return (list);
}


// Cone HSV
GLuint GLWidget::make3DHSVHistogram() {
    GLuint list          = glGenLists(1);

    glNewList(list, GL_COMPILE);
        glPushMatrix();
            glTranslatef( 0, -_sizeCubeRVBRef , 0 );
            this->makeHSVConeRef();
        glPopMatrix();
    glEndList();

    return (list);
}

void GLWidget::makeHSVConeRef() {
    // Noir Blanc Rouge
    glBegin (GL_LINE_STRIP);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeCubeRVBRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(255,0,0);
        glVertex3f(_sizeCubeRVBRef*cos(0), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(0));
    glEnd();

    // Noir Blanc Jaune
    glBegin (GL_LINE_STRIP);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeCubeRVBRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(255, 255, 0);
        glVertex3f(_sizeCubeRVBRef*cos(Pi/3), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(Pi/3));
    glEnd();

    // Noir Blanc Vert
    glBegin (GL_LINE_STRIP);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeCubeRVBRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(0, 255, 0);
        glVertex3f(_sizeCubeRVBRef*cos(2*Pi/3), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(2*Pi/3));
    glEnd();

    // Noir Blanc Cyan
    glBegin (GL_LINE_STRIP);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeCubeRVBRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(0,255,255);
        glVertex3f(_sizeCubeRVBRef*cos(Pi), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(Pi));
    glEnd();


    // Noir Blanc Blue
    glBegin (GL_LINE_STRIP);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeCubeRVBRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(0,0, 255);
        glVertex3f(_sizeCubeRVBRef*cos(4*Pi/3), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(4*Pi/3));
    glEnd();


    // Noir Blanc Magenta
    glBegin (GL_LINE_STRIP);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeCubeRVBRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(255,0, 255);
        glVertex3f(_sizeCubeRVBRef*cos(5*Pi/3), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(5*Pi/3));
    glEnd();


    // Rouge Jaune Vert Cyan Blue Magenta
    glBegin (GL_LINE_LOOP);
        glColor3ub(255,0, 0);
        glVertex3f(_sizeCubeRVBRef*cos(0), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(0));
        glColor3ub(255, 255, 0);
        glVertex3f(_sizeCubeRVBRef*cos(Pi/3), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(Pi/3));
        glColor3ub( 0,255, 0);
        glVertex3f(_sizeCubeRVBRef*cos(2*Pi/3), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(2*Pi/3));
        glColor3ub(0,255, 255);
        glVertex3f(_sizeCubeRVBRef*cos(Pi), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(Pi));
        glColor3ub(0, 0, 255);
        glVertex3f(_sizeCubeRVBRef*cos(4*Pi/3), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(4*Pi/3));
        glColor3ub(255,0, 255);
        glVertex3f(_sizeCubeRVBRef*cos(5*Pi/3), 2*_sizeCubeRVBRef, _sizeCubeRVBRef*sin(5*Pi/3));
    glEnd();
}

GLuint GLWidget::makeColorHSVSpheres(vector<vector<double> > positionArray, vector<vector<double> > colorArray,  double sizeImage) {
    double channel1     = 300, channel2 = 300, channel3 = 300;
    GLuint list         = glGenLists(2);

    glNewList(list, GL_COMPILE);
    glPushMatrix();
    glTranslatef( 0, -_sizeCubeRVBRef , 0 );

    for (int i = 0; i < sizeImage; i+=5) {
        if ( (colorArray[i][0] != channel1) && (colorArray[i][1] != channel2) && (colorArray[i][2] != channel3)) {
            glPushMatrix();
                glBegin (GL_POINTS);
                    glColor4f((float)(colorArray[i][2]/255), (float)(colorArray[i][1]/255), (float)(colorArray[i][0]/255), 1);
                    glVertex3f( (GLfloat)(_sizeCubeRVBRef)*(positionArray[i][1]*cos(2*Pi*positionArray[i][0]/180)/255)*(positionArray[i][2]/255),
                                (GLfloat)(2*_sizeCubeRVBRef*positionArray[i][2]/255),
                                (GLfloat)(_sizeCubeRVBRef)*(positionArray[i][1]*sin(2*Pi*positionArray[i][0]/180)/255)*(positionArray[i][2]/255));
                glEnd();
            glPopMatrix();
            channel1    = colorArray[i][0];
            channel2    = colorArray[i][1];
            channel3    = colorArray[i][2];
        }
    }
    glPopMatrix();
    glEndList();

    return (list);
}


// Pyramide HSL
GLuint GLWidget::make3DHSLHistogram() {
    GLuint list          = glGenLists(1);

    glNewList(list, GL_COMPILE);
        glPushMatrix();
            glTranslatef( 0, -_sizeHSVRef , 0 );
            this->makeHSLPyramidRef();
        glPopMatrix();
    glEndList();

    return (list);
}

 void GLWidget::makeHSLPyramidRef() {
// Pyramide du bas
    // Noir Gris Rouge
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(255,0,0);
          glVertex3f(_sizeHSVRef*cos(0), _sizeHSVRef, _sizeHSVRef*sin(0));
    glEnd();

    // Noir Gris Jaune
    glBegin (GL_LINE_STRIP);
     glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(255, 255, 0);
        glVertex3f(_sizeHSVRef*cos(Pi/3), _sizeHSVRef, _sizeHSVRef*sin(Pi/3));
    glEnd();

    // Noir Gris Vert
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(0, 255, 0);
        glVertex3f(_sizeHSVRef*cos(2*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(2*Pi/3));
    glEnd();

    // Noir Gris Cyan
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(0,255,255);
          glVertex3f(_sizeHSVRef*cos(Pi), _sizeHSVRef, _sizeHSVRef*sin(Pi));
    glEnd();


    // Noir Gris Blue
    glBegin (GL_LINE_STRIP);
       glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(0,0, 255);
        glVertex3f(_sizeHSVRef*cos(4*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(4*Pi/3));
    glEnd();


    // Noir Gris Magenta
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(0,0,0);
        glVertex3f(0, 0, 0);
        glColor3ub(255,0, 255);
        glVertex3f(_sizeHSVRef*cos(5*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(5*Pi/3));
    glEnd();


// Pyramide du Haut
    // Blanc Gris Rouge
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeHSVRef, 0);
        glColor3ub(255,0,0);
          glVertex3f(_sizeHSVRef*cos(0), _sizeHSVRef, _sizeHSVRef*sin(0));
    glEnd();

    // Blanc Gris Jaune
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeHSVRef, 0);
        glColor3ub(255, 255, 0);
        glVertex3f(_sizeHSVRef*cos(Pi/3), _sizeHSVRef, _sizeHSVRef*sin(Pi/3));
    glEnd();

    // Blanc Gris Vert
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeHSVRef, 0);
        glColor3ub(0, 255, 0);
        glVertex3f(_sizeHSVRef*cos(2*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(2*Pi/3));
    glEnd();

    // Blanc Gris Cyan
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeHSVRef, 0);
        glColor3ub(0,255,255);
        glVertex3f(_sizeHSVRef*cos(Pi), _sizeHSVRef, _sizeHSVRef*sin(Pi));
    glEnd();


    // Blanc Gris Blue
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeHSVRef, 0);
        glColor3ub(0,0, 255);
        glVertex3f(_sizeHSVRef*cos(4*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(4*Pi/3));
    glEnd();


    // Blanc Gris Magenta
    glBegin (GL_LINE_STRIP);
        glColor3ub(255/2,255/2,255/2);
        glVertex3f(0, _sizeHSVRef, 0);
        glColor3ub(255,255,255);
        glVertex3f(0, 2*_sizeHSVRef, 0);
        glColor3ub(255,0, 255);
        glVertex3f(_sizeHSVRef*cos(5*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(5*Pi/3));
    glEnd();




    // Rouge Jaune Vert Cyan Blue Magenta
    glBegin (GL_LINE_LOOP);
        glColor3ub(255,0, 0);
        glVertex3f(_sizeHSVRef*cos(0), _sizeHSVRef, _sizeHSVRef*sin(0));
        glColor3ub(255, 255, 0);
        glVertex3f(_sizeHSVRef*cos(Pi/3), _sizeHSVRef, _sizeHSVRef*sin(Pi/3));
        glColor3ub( 0,255, 0);
        glVertex3f(_sizeHSVRef*cos(2*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(2*Pi/3));
        glColor3ub(0,255, 255);
        glVertex3f(_sizeHSVRef*cos(Pi), _sizeHSVRef, _sizeHSVRef*sin(Pi));
        glColor3ub(0, 0, 255);
        glVertex3f(_sizeHSVRef*cos(4*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(4*Pi/3));
        glColor3ub(255,0, 255);
        glVertex3f(_sizeHSVRef*cos(5*Pi/3), _sizeHSVRef, _sizeHSVRef*sin(5*Pi/3));
    glEnd();
}

GLuint GLWidget::makeColorHSLSpheres(vector<vector<double> > positionArray, vector<vector<double> > colorArray,  double sizeImage) {
    double channel1     = 300, channel2 = 300, channel3 = 300;
    GLuint list         = glGenLists(2);

    glNewList(list, GL_COMPILE);
    glPushMatrix();
    glTranslatef( 0, -_sizeHSVRef , 0 );

    for (int i = 0; i < sizeImage; i+=5) {
        if ( (colorArray[i][0] != channel1) && (colorArray[i][1] != channel2) && (colorArray[i][2] != channel3)) {
            glPushMatrix();
                glBegin (GL_POINTS);
                    glColor3f((float)(colorArray[i][2]/255), (float)(colorArray[i][1]/255), (float)(colorArray[i][0]/255));

                    if (positionArray[i][1] <= (255/2)) {
                        glVertex3f( (GLfloat)(((_sizeHSVRef*positionArray[i][2])/(255/2))*positionArray[i][1]*cos(2*Pi*positionArray[i][0]/180)/255),
                                    (GLfloat)((_sizeHSVRef*positionArray[i][1])/(255/2)),
                                    (GLfloat)(((_sizeHSVRef*positionArray[i][2])/(255/2))*positionArray[i][1]*sin(2*Pi*positionArray[i][0]/180)/255));
                    }
                    else {
                        glVertex3f( (GLfloat)(((_sizeHSVRef*positionArray[i][2])/(255/2))*(255 - positionArray[i][1])*cos(2*Pi*positionArray[i][0]/180)/255),
                                    (GLfloat)((_sizeHSVRef*positionArray[i][1])/(255/2)),
                                    (GLfloat)(((_sizeHSVRef*positionArray[i][2])/(255/2))*(255 - positionArray[i][1])*sin(2*Pi*positionArray[i][0]/180)/255));
                    }

                glEnd();
            glPopMatrix();
            channel1    = colorArray[i][0];
            channel2    = colorArray[i][1];
            channel3    = colorArray[i][2];
        }
    }
    glPopMatrix();
    glEndList();

    return (list);
}



/** *****       ************************       **** **/
/**                                                 **/
/** *****     4. Evenmenents tilisateurs       **** **/
/**                                                 **/
/** *****       ************************       **** **/

void GLWidget::mousePressEvent(QMouseEvent *event) {
    _lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - _lastPos.x();
    int dy = event->y() - _lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(_xRot + 8 * dy);
        setYRotation(_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(_xRot + 8 * dy);
        setZRotation(_zRot + 8 * dx);
    }
    _lastPos = event->pos();
}

void GLWidget::create3DRGBHistogram(vector<vector<double> > rgbArray, double sizeImage) {
    _inRGB          =   true;
    _inHSV          =   false;
    _inHSL          =   false;
    _refListId      =   this->make3DRGBHistogram();
    _refPointListId =   this->makeColorRGBSpheres(rgbArray, sizeImage);
    updateGL();
}

void GLWidget::create3DHSVHistogram(vector<vector<double> > hsvArray, vector<vector<double> > rgbArray, double sizeImage) {
    _inRGB          =   false;
    _inHSV          =   true;
    _inHSL          =   false;
    _refListId       =   this->make3DHSVHistogram();
    _refPointListId =   this->makeColorHSVSpheres(hsvArray,  rgbArray, sizeImage);
    updateGL();
}

void GLWidget::create3DHSLHistogram(vector<vector<double> > hslArray, vector<vector<double> > rgbArray, double sizeImage) {
    _inRGB          =   false;
    _inHSV          =   false;
    _inHSL          =   true;
    _refListId      =   this->make3DHSLHistogram();
    _refPointListId =   this->makeColorHSLSpheres(hslArray,  rgbArray, sizeImage);
    updateGL();
}



/** *****       ************************       **** **/
/**                                                 **/
/** *****                5.Autres              **** **/
/**                                                 **/
/** *****       ************************       **** **/

void GLWidget::normalizeAngle(GLint *angle) {
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}

void GLWidget::setXRotation(GLint angle)
{
    normalizeAngle(&angle);
    if (angle != _xRot) {
        _xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(GLint angle)
{
    normalizeAngle(&angle);
    if (angle != _yRot) {
        _yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(GLint angle) {
    normalizeAngle(&angle);
    if (angle != _zRot) {
        _zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}
