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



#include "cvImage.h"
#include "cvsift.h"
#include "cvMatch.h"
#include <QImage>


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



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************         0. Macro : Calcul de la moyenne          ************************       **** **/

/// 0.0     Variables Globales pour la segmentation couleur
static double mean(double param1, double param2) {
    return (double)((param1 + param2)/2);
}


/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************       fin  0. Macro : Calcul de la moyenne       ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            1. Constructeur & Destructeur         ************************       **** **/

/// 1.1     Constructeur
cvImage::cvImage() {
    _numChannels                = 3;

    _cvImageFirst               = 0;
    _cvImageSecond              = 0;

    _cvImageSiftFirst           = 0;
    _cvImageSiftSecond          = 0;

    _cvImageProjectionFirst     = 0;
    _cvImageProjectionSecond    = 0;

    _cvImageMosaic              = 0;

    _alpha                      = 1.0;
    _beta                       = 1.0;
    _gamma                      = 1.0;

    _qtRealImageSize            = QSize(100, 100);
    _cvHomographyMat            = cvCreateMat( 3, 3, CV_32FC1 );

}

/// 1.2     Destructeur
cvImage::~cvImage() {
    cvReleaseImage(&_cvImageSiftFirst);
    cvReleaseImage(&_cvImageSiftSecond);
    cvReleaseImage(&_cvImageFirst);
    cvReleaseImage(&_cvImageSecond);
    cvReleaseImage(&_cvImageProjectionFirst);
    cvReleaseImage(&_cvImageProjectionSecond);

    cvReleaseImage(&_cvImageMosaic);
    cvReleaseMat(&_cvHomographyMat);
}


/** *****       ************************          fin Constructeur & Destructeur          ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************       2.    Communication avec mainWindow        ************************       **** **/

/// 2.1   Traitements communs
    /// 2.1.1   Ouverture de deux images
bool    cvImage::openImage(const char *pathImage1, const char *pathImage2) {
    bool isOpened       = false;
    _cvImageFirst       = cvLoadImage(pathImage1, CV_LOAD_IMAGE_COLOR);
    _cvImageSecond      = cvLoadImage(pathImage2, CV_LOAD_IMAGE_COLOR);


    if(0 == _cvImageFirst || 0 == _cvImageSecond) {
        printf("Could not load image file: %s\n",    pathImage1);
        isOpened        =  false;
    }
    else {

        _cvImageMosaic  = cvCreateImage(cvSize(2*_cvImageFirst->width, _cvImageFirst->height), IPL_DEPTH_8U, 3);
        _cvMatch        = new cvMatch();
        isOpened        = true;
    }

    return isOpened;
}

    /// 2.1.2   Récuperation de la première image
QImage  cvImage::getFirstImage(QSize sizeImage) {
    return this->cvToQt(sizeImage, _cvImageFirst);
}

    /// 2.1.3   Récuperation de la seconde image
QImage  cvImage::getSecondImage(QSize sizeImage) {
    return this->cvToQt(sizeImage, _cvImageSecond);
}

    /// 2.1.4   Récuperation de la vraie taille de l'image
QSize   cvImage::getQtRealImageSize() {
    return _qtRealImageSize;
}

    /// 2.1.5   Paramètres de fondu entre les deux images du panorama (cf. méthode 3.2 processProjection(int))
void    cvImage::setBlurringParams(double alpha, double beta, double gamma) {
    _alpha  = alpha;
    _beta   = beta;
    _gamma  = gamma;
}



/// 2.2   SIFT et Matching entre deux images
    /// 2.2.1   Récupération des caractéristiques SIFT de la premiére image (celle de reference)
QImage  cvImage::processSiftFirst(QSize size, int numOctaves, int numIntervals, double curvatureThreshold, double sigmaAntiAliasing) {

    cvSift *cvSiftFirst    = new cvSift(_cvImageFirst);

    cvSiftFirst->processSift(numOctaves, numIntervals, curvatureThreshold, sigmaAntiAliasing);
    _keyPointsFirst = cvSiftFirst->getKeyPoints();
    _keyDescsListFirst = cvSiftFirst->getDescriptionList();

    _cvImageSiftFirst = cvSiftFirst->getKeyPointsImage();
    return this->cvToQt(size, _cvImageSiftFirst);
}

    /// 2.2.2   Récupération des caractéristiques SIFT de la seconde image (celle projetée)
