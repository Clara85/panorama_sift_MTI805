/* ******       ************************        **********************************        ************************       ***** */
/*                                                                                                                             */
/* ******       Informations :                                                                                           ***** */
/*              Code crée par Julien Couet pour le cour de maitrise MTI880 de l'ETS (http://www.etsmtl.ca/) encadré par        */
/*              le professeur Luc Duong   (METTRE SITE DU COURS).                                                              */
/*                                                                                                                             */
/* ******       Licence, Droits et Garantie :                                                                            ***** */
/*              Ce code est libre de tous droits dans la limite de ceux de OpenCV : http://opencv.willowgarage.com/wiki/       */
/*              qui est sous licence 'BSD' :  http://en.wikipedia.org/wiki/BSD_licenses                                        */
/*              Il n'y a aussi aucune garantie de fonctionnement, c'est unp rojet universitaire                                */
/*                                                                                                                             */
/* ******       Contacts :                                                                                               ***** */
/*              couet.julien@gmail.com     luc.duong@estml.ca                                                                  */
/*                                                                                                                             */
/* ******       ************************        **********************************        ************************       ***** */


#ifndef CVMATCH_H
#define CVMATCH_H


#include <QImage>
#include <QSize>
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
            /** *****                                                 Classe cvMatch                                                 **** **/
            /** *****                                                                                                                **** **/
            /** *****           Calcul et retourne la liste de matchs (pixels associés) entre deux images                            **** **/
            /** *****           Retourne également une image contenant celle de reference (A) et celle qui sera projetée (B)         **** **/
            /** *****           (Res = A + B) et ou des lignes sont dessinnées pour illustrer les matchs                             **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Communication avec cvImage                                                **** **/
            /** *****                                   3. Traitements OpenCV                                                        **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/



class cvMatch  {

    public  :
        /** 1.  Constructeur & Destructeur **/
            cvMatch();
            ~cvMatch();


        /** 2.  Communication avec cvImage **/
            IplImage *          findMatches(IplImage *, vector<Descriptor>, IplImage *, vector<Descriptor>);
            vector<Descriptor>  getGoodFeaturesListFirst () ;
            vector<Descriptor>  getGoodFeaturesListSecond () ;


    private :
        vector<Descriptor>  goodFeaturesListFirst, goodFeaturesListSecond;

        /** 3.  Traitements OpenCV  **/
            double          processSquareDistance(Descriptor, Descriptor);
            IplImage *      combineImages(IplImage *, IplImage *);
            bool            processPointsMatching(Descriptor &, Descriptor,  vector<Descriptor> );

};

#endif // CVMATCH_H
