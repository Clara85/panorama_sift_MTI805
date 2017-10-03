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


#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <vector>
using namespace std;


            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                  Classe Descriptor                                             **** **/
            /** *****                                                                                                                **** **/
            /** *****                               Structure contenant les Descripteurs SIFT d'une image                            **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/

class Descriptor {
    public:
            float           xi, yi;	// Location
            vector<double>  fv;         // Vecteur des caractéristiques

            /// 1er Constructeur par défaut
            Descriptor()  {
            }

            /// 2nd Constructeur
            Descriptor(float x, float y, vector<double> const& f)    {
                    xi = x;
                    yi = y;
                    fv = f;
            }
};

#endif // DESCRIPTOR_H
