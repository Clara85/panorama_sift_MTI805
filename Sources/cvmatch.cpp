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


#include "cvmatch.h"
#include "Descriptor.h"
#include "Keypoint.h"



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




/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            1. Constructeur & Destructeur         ************************       **** **/

/// 1.1     Constructeur
cvMatch::cvMatch() {

}


/// 1.2 Destructeur
cvMatch::~cvMatch() {

}


/** *****       ************************          fin Constructeur & Destructeur          ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************       2.    Communication avec cvImage           ************************       **** **/

/// 2.1     Retourne la liste des matchs pertinents
    /// 2.1.1    Liste de la première image
vector<Descriptor> cvMatch::getGoodFeaturesListFirst () {
    return goodFeaturesListFirst;
}

    /// 2.1.2     Liste de la seconde image
vector<Descriptor> cvMatch::getGoodFeaturesListSecond () {
    return goodFeaturesListSecond;
}



/// 2.2     Retourne une image contenant celle de reference (A) et celle qui sera projetée (B) (Res = A + B)
///         et ou des lignes sont dessinnées pour illustrer les matchs
IplImage *   cvMatch::findMatches(IplImage *cvImageFirst, vector<Descriptor> keyDescsListFirst,
                                  IplImage *cvImageSecond, vector<Descriptor> keyDescsListSecond)  {
        IplImage    *result = combineImages(cvImageFirst, cvImageSecond);
        CvPoint     p, q;
        CvScalar    color;


       for (int i = 0;  i< keyDescsListFirst.size(); i++)  {

           Descriptor match;

           // Si il y a un match (match non NULL)
           if(processPointsMatching(match, keyDescsListFirst[i], keyDescsListSecond)) {

               // On le conserve
               goodFeaturesListFirst.push_back(keyDescsListFirst[i]);
               goodFeaturesListSecond.push_back(match);

               // Puis on dessine sur l'image result (A + B cf. 3.3) une ligne
               // Reliant les deux points
               p.x = keyDescsListFirst[i].xi;
               p.y = keyDescsListFirst[i].yi;

               q.x = match.xi + cvImageFirst->width;
               q.y = match.yi;

               color = CV_RGB( rand()&255, rand()&255, rand()&255 );

               cvLine( result,  p, q, color, 1, 8, 0 );
               cvCircle(result, p, 2, color, 2, 8, 0);
               cvCircle(result, q, 2, color, 2, 8, 0);

           }

       }

       return result;
}


/** *****       ************************          fin Communication avec cvImage          ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            3.    Traitements OpenCV              ************************       **** **/

/// 3.1     Match un 'Descriptor' d'une première liste (image 1) (KeyDescs) avec un autre contenu dans une seconde liste (keyDescsList)
///         Si un match est trouvé, le 'Descriptor' de la seconde liste est retourné dans 'match'
bool        cvMatch::processPointsMatching(Descriptor &match, Descriptor KeyDescs,  vector<Descriptor> keyDescsList)  {

    double dsq, distsq1 = 100000000, distsq2 = 100000000;


    for (int i = 0; i < keyDescsList.size(); i++)  {
        dsq = processSquareDistance(KeyDescs, keyDescsList[i]);

        if (dsq < distsq1) {
            distsq2 = distsq1;
            distsq1 = dsq;
            match = keyDescsList[i];
        }

        else if (dsq < distsq2) {
            distsq2 = dsq;
        }
    }

     if (10 * 10 * distsq1 < 6 * 6 * distsq2)  {
          return true;
     }

     else
         return false;
}

/// 3.2     Calcul la norme (Carré de la différence entre les vecteurs de caractéristiques) entre deux 'Descriptors' (KeyDescs1 & KeyDescs2)
double      cvMatch::processSquareDistance(Descriptor KeyDescs1, Descriptor KeyDescs2)  {

    vector<double> pk1, pk2;
    double diff, squareDistance = 0;

    pk1 = KeyDescs1.fv;
    pk2 = KeyDescs2.fv;

    for (int i = 0; i < 128; i++) {
        diff = (double)pk1[i] - (double)pk2[i];
        squareDistance += diff * diff;
    }

    return squareDistance;
}

/// 3.3     Retourne une image contenant celle de reference (A) et celle qui sera projetée (B) (Res = A + B)
IplImage *  cvMatch::combineImages(IplImage *cvImageFirst, IplImage *cvImageSecond) {
    IplImage *cvImagePointsMatched  = cvCreateImage(cvSize(cvImageFirst->width + cvImageSecond->width, cvImageFirst->height), IPL_DEPTH_8U, 3);
    CvScalar cvScalar;


    for(int y=0; y<cvImageFirst->height; y++) {
        for(int x=0; x<cvImageFirst->width; x++) {

            cvScalar.val[0] = cvGet2D(cvImageFirst,y,x).val[0];
            cvScalar.val[1] = cvGet2D(cvImageFirst,y,x).val[1];
            cvScalar.val[2] = cvGet2D(cvImageFirst,y,x).val[2];

            cvSet2D(cvImagePointsMatched, y, x, cvScalar);
        }
    }

    for(int y=0; y<cvImageSecond->height; y++) {
        for(int x=0; x<cvImageSecond->width; x++) {

            cvScalar.val[0] = cvGet2D(cvImageSecond,y,x).val[0];
            cvScalar.val[1] = cvGet2D(cvImageSecond,y,x).val[1];
            cvScalar.val[2] = cvGet2D(cvImageSecond,y,x).val[2];

            cvSet2D(cvImagePointsMatched, y, x + cvImageFirst->width, cvScalar);
        }
    }

    return cvImagePointsMatched;
}


/** *****       ************************              fin Traitements OpenCV              ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/






