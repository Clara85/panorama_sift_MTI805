//  Couet Julien pour Luc Duong ETS Génie Technologie de l'Information
//  Projet cour MGR8800, Vision 3D

#ifndef GLWIDGET_H
#define GLWIDGET_H

                        /** *****       *************************      **** **/
                        /**                                                 **/
                        /**                Classe _cvImage                  **/
                        /**                                                 **/
                        /**         1. Structure de l'application           **/
                        /**          2. Construction Interface UI           **/
                        /**    3. Récupération des Evenements Utilisateurs  **/
                        /**                                                 **/
                        /** *****       ************************       **** **/

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;


class GLWidget : public QGLWidget {
Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    int xRotation() const { return _xRot; }
    int yRotation() const { return _yRot; }
    int zRotation() const { return _zRot; }

    void create3DRGBHistogram(vector<vector<double> >, double);
    void create3DHSVHistogram(vector<vector<double> >, vector<vector<double> >, double);
    void create3DHSLHistogram(vector<vector<double> >, vector<vector<double> >, double);

public slots:
    void setXRotation(GLint angle);
    void setYRotation(GLint angle);
    void setZRotation(GLint angle);


signals:
    void xRotationChanged(GLint angle);
    void yRotationChanged(GLint angle);
    void zRotationChanged(GLint angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(GLint width, GLint height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private:
// Variables
    GLint       _sizeCubeRVBRef, _sizeHSVRef;
    GLuint      _refListId, _refPointListId;
    GLint       _xRot;
    GLint       _yRot;
    GLint       _zRot;
    QPoint      _lastPos;
    bool        _inRGB, _inHSV, _inHSL;

// Fonctions
    GLuint  make3DRGBHistogram();
    GLuint  make3DHSVHistogram();
    GLuint  make3DHSLHistogram();
    GLuint  makeColorRGBSpheres(vector<vector<double> >, double);
    GLuint  makeColorHSVSpheres(vector<vector<double> >, vector<vector<double> >, double);
    GLuint  makeColorHSLSpheres(vector<vector<double> >, vector<vector<double> >, double);

    void    makeRGBCubeRef();
    void    makeHSVConeRef();
    void    makeHSLPyramidRef();

    void    drawScene(GLuint gear, GLdouble angle, GLfloat, GLfloat, GLfloat);
    void    normalizeAngle(GLint *angle);

};

#endif
