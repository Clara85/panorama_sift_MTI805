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


#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H


#include <QtGui>
#include <QList>
#include <QSize>
#include <QPoint>
#include <QPixmap>
#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <iostream>
using namespace std;


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


class TextWidget : public QWidget  {
    Q_OBJECT

    public:
        /** 1.1     Constructeur & Destructeur */
            TextWidget(QWidget *parent, QSize);
            ~TextWidget();

        /// 2.1     Insertion du nouveau texte à afficher
            void setText(QString);


    protected:
        /// 2.2     Affichage du texte via QPainter. Appeler à chaque update();
            void paintEvent(QPaintEvent *event);

    private:
        QString  *_qtText;
        QSize     _size;
};

#endif // TEXTWIDGET_H
