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


#ifndef CVIMAGE_H
#define CVIMAGE_H


#include <QImage>
#include <QSize>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

#include "cvsift.h"
#include "Keypoint.h"
#include "Descriptor.h"
#include "cvmatch.h"



            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                 Classe cvImage                                                 **** **/
            /** *****                                                                                                                **** **/
            /** *****           Permet de charger une image (jpeg, png, pgm, etc...) et de traiter la couleur de ses pixels          **** **/
            /** *****                       suivant trois espaces : RGB, HSV et HSL via la librairie OpenCV                          **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   0. Macro : Calcul de la moyenne (mean)                                       **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Communication avec mainWindow                                             **** **/
            /** *****                                   3. Traitements OpenCV                                                        **** **/
            /** *****                                   4. Conversion OpenCV - Qt                                                    **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/



class cvImage {

    public:
        /** 1.  Constructeur & Destructeur **/
            cvImage();
            ~cvImage();

        /** 2.   Communication avec mainWindow**/
        /// 2.1   Traitements communs
            bool                    openImage(const char *, const char *);
            QImage                  getFirstImage(QSize);
            QImage                  getSecondImage(QSize);
            QSize                   getQtRealImageSize();
            void                    setBlurringParams(double, double, double);


        /// 2.2   SIFT et Matching entre deux images
            QImage                  processSiftFirst(QSize, int, int, double, double);
            QImage                  processSiftSecond(QSize, int, int, double, double);
            QImage                  processMatch(QSize);

        /// 2.3   Création de l'image du panorama de deux images
            QImage                  getMosaicImage(QSize);


        /** 3.  Traitements OpenCV  **/
        /// 3.1    Calcul de l'homographie
            void                    computeHomography(bool, double);
            void                    computeHomography(vector<vector<int> >, vector<vector<int> >);

        /// 3.2    Projection des pixels de la seconde image dans le plan image de la première (celle de reference)
            void                    processProjection(int);


    private:
        cvMatch                 *_cvMatch;
        vector<Keypoint>        _keyPointsFirst, _keyPointsSecond;
        vector<Descriptor>      _keyDescsListFirst, _keyDescsListSecond;

        IplImage                *_cvImageFirst, *_cvImageSecond,  *_cvImageSiftFirst, *_cvImageSiftSecond,
                                *_cvImageProjectionFirst, *_cvImageProjectionSecond, *_cvImageMosaic;
        CvMat                   *_cvHomographyMat;

        int                     _numChannels;
        QSize                   _qtRealImageSize;
        double                  _alpha, _beta, _gamma;

        /** 4.  Conversion OpenCV - Qt  **/
            QImage                  cvToQt(QSize, IplImage *);

};
#endif // CVIMAGE_H
