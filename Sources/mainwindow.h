/* ******       ************************        **********************************        ************************       ***** */
/*                                                                                                                             */
/* ******       Informations :                                                                                           ***** */
/*              Code crée par Julien Couet pour le cour de maitrise MTI880 de l'ETS (http://www.etsmtl.ca/) encadré par        */
/*              le professeur Luc Duong   http://profs.etsmtl.ca/lduong/cours/mti880/                                          */
/*                                                                                                                             */
/* ******       Licence, Droits et Garantie :                                                                            ***** */
/*              Ce code est libre de tous droits dans la limite de ceux de OpenCV : http://opencv.willowgarage.com/wiki/       */
/*              qui est sous licence 'BSD' :  http://en.wikipedia.org/wiki/BSD_licenses                                        */
/*                                                                                                                             */
/* ******       Contacts :                                                                                               ***** */
/*              couet.julien@gmail.com                                                                                         */
/*                                                                                                                             */
/* ******       ************************        **********************************        ************************       ***** */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtGui>
#include "cvimage.h"
#include "imagewidget.h"
#include "textwidget.h"
#include <iostream>
#include <vector>

#include <QComboBox>
#include <QScrollArea>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSpinBox>
#include <QImage>

using namespace std;


QT_BEGIN_NAMESPACE
    class QAction;
    class QLabel;
    class QFrame;
    class QMenu;
    class QScrollArea;
    class QSlider;
    class QWidget;
    class QPixmap;
    class QPushButton;
    class QRadioButton;
    class QCheckBox;
    class QGridLayout;
    class QHBoxLayout;
    class QSpinBox;
    class QToolButton;
    class QVBoxLayout;
    class QColor;
    class QSlider;
QT_END_NAMESPACE

class GLWidget;


            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                Classe mainWindow                                               **** **/
            /** *****                                                                                                                **** **/
            /** *****                   Création de l'interface utilisateur, communication entre les widgets et récuperation         **** **/
            /** *****                    des actions utilisateurs                                                                    **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Construction de l'interface                                               **** **/
            /** *****                                   3. Actions Utilisateurs                                                      **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/



class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        /** 1    Constructeur & Destructeur **/
            MainWindow();
            ~MainWindow();

    private slots:
        /** 3    Actions Utilisateurs **/
            void about();
            void openImage();
            void hideSliders();
            void processFeaturesMatching(); /// APPEL AUX FONCTIONS OPENCV DE LA CLASSE cvImage
            void resetCurrentImages();
            void setPointSize();
            void setSlidersParams();
            void processMosaic();  /// APPEL AUX FONCTIONS OPENCV DE LA CLASSE cvImage


    private:
         // Mes classes
            TextWidget      *_textWidget;
            cvImage         *_cvImage;
            ImageWidget     *_secondImageWidget, *_firstImageWidget, *_panoramaImageWidget;


        // Qt Interface
            QGridLayout     *_centralLayout, *_controlLayout, *_tasksLayout, *_pointsSelectionLayout, *_pamoramaLayout;
            QWidget         *_centralWidget;
            QScrollArea     *_textWidgetArea, *_panoramaImageArea, *_firstImageArea, *_secondImageArea;

        // Controles
            QCheckBox       *_useRansac;
            QComboBox       *_typeOfBlurring, *_tasksComboBox, *_pointsSelectionComboBox;
            QSpinBox        *_numberOctavesBox, *_numberIntervalsBox;

            QPushButton     *_resetParamsButton;
            QSlider         *_contrastThresholdSlider, *_sigmaAntiAliasingSlider, *_ransacSlider, *_pointsSizeSlider, *_alphaSlider, *_betaSlider, *_gammaSlider;
            QLabel          *_pointsSelectionLabel, *_contrastThresholdLabel, *_sigmaAntiAliasingLabel, *_ransacLabel, *_numberOctavesLabel, *_numberIntervalsLabel, *_pointsSizeLabel, *_typeOfBlurringLabel, *_logo, *_alphaLabel, *_betaLabel, *_gammaLabel;
            QFrame          *_frame0, *_frame1, *_frame2, *_frame3, *_frame4, *_frame5;

        // Menu
            QMenu           *fileMenu,  *helpMenu;
            QAction         *openImageAct, *exitAct, *aboutAct, *aboutQtAct;
            QSize           _sizeImageLabelArea, _sizeImagePanoramaLabelArea, _sizeTextLabelArea;
            bool            _imageIsOpen, _colorIsChosen;
            QVariant        _variant;


        /** 2.  Construction de l'interface **/
            void            createUI();
            QSlider *       createSlider(const char *, int, int, int);
            void            createActions();
            void            createMenus();
};

#endif
