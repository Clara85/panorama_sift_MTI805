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


#include "imagewidget.h"



            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                 Classe imageWidget                                             **** **/
            /** *****                                                                                                                **** **/
            /** *****                    Permet d'afficher une image dans un widget Qt via la classe QPainter                        **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Affichage de l'image                                                      **** **/
            /** *****                                   3. Actions Utilisateurs                                                      **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            1. Constructeur & Destructeur         ************************       **** **/

/// 1.1     Constructeur
ImageWidget::ImageWidget(QWidget *parent, QSize size) : QWidget(parent) {
   _qtImage = new QImage;

   _qtRealImageSize = QSize(100, 100);

   QImage dummy = QImage(size, QImage::Format_RGB32);
    dummy.fill(qRgb(204, 204, 204));
    *_qtImage = dummy;
    _pointsOnImageSize  = 2;

    this->resetPointsSelection();

    update();
}


/// 1.2     Destructeur
ImageWidget::~ImageWidget() {
    // Rien car un objet de la classe QImage n'a pas besoin d'etre desalloué (memoire)
}


/** *****       ************************          fin Constructeur & Destructeur          ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            2.   Affichage de l'image             ************************       **** **/

/// 2.1     Insertion de la nouvelle image à afficher
void ImageWidget::setImage(QImage qtImage) {
     *_qtImage = qtImage;
     this->resetPointsSelection();

     update();
}

/// 2.2     On redessine l'image initiale pour effacer les points
void ImageWidget::resetImage() {

     QImage dummy = QImage(_qtImage->size(), QImage::Format_RGB32);
      dummy.fill(qRgb(204, 204, 204));
      *_qtImage = dummy;

     this->resetPointsSelection();

     update();
}

/// 2.3     Affichage de l'image via QPainter. Appeler à chaque update();
void ImageWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
       painter.drawPixmap(0, 0, QPixmap::fromImage(*_qtImage));
    painter.end();
}


/** *****       ************************            fin Affichage de l'image              ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************         3.     Actions Utilisateurs              ************************       **** **/

/// 3.1     Vecteur des points selectionnés
    /// 3.1.1     Recupération de la vraie taille de l'image (OpenCV) pour convertir
    ///           coordonnées des points selectionnées sur l'image Qt. (cf. méthode 3.2.2 paintOnImage())
void ImageWidget::setQtRealImageSize(QSize qtRealImageSize) {
    _qtRealImageSize = qtRealImageSize;
}

    /// 3.1.2     Taille en pixels des points sur l'image
void ImageWidget::setPointsOnImageSize(int pointsOnImageSize) {
    _pointsOnImageSize = pointsOnImageSize;
}

    /// 3.1.3     Recupération du vecteur contenant les points selectionnées dans les coordonnées
    ///           de la vraie image (OpenCV).
vector<vector<int> > ImageWidget::getPointsArray( ) {
    if (_pointArray[3][0] == -1)  {

        this->resetPointsSelection();
        emit resetCurrentImages();
        this->update();

    }
    return _pointArray;
}

    /// 3.1.4     Reinitialisation du vecteur des points selectionné
void ImageWidget::resetPointsSelection() {
    _numCheckedPoints   = 0;

    vector<vector<int> > vect(8, std::vector<int> (2, -1));
    _pointArray  = vect;
}


/// 3.2     Selection des points
    /// 3.2.1     Récupération de l'evenement utilisateur (QMouseEvent)
void ImageWidget::mousePressEvent(QMouseEvent *event) {

    if (_numCheckedPoints == 0) { // Rouge
        this->paintOnImage(event, qRgb(255, 0, 0));
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }

    else if (_numCheckedPoints == 1) { // Vert
        this->paintOnImage(event, qRgb(0, 255, 0));
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }

    else if (_numCheckedPoints == 2) { // Bleu
        this->paintOnImage(event, qRgb(0, 0, 255));
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }

    else if (_numCheckedPoints == 3) { // Jaune
        this->paintOnImage(event, qRgb(255, 255, 0));
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }


    else if (_numCheckedPoints == 4) { // Cyan
        this->paintOnImage(event, qRgb(0, 255, 255));
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }

    else if (_numCheckedPoints == 5) { // Magenta
        this->paintOnImage(event, qRgb(255, 0, 255));
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }

    else if (_numCheckedPoints == 6) { // Blanc
        this->paintOnImage(event, qRgb(255, 255, 255));
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }

    else if (_numCheckedPoints == 7) { // Noir
        this->paintOnImage(event, qRgb(0, 0, 0));
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }

    else if(_numCheckedPoints >= 8) { // Le Maximum des points est atteint
        vector<vector<int> > vect(8, std::vector<int> (2, -1));
        _pointArray  = vect;
        _numCheckedPoints = 0;
        emit resetCurrentImages();
        emit getNumberOfCheckedPoints(_numCheckedPoints);
    }

    this->update();
}

    /// 3.2.2     On dessine le point selectionné sur l'image et on récupère ses coordonnées
    ///           dans un vecteur (_pointArray)
void ImageWidget::paintOnImage(QMouseEvent *event, QRgb pointsColor) {

    if ( ( (int)( (_qtImage->width() - _qtRealImageSize.width() ) / 2)  <= event->x() ) &&
         ( (int)( (_qtImage->width() + _qtRealImageSize.width() ) / 2)  >= event->x() ) &&
         ( (int)( (_qtImage->height() - _qtRealImageSize.height() ) / 2)  <= event->y() ) &&
         ( (int)( (_qtImage->height() + _qtRealImageSize.height() ) / 2)  >= event->y() ) ) {


        _pointArray[_numCheckedPoints][0] = (int)(event->x() - (int)((_qtImage->width() - _qtRealImageSize.width() ) / 2));
        _pointArray[_numCheckedPoints][1] = (int)(event->y() - (int)((_qtImage->height() - _qtRealImageSize.height() ) / 2));


        for (int i = -_pointsOnImageSize; i < _pointsOnImageSize; i++)  {
            for (int j = -_pointsOnImageSize; j < _pointsOnImageSize; j++) {

                _qtImage->setPixel(event->x() + i, event->y() + j, pointsColor);

            }
        }

        _numCheckedPoints ++;
    }

    else {
        _pointArray[_numCheckedPoints][0] = -1;
        _pointArray[_numCheckedPoints][1] = -1;

        QMessageBox::information(this, tr("Mosaic \n"), tr("Cliquer sur l'image svp..."));
        return;
    }

}


/** *****       ************************           fin Actions Utilisateurs               ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/

