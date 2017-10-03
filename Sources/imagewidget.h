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



#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H


                        /** *****       *************************      **** **/
                        /**                                                 **/
                        /**                Classe ImageWidget               **/
                        /**                                                 **/
                        /**         1. Structure de l'application           **/
                        /**          2. Construction Interface UI           **/
                        /**    3. Récupération des Evenements Utilisateurs  **/
                        /**                                                 **/
                        /** *****       ************************       **** **/
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
#include <vector>
#include <QMessageBox>

using namespace std;


            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                 Classe imageWidget                                             **** **/
            /** *****                                                                                                                **** **/
            /** *****                    Permet d'afficher une image dans un widget Qt via la classe QPainter                        **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Affichage de l'image                                                      **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/



class ImageWidget : public QWidget {
    Q_OBJECT

    public:
        /** 1.1     Constructeur & Destructeur */
            ImageWidget(QWidget *, QSize );
            ~ImageWidget();

        /// 2.1     Insertion de la nouvelle image à afficher
            void                    setImage(QImage);

        /// 2.2     On redessine l'image initiale pour effacer les points
            void                    resetImage();


        /// 3.1     Vecteur des points selectionnés            
            void                    setPointsOnImageSize(int);
            vector<vector<int> >    getPointsArray( );
            void                    setQtRealImageSize(QSize);
            void                    resetPointsSelection();

    signals:    /// Communication avec la classe cvImage
        void                    getNumberOfCheckedPoints(int );
        void                    resetCurrentImages();


    protected:
        /// 2.3     Affichage de l'image via QPainter. Appeler à chaque update();
            void                    paintEvent(QPaintEvent *);

        /// 3.2.1     Récupération de l'evenement utilisateur (QMouseEvent)
            void                    mousePressEvent(QMouseEvent *);

    private:
        QImage                  *_qtImage;         // Image affiché dans le widget
        QSize                   _qtRealImageSize;  // Taille de l'image OpenCV (vraie taille)
        int                     _pointsOnImageSize, _numCheckedPoints;
        vector<vector<int> >    _pointArray;       // Vecteur contenant les points selectionnés

        /// 3.2.2     On dessine le point selectionné sur l'image et on récupère ses coordonnées
        ///           dans un vecteur (_pointArray)
            void                    paintOnImage(QMouseEvent *, QRgb);
};

#endif //IMAGEWIDGET_H
