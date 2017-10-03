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


#ifndef KEYPOINT_H
#define KEYPOINT_H

#include <vector>
#include <stdio.h>
//#include <tar.h>
using namespace std;



            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                   Classe KeyPoint                                              **** **/
            /** *****                                                                                                                **** **/
            /** *****                      Structure contenant les KeyPoints (points caractèristiques SIFT) d'une image              **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/


class Keypoint  {
    public:
            float           xi, yi;	// Location
            vector<double>  mag;	// Liste des magnitudes au point (xi, yi)
            vector<double>  orien;	// Liste des orientations detectées au point (xi, yi)
             int            scale;	// Echelle à laquelle le KeyPoint est trouvée

            Keypoint() { }
            Keypoint(float x, float y) { xi=x; yi=y; }
            Keypoint(float x, float y, vector<double> const&  m, vector<double> const& o,  int s) {
                    xi = x;
                    yi = y;
                    mag = m;
                    orien = o;
                    scale = s;
             }
};


#endif // KEYPOINT_H