QImage  cvImage::processSiftSecond(QSize size, int numOctaves, int numIntervals, double curvatureThreshold, double sigmaAntiAliasing) {
    cvSift *cvSiftSecond    = new cvSift(_cvImageSecond);

    cvSiftSecond->processSift(numOctaves, numIntervals, curvatureThreshold, sigmaAntiAliasing);
    _keyPointsSecond = cvSiftSecond->getKeyPoints();
    _keyDescsListSecond = cvSiftSecond->getDescriptionList();

    _cvImageSiftSecond = cvSiftSecond->getKeyPointsImage();
    return this->cvToQt(size, _cvImageSiftSecond);
}

    /// 2.2.3   Récupération des caractéristiques SIFT de la seconde image (celle projetée)
QImage  cvImage::processMatch(QSize sizeImage) {
    return this->cvToQt(sizeImage, _cvMatch->findMatches(_cvImageFirst, _keyDescsListFirst, _cvImageSecond, _keyDescsListSecond));
}


/// 2.3   Création de l'image du panorama de deux images
QImage  cvImage::getMosaicImage(QSize sizeImage) {
    return this->cvToQt(sizeImage, _cvImageMosaic);
}


/** *****       ************************        fin Communication avec mainWindow         ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            3.    Traitements OpenCV              ************************       **** **/

/// 3.1    Calcul de l'homographie
    /// 3.1.1   Via des données SIFT  : Selection automatique
void    cvImage::computeHomography(bool useRansac, double ransacThreshold) {

    vector<Descriptor> vect1, vect2;
    vect1 = _cvMatch->getGoodFeaturesListFirst();
    vect2 = _cvMatch->getGoodFeaturesListSecond();


    cvSetIdentity(_cvHomographyMat);


    if( (vect1.size() > 3) && (vect2.size() > 3) )  {

        CvMat* cvFirstPointArray    = cvCreateMat(  vect1.size(), 3, CV_32FC1 );
        CvMat* cvSecondPointArray   = cvCreateMat(  vect1.size(), 3, CV_32FC1 );
        cvSetIdentity(cvFirstPointArray);
        cvSetIdentity(cvSecondPointArray);


        for (std::size_t i = 0; i < vect1.size() ; i ++) {
            cvmSet(cvFirstPointArray,   i, 0, (double)(vect1[i].xi));
            cvmSet(cvFirstPointArray,   i, 1, (double)(vect1[i].yi));
            cvmSet(cvFirstPointArray,   i, 2, 1.0);

            cvmSet(cvSecondPointArray,   i, 0, (double)(vect2[i].xi));
            cvmSet(cvSecondPointArray,   i, 1, (double)(vect2[i].yi));
            cvmSet(cvSecondPointArray,   i, 2, 1.0);
        }


        if (useRansac == true)  {
            cvFindHomography(    cvSecondPointArray , cvFirstPointArray, _cvHomographyMat, CV_RANSAC, ransacThreshold);
        }

        else if (useRansac == false) {
            cvFindHomography( cvSecondPointArray , cvFirstPointArray, _cvHomographyMat, 0);
        }

        cvReleaseMat(&cvFirstPointArray);
        cvReleaseMat(&cvSecondPointArray);
    } 
}

    /// 3.1.2   Via des données SIFT : Selection manuelle
