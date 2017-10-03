# /* ******       ************************        **********************************        ************************       ***** */
# /*                                                                                                                             */
# /* ******       Informations :                                                                                           ***** */
# /*              Code crée par Julien Couet pour le cour de maitrise MTI880 de l'ETS (http://www.etsmtl.ca/) encadré par        */
# /*              le professeur Luc Duong   http://profs.etsmtl.ca/lduong/cours/mti880/                                          */
# /*                                                                                                                             */
# /* ******       Licence, Droits et Garantie :                                                                            ***** */
# /*              Ce code est libre de tous droits dans la limite de ceux de OpenCV : http://opencv.willowgarage.com/wiki/       */
# /*              qui est sous licence 'BSD' :  http://en.wikipedia.org/wiki/BSD_licenses                                        */
# /*                                                                                                                             */
# /* ******       Contacts :                                                                                               ***** */
# /*              couet.julien@gmail.com                                                                                         */
# /*                                                                                                                             */
# /* ******       ************************        **********************************        ************************       ***** */


QT       += core \
            widgets

QT       -= gui



HEADERS         +=  mainwindow.h \
                    cvimage.h \
                    imagewidget.h \
                    textwidget.h \
                    cvsift.h \
                    Keypoint.h \
                    Descriptor.h \
                    cvmatch.h

SOURCES         +=  main.cpp \
                    mainwindow.cpp \
                    cvimage.cpp \
                    imagewidget.cpp \
                    textwidget.cpp \
                    cvsift.cpp \
                    cvmatch.cpp \



# ICI METTRE LES PATHS OPENCV
# Ici mettre le path vers les Headers OpenCV
#headers         =   opencv/build/include/opencv \
#                    opencv/build/include/

# Ici mettre le path vers les Librairies OpenCV

CONFIG   += console

TEMPLATE = app

INCLUDEPATH += D:\opencv\release\install\include

LIBS += D:\opencv\release\bin\libopencv_core2410.dll \
        D:\opencv\release\bin\libopencv_contrib2410.dll \
        D:\opencv\release\bin\libopencv_highgui2410.dll \
        D:\opencv\release\bin\libopencv_imgproc2410.dll \
        D:\opencv\release\bin\libopencv_calib3d2410.dll \
        D:\opencv\release\bin\libopencv_features2d2410.dll \
        D:\opencv\release\bin\libopencv_flann2410.dll \
        D:\opencv\release\bin\libopencv_gpu2410.dll \
        D:\opencv\release\bin\libopencv_legacy2410.dll \
        D:\opencv\release\bin\libopencv_ml2410.dll \
        D:\opencv\release\bin\libopencv_objdetect2410.dll \
        D:\opencv\release\bin\libopencv_video2410.dll \
        D:\opencv\release\bin\libopencv_nonfree2410.dll
