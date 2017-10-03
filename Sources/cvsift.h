/* ******       ************************        **********************************        ************************       ***** */
/*                                                                                                                             */
/* ******       Informations :                                                                                           ***** */
/*              Code crée par Julien Couet pour le cour de maitrise MTI880 de l'ETS (http://www.etsmtl.ca/) encadré par        */
/*              le professeur Luc Duong   http://profs.etsmtl.ca/lduong/cours/mti880/                                          */
/*              Cette classe a été construite sur celui totalement libre de droits de Utkarsh Sinha : http://aishack.in/       */
/*                                                                                                                             */
/* ******       Licence, Droits et Garantie :                                                                            ***** */
/*              Ce code est libre de tous droits dans la limite de ceux de OpenCV : http://opencv.willowgarage.com/wiki/       */
/*              qui est sous licence 'BSD' :  http://en.wikipedia.org/wiki/BSD_licenses                                        */
/*                                                                                                                             */
/* ******       Contacts :                                                                                               ***** */
/*              couet.julien@gmail.com                                                                                         */
/*                                                                                                                             */
/* ******       ************************        **********************************        ************************       ***** */


#ifndef CVSIFT_H
#define CVSIFT_H


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#include "Keypoint.h"
#include "Descriptor.h"


            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                  Classe cvSift                                                 **** **/
            /** *****                                                                                                                **** **/
            /** *****                   Trouve les KeyPoints SIFT d'une image et calcul les Descripteurs de chacun                   **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   0. Macro : Calcul de la moyenne (mean)                                       **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Communication avec cvImage                                                **** **/
            /** *****                                   3. Traitements OpenCV                                                        **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/


class cvSift  {

    public :
        /** 1.  Constructeur & Destructeur **/
           cvSift(const IplImage *);
            void createWorking2DArrays();
           ~cvSift();


       /** 2.    Communication avec cvImage **/
       /// 2.1     Calcul des descripteurs SIFT d'une image
           void                 processSift(int, int, double, double);

       /// 2.2     Retourne l'image initiale sur laquelle sont dessinés les KeyPoints
           IplImage *           getKeyPointsImage();

       /// 2.3     Récupération des tableaux (vecteurs) des KeyPoints et des Descripteurs SIFT
           vector<Keypoint>     getKeyPoints();
           vector<Descriptor>   getDescriptionList();
           int                  getNumKeyPoints();


    private :
        int                 _numOctaves, _numIntervals, _numKeypoints;	// Nombre de KeyPoints trouvés
        double              _contrastThreshold, _curvatureThreshold, _sigmaAntiAliasing, _sigmaPreblur;
        IplImage            *_cvCurrentImage;                           // L'image courrante

        IplImage            ***_cvGaussianImages2DArray;    // Tableau 2D contenant les différentes gaussiennes (images lissées pour chaque octaves)
        IplImage            ***_cvDOGImages2DArray;         // Tableau 2D contenant les différences de gaussiennes pour pour chaque octaves
        IplImage            ***_cvBinarieImages2DArray;     // Tableau 2D contenant les différentes images binaires : 1 = extrema, 0 = non extrema
        double              **_sigma2DArray;                // Tableau 2D contenant le sigma pour lisser l'image à une certaine octave

        vector<Keypoint>    _keyPoints;	// Vecteur des KeyPoints
        vector<Descriptor>  _keyDescs;	// Vecteur des Descripteurs



        /** 3.    Traitements OpenCV  **/
        /// 3.1      Création des KeyPoints
            void                BuildScaleSpace();
            void                processExtremasDetection();
            void                processOrientation();
            int                 getKernelSize(double, double);

        /// 3.2     Création des Descripteurs
            void                extractKeypointDescriptors();
            CvMat *             buildInterpolatedGaussianTable( int , double );
            double              gaussian2D(double, double, double);

};

#endif // CVSIFT_H