void    cvImage::computeHomography(vector<vector<int> > firstPointArray, vector<vector<int> > secondPointArray) {

    int size = firstPointArray.size();

     for (std::size_t i = 0; i < firstPointArray.size(); i++) {
         if(firstPointArray[i][0] == -1 && firstPointArray[i][1] == -1) {
            size = i + 1;
            i = firstPointArray.size();
         }
     }

     CvMat* cvFirstPointArray    = cvCreateMat(  size, 3, CV_32FC1 );
     CvMat* cvSecondPointArray   = cvCreateMat(  size, 3, CV_32FC1 );

     cvSetIdentity(cvFirstPointArray);
     cvSetIdentity(cvSecondPointArray);
     cvSetIdentity(_cvHomographyMat);

     if (size > 3) {
         for (int i = 0; i < size; i++) {
             cvmSet(cvFirstPointArray,   i, 0, (double)((double)firstPointArray[i][0])*((double)_cvImageFirst->width/(double)_qtRealImageSize.width()));
             cvmSet(cvFirstPointArray,   i, 1, (double)((double)firstPointArray[i][1])*((double)_cvImageFirst->height/(double)_qtRealImageSize.height()));
             cvmSet(cvFirstPointArray,   i, 2, 1.0);

             cvmSet(cvSecondPointArray,  i, 0, (double)((double)secondPointArray[i][0]*((double)_cvImageFirst->width/(double)_qtRealImageSize.width())));
             cvmSet(cvSecondPointArray,  i, 1, (double)((double)secondPointArray[i][1]*((double)_cvImageFirst->height/(double)_qtRealImageSize.height())));
             cvmSet(cvSecondPointArray,  i, 2, 1.0);
         }

         cvFindHomography( cvSecondPointArray , cvFirstPointArray , _cvHomographyMat, 0);
     }

    cvReleaseMat(&cvFirstPointArray);
    cvReleaseMat(&cvSecondPointArray);
}



/// 3.2    Projection des pixels de la seconde image dans le plan image de la première (celle de reference)
void    cvImage::processProjection(int typeOfBlurring) {

    CvScalar s;
    s.val[0] = 0;
    s.val[1] = 0;
    s.val[2] = 0;

    CvMat* cvIdentity                   = cvCreateMat( 3, 3, CV_32FC1 );
    cvSetIdentity(cvIdentity);

    IplImage *cvImageProjectionFirst    = cvCreateImage(cvSize(2*_cvImageFirst->width, _cvImageFirst->height), IPL_DEPTH_8U, 3);
    IplImage *cvImageProjectionSecond   = cvCreateImage(cvSize(2*_cvImageFirst->width, _cvImageFirst->height), IPL_DEPTH_8U, 3);
    IplImage *cvImageMosaic             = cvCreateImage(cvSize(2*_cvImageFirst->width, _cvImageFirst->height), IPL_DEPTH_8U, 3);



    for(int x=0; x<_cvImageMosaic->width; x++) {
       for(int y=0; y<_cvImageMosaic->height; y++) {

           //On remplace le pixel dans l'image
           cvSet2D(cvImageProjectionFirst, y, x, s);
           cvSet2D(cvImageProjectionSecond, y, x, s);
           cvSet2D(cvImageMosaic, y, x, s);
           cvSet2D(_cvImageMosaic, y, x, s);


       }
   }


    cvWarpPerspective( _cvImageFirst, cvImageProjectionFirst, cvIdentity , CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );

    cvWarpPerspective( _cvImageSecond, cvImageProjectionSecond,  _cvHomographyMat, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );

    if (typeOfBlurring == 0) {   // Max Blurring
        CvScalar cvScalar;

        for(int y=0; y<_cvImageMosaic->height; y++) {
            for(int x=0; x<_cvImageMosaic->width; x++) {

                cvScalar.val[0] = max(cvGet2D(cvImageProjectionFirst,y,x).val[0], cvGet2D(cvImageProjectionSecond,y,x).val[0]);
                cvScalar.val[1] = max(cvGet2D(cvImageProjectionFirst,y,x).val[1], cvGet2D(cvImageProjectionSecond,y,x).val[1]);
                cvScalar.val[2] = max(cvGet2D(cvImageProjectionFirst,y,x).val[2], cvGet2D(cvImageProjectionSecond,y,x).val[2]);

                cvSet2D(cvImageMosaic, y, x, cvScalar);
            }
        }
        _cvImageMosaic = cvCloneImage(cvImageMosaic);
    }

    else if (typeOfBlurring == 1) {   // Mean Blurring
        CvScalar cvScalar;

        for(int y=0; y<_cvImageMosaic->height; y++) {
            for(int x=0; x<_cvImageMosaic->width; x++) {

                cvScalar.val[0] = mean(cvGet2D(cvImageProjectionFirst,y,x).val[0], cvGet2D(cvImageProjectionSecond,y,x).val[0]);
                cvScalar.val[1] = mean(cvGet2D(cvImageProjectionFirst,y,x).val[1], cvGet2D(cvImageProjectionSecond,y,x).val[1]);
                cvScalar.val[2] = mean(cvGet2D(cvImageProjectionFirst,y,x).val[2], cvGet2D(cvImageProjectionSecond,y,x).val[2]);

                cvSet2D(cvImageMosaic, y, x, cvScalar);
            }
        }
        _cvImageMosaic = cvCloneImage(cvImageMosaic);
    }

    else if (typeOfBlurring == 2) {// Weight Blurring
        cvAddWeighted( cvImageProjectionFirst, _alpha, cvImageProjectionSecond, _beta,  _gamma, cvImageMosaic );
        _cvImageMosaic = cvCloneImage(cvImageMosaic);
    }

    cvReleaseImage(&cvImageMosaic);
    cvReleaseImage(&cvImageProjectionFirst);
    cvReleaseImage(&cvImageProjectionSecond);
    cvReleaseMat(&cvIdentity);

}


