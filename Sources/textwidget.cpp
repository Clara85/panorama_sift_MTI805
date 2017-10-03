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


#include "textwidget.h"

            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                Classe textWidget                                               **** **/
            /** *****                                                                                                                **** **/
            /** *****                   Permet d'afficher le texte explicatif décrivant les trois espaces couleurs                   **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Affichage du texte                                                        **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            1. Constructeur & Destructeur         ************************       **** **/

/// 1.1     Constructeur
TextWidget::TextWidget(QWidget *parent, QSize size) : QWidget(parent) {
    _size = size;
    _qtText = new QString("");

    update();
}

/// 1.2     Destructeur
TextWidget::~TextWidget() {
}


/** *****       ************************          fin Constructeur & Destructeur          ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************             2.   Affichage du texte              ************************       **** **/

/// 2.1     Insertion deu nouveau texte à afficher
void TextWidget::setText(QString qtText) {
     *_qtText = qtText;
     update();
}

/// 2.2     Affichage du texte via QPainter. Appeler à chaque update();
void TextWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    QRectF rect(0, 0, _size.width(), _size.height());

    painter.begin(this);
    painter.drawText(rect, Qt::AlignCenter, "Lorem ipsum dolor sit amet, \n"
                                            " consectetur adipiscing elit. \n "
                                            "Ut lorem lorem, gravida a semper nec, \n "
                                            "sollicitudin vel arcu. \n "
                                            "Suspendisse vel purus eget sem fringilla volutpat ut eget est.\n\n");
     painter.end();
}


/** *****       ************************              fin Affichage du texte              ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/