/** *****       ************************              fin Traitements OpenCV              ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************           4.    Conversion OpenCV - Qt            ************************       **** **/

/// 4.1.   Convetit une image OpenCV (IplImage) en image Qt (qtImage)
QImage  cvImage::cvToQt(QSize sizeLabelArea, IplImage *image) {

    QImage qtImage;
    QImage dummy(sizeLabelArea, QImage::Format_RGB32);
    dummy.fill(qRgb(204, 204, 204));

    switch (image->depth) {

        case IPL_DEPTH_8U:
            switch (image->nChannels) {
                case 1:
                    qtImage = QImage(image->width, image->height, QImage::Format_RGB32);
                    for (int y = 0; y < image->height; y++) {
                        for (int x = 0; x < image->width; x++) {

                            qtImage.setPixel(x, y, qRgb(cvGet2D(image,y,x).val[0],
                                                        cvGet2D(image,y,x).val[0],
                                                        cvGet2D(image,y,x).val[0]));
                        }
                    }

                    qtImage = qtImage.scaled(sizeLabelArea*0.98, Qt::KeepAspectRatio, Qt::FastTransformation);
                    for (int y = 0; y < qtImage.height(); y++) {
                           for (int x = 0; x < qtImage.width(); x++) {
                           dummy.setPixel( (int)((dummy.width() - qtImage.width()) / 2) + x,  (int)((dummy.height() - qtImage.height()) / 3) + y, qtImage.pixel(x, y));
                         }
                     }
                break;

                case 3:
                    qtImage = QImage(image->width, image->height, QImage::Format_RGB32);
                    for (int y = 0; y < image->height; y++) {
                        for (int x = 0; x < image->width; x++) {
                            qtImage.setPixel(x, y, qRgb(cvGet2D(image,y,x).val[2],
                                                        cvGet2D(image,y,x).val[1],
                                                        cvGet2D(image,y,x).val[0]));
                        }
                    }

                    qtImage = qtImage.scaled(sizeLabelArea*0.98, Qt::KeepAspectRatio, Qt::FastTransformation);

                    _qtRealImageSize.setHeight(qtImage.height());
                    _qtRealImageSize.setWidth(qtImage.width());


                    for (int y = 0; y < qtImage.height(); y++) {
                           for (int x = 0; x < qtImage.width(); x++) {
                           dummy.setPixel( (int)((dummy.width() - qtImage.width()) / 2) + x,  (int)((dummy.height() - qtImage.height()) / 3) + y, qtImage.pixel(x, y));
                         }
                     }
                break;

                default:
                   printf("This number of channels is not supported\n");
               break;
            }
        break;

        default:
            printf("This type of IplImage is not implemented in QOpenCVWidget\n");
        break;
    }

    return dummy;
}


/** *****       ************************            fin Conversion OpenCV - Qt            ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/
