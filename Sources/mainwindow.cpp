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


#include "mainwindow.h"


            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                Classe mainWindow                                               **** **/
            /** *****                                                                                                                **** **/
            /** *****                   Création de l'interface utilisateur, communication entre les widgets et récuperation         **** **/
            /** *****                    des actions utilisateurs                                                                    **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Construction de l'interface                                               **** **/
            /** *****                                   3. Actions Utilisateurs                                                      **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            1. Constructeur & Destructeur         ************************       **** **/

/// 1.1     Constructeur
/**    *****   *****   *****   *****   **/
/** !! CHANGER LE PATH DU LOGO ICI  !! **/
/**    *****   *****   *****   *****   **/
MainWindow::MainWindow() {
    _imageIsOpen                    = false;
    _colorIsChosen                  = false;

    _sizeImageLabelArea             = QSize(420, 300);
    _sizeImagePanoramaLabelArea     = QSize(848, 300);
    _sizeTextLabelArea              = QSize(300, 300);


    _centralWidget                  = new QWidget(this);
    _textWidget                     = new TextWidget(this,      _sizeTextLabelArea);
    _firstImageWidget               = new ImageWidget(this,     _sizeImageLabelArea);
    _secondImageWidget              = new ImageWidget(this,     _sizeImageLabelArea);
    _panoramaImageWidget            = new ImageWidget(this,     _sizeImagePanoramaLabelArea);
    _cvImage                        = new cvImage;

    _firstImageArea                 = new QScrollArea(this);
    _secondImageArea                = new QScrollArea(this);
    _textWidgetArea                   = new QScrollArea(this);
    _panoramaImageArea              = new QScrollArea(this);

    _centralLayout                  = new QGridLayout();
    _controlLayout                  = new QGridLayout();


    // Logo Ets
    /**    *****   *****   *****   *****   **/
    /** !! CHANGER LE PATH DU LOGO ICI  !! **/
    /**    *****   *****   *****   *****   **/
    _logo                           = new QLabel(this);
    _logo->setPixmap(QPixmap("/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/Image/logo.png"));
    _logo->setGeometry(40, 20, 300, 230);


    /// Frames
    _frame0                         = new QFrame(this);
    _frame1                         = new QFrame(this);
    _frame2                         = new QFrame(this);



    /// 1. Choix des Taches (Mise en correspondance ou Panorama)
        _tasksComboBox                  = new QComboBox(this);
        _tasksComboBox->addItem("Mise en correspondance");
        _tasksComboBox->addItem("Panorama");
        _tasksComboBox->setCurrentIndex(0);
        QObject::connect(_tasksComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(hideSliders()));


        _resetParamsButton              = new QPushButton("Reinitialisation", this);
        QObject::connect(_resetParamsButton,SIGNAL(clicked()), this, SLOT(resetCurrentImages()));

        _tasksLayout                    = new QGridLayout();



    /// 2. Mise en correspondance
        _pointsSelectionLabel           = new QLabel("Selection des points : ", this);

        _pointsSelectionComboBox        = new QComboBox(this);
        _pointsSelectionComboBox->addItem("Automatique");
        _pointsSelectionComboBox->addItem("Manuelle");
        _pointsSelectionComboBox->setCurrentIndex(0);
        QObject::connect(_pointsSelectionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(hideSliders()));

        _pointsSelectionLayout          = new QGridLayout();


        /// 2.1 Automatique
            _numberOctavesLabel             = new QLabel("Octaves : ", this);
            _numberIntervalsLabel           = new QLabel("Intervales : ", this);
            _numberOctavesBox               = new QSpinBox(this);
            _numberOctavesBox->setRange(0, 6);
            _numberOctavesBox->setValue(4);
            QObject::connect(_numberOctavesBox, SIGNAL(valueChanged(int)), this, SLOT(processFeaturesMatching()));
            _numberIntervalsBox             = new QSpinBox(this);
            _numberIntervalsBox->setRange(0, 4);
            _numberIntervalsBox->setValue(2);
            QObject::connect(_numberIntervalsBox, SIGNAL(valueChanged(int)), this, SLOT(processFeaturesMatching()));

            _contrastThresholdLabel         = new QLabel("Seuil de contraste : 0.005", this);
            _contrastThresholdSlider        = createSlider(SLOT(setSlidersParams()), 1, 100, 5);
            _sigmaAntiAliasingLabel          = new QLabel("Sigma Anti-Aliasing : 0.5", this);
            _sigmaAntiAliasingSlider        = createSlider(SLOT(setSlidersParams()), 1, 10, 5);


        /// 2.2 Manuel
            _pointsSizeLabel                = new QLabel("Taille des points : 9", this);
            _pointsSizeSlider               = createSlider(SLOT(setPointSize()), 1, 4, 4);



    /// 3. Panorama
        _useRansac                      = new QCheckBox("Ransac", this);
        _useRansac->click();
        QObject::connect(_useRansac, SIGNAL(clicked()), this, SLOT(processMosaic()));

        _ransacLabel                    = new QLabel("Seuil : 10", this);
        _ransacSlider                   = createSlider(SLOT(processMosaic()), 1, 100, 100);

        _typeOfBlurringLabel            = new QLabel("Types de Fondu : ");
        _typeOfBlurring                 = new QComboBox(this);
        _typeOfBlurring->addItem("Maximum");
        _typeOfBlurring->addItem("Moyen");
        _typeOfBlurring->addItem("Pondere");
        _typeOfBlurring->setCurrentIndex(0);
        QObject::connect(_typeOfBlurring, SIGNAL(currentIndexChanged(int)), this, SLOT(hideSliders()));

        _alphaLabel                     = new QLabel("Alpha : 1.0", this);
        _betaLabel                      = new QLabel("Beta  : 1.0", this);
        _gammaLabel                     = new QLabel("Gamma : 1.0", this);
        _alphaSlider                    = createSlider(SLOT(setSlidersParams()), 0, 200, 100);
        _betaSlider                     = createSlider(SLOT(setSlidersParams()), 0, 200, 100);
        _gammaSlider                    = createSlider(SLOT(setSlidersParams()), -500, 500, 0);

        _pamoramaLayout                 = new QGridLayout();


        _pointsSizeLabel->hide();
        _pointsSizeSlider->hide();
        _useRansac->hide();
        _ransacLabel->hide();
        _ransacSlider->hide();
        _typeOfBlurringLabel->hide();
        _typeOfBlurring->hide();
        _alphaSlider->hide();
        _alphaLabel->hide();
        _betaSlider->hide();
        _betaLabel->hide();
        _gammaSlider->hide();
        _gammaLabel->hide();


    QObject::connect(_firstImageWidget, SIGNAL(resetCurrentImages()), this, SLOT(resetCurrentImages()));
    QObject::connect(_secondImageWidget, SIGNAL(resetCurrentImages()), this, SLOT(resetCurrentImages()));


    this->setCentralWidget(_centralWidget);
    this->createActions();
    this->createMenus();
    this->createUI();
    this->setWindowTitle(tr("Mosaique"));
    this->resize(1200, 640);
    this->setFixedSize(1200, 640);
}

/// 1.2     Destructeur
MainWindow::~MainWindow() {
    delete(_cvImage);

    delete(_alphaSlider);
    delete(_betaSlider);
    delete(_gammaSlider);
    delete(_pointsSizeSlider);
    delete(_ransacSlider);
    delete(_contrastThresholdSlider);
    delete(_sigmaAntiAliasingSlider);


    delete(_frame0);
    delete(_frame1);
    delete(_frame2);


    delete(_pointsSelectionLabel);
    delete(_contrastThresholdLabel);
    delete(_sigmaAntiAliasingLabel);
    delete(_ransacLabel);
    delete(_numberOctavesLabel);
    delete(_numberIntervalsLabel);
    delete(_logo);
    delete(_alphaLabel);
    delete(_betaLabel);
    delete(_gammaLabel);
    delete(_typeOfBlurringLabel);
    delete(_pointsSizeLabel);


    delete(_numberOctavesBox);
    delete(_numberIntervalsBox);
    delete(_typeOfBlurring);
    delete(_tasksComboBox);
    delete(_pointsSelectionComboBox);
    delete(_useRansac);
    delete(_resetParamsButton);



    delete(_panoramaImageWidget);
    delete(_firstImageWidget);
    delete(_secondImageWidget);
    delete(_textWidget);

    delete(_firstImageArea);
    delete(_secondImageArea);
    delete(_textWidgetArea);
    delete(_panoramaImageArea);
}


/** *****       ************************          fin Constructeur & Destructeur          ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************         2.   Construction de l'interface         ************************       **** **/

/// 2.1     Création de l'interface utilisateur
void MainWindow::createUI() {
// Widgets
    _firstImageArea->setWidget(_firstImageWidget);
    _firstImageArea->setWidgetResizable(true);
    _firstImageArea->setMinimumSize(_sizeImageLabelArea);
    _firstImageArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    _secondImageArea->setWidget(_secondImageWidget);
    _secondImageArea->setMinimumSize(_sizeImageLabelArea);
    _secondImageArea->setWidgetResizable(true);
    _secondImageArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    _textWidgetArea->setWidget(_textWidget);
    _textWidgetArea->setWidgetResizable(true);
    _textWidgetArea->setMinimumSize(_sizeTextLabelArea);
    _textWidgetArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);



    _panoramaImageArea->setWidget(_panoramaImageWidget);
    _panoramaImageArea->setMinimumSize(_sizeImagePanoramaLabelArea);
    _panoramaImageArea->setWidgetResizable(true);
    _panoramaImageArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);




    _frame0->setFrameShape(QFrame::HLine);
    _frame0->setFrameShadow(QFrame::Raised);
    _frame1->setFrameShape(QFrame::HLine);
    _frame1->setFrameShadow(QFrame::Raised);
    _frame2->setFrameShape(QFrame::HLine);
    _frame2->setFrameShadow(QFrame::Sunken);



 /// Layouts
    /// 1. Choix des Taches (Mise en correspondance ou Panorama)
        _tasksLayout->addWidget(_tasksComboBox,                         0, 0, 1, 2);
        _tasksLayout->addWidget(_resetParamsButton,                     0, 2, 1, 2);

   /// 2. Mise en correspondance
        _pointsSelectionLayout->addWidget(_pointsSelectionLabel,        0, 0, 1, 2);
        _pointsSelectionLayout->addWidget(_pointsSelectionComboBox,     0, 2, 1, 2);

        /// 2.1 Automatic
            _pointsSelectionLayout->addWidget(_numberOctavesLabel,      1, 0, 1, 1);
            _pointsSelectionLayout->addWidget(_numberOctavesBox,        1, 1, 1, 1);
            _pointsSelectionLayout->addWidget(_numberIntervalsLabel,    1, 2, 1, 1);
            _pointsSelectionLayout->addWidget(_numberIntervalsBox,      1, 3, 1, 1);

            _pointsSelectionLayout->addWidget(_contrastThresholdLabel,  2, 0, 1, 2);
            _pointsSelectionLayout->addWidget(_contrastThresholdSlider, 2, 2, 1, 2);

            _pointsSelectionLayout->addWidget(_sigmaAntiAliasingLabel,  3, 0, 1, 2);
            _pointsSelectionLayout->addWidget(_sigmaAntiAliasingSlider, 3, 2, 1, 2);

       /// 2.2 Manual
            _pointsSelectionLayout->addWidget(_pointsSizeLabel,         1, 0, 1, 2);
            _pointsSelectionLayout->addWidget(_pointsSizeSlider,        1, 2, 1, 2);

    /// 3. Panorama
        _pamoramaLayout->addWidget(_useRansac,                          0, 0, 1, 2);

        _pamoramaLayout->addWidget(_ransacLabel,                        1, 0, 1, 2);
        _pamoramaLayout->addWidget(_ransacSlider,                       1, 2, 1, 2);

        _pamoramaLayout->addWidget(_typeOfBlurringLabel,                2, 0, 1, 2);
        _pamoramaLayout->addWidget(_typeOfBlurring,                     2, 2, 1, 2);

        _pamoramaLayout->addWidget(_alphaLabel,                         3, 0, 1, 2);
        _pamoramaLayout->addWidget(_alphaSlider,                        3, 2, 1, 2);

        _pamoramaLayout->addWidget(_betaLabel,                          4, 0, 1, 2);
        _pamoramaLayout->addWidget(_betaSlider,                         4, 2, 1, 2);

        _pamoramaLayout->addWidget(_gammaLabel,                         5, 0, 1, 2);
        _pamoramaLayout->addWidget(_gammaSlider,                        5, 2, 1, 2);


   /// 4. Final
        _controlLayout->addWidget(_frame0,                              0, 0);
        _controlLayout->addLayout(_tasksLayout,                         1, 0);
        _controlLayout->addWidget(_frame1,                              2, 0);
        _controlLayout->addLayout(_pointsSelectionLayout,               3, 0);
        _controlLayout->addLayout(_pamoramaLayout,                      3, 0);
        _controlLayout->addWidget(_frame2,                              4 ,0);
        _controlLayout->addWidget(_logo,                                5, 0);




    _centralLayout->addWidget(_firstImageArea,                          0, 0);
    _centralLayout->addWidget(_secondImageArea,                         0, 1);
    _centralLayout->addWidget(_textWidgetArea,                            0, 2);
    _centralLayout->addWidget(_panoramaImageArea,                       1, 0, 1, 2);
    _centralLayout->addLayout(_controlLayout,                           1, 2);
    _centralWidget->setLayout(_centralLayout);
}

/// 2.2     Création des Actions du menu
void MainWindow::createActions() {

    openImageAct = new QAction(tr("&Ouvrir deux images..."), this);
    openImageAct->setShortcut(tr("Ctrl+O"));
    connect(openImageAct, SIGNAL(triggered()), this, SLOT(openImage()));

    exitAct = new QAction(tr("Quitter"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&A propos de Mosaic"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

/// 2.3     Création du menu
void MainWindow::createMenus()  {
    fileMenu = this->menuBar()->addMenu(tr("&Fichier"));
    fileMenu->addAction(openImageAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = this->menuBar()->addMenu(tr("&Aide"));
    helpMenu->addAction(aboutAct);
}

/// 2.4     Construction de Sliders
QSlider *MainWindow::createSlider(const char *setterSlot, int minValue, int maxValue, int currentValue) {
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(minValue, maxValue);
    slider->setValue(currentValue);
    slider->setSingleStep(1);
    slider->setPageStep(1);
    slider->setTickInterval(20);
    slider->setTickPosition(QSlider::TicksRight);
    QObject::connect( slider, SIGNAL(valueChanged(int)), this, setterSlot);

    return slider;
}


/** *****       ************************          fin Construction de l'interface         ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************         3.     Actions Utilisateurs              ************************       **** **/

/// 3.1     Actions des menus
    /// 3.1.1     Ouverture de deux images
void MainWindow::openImage() {
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::currentPath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("Toutes les image (*.jpg *.pgm *.pgn)");
    QStringList fileNames;
    if (dialog.exec())
            fileNames = dialog.selectedFiles();

    if (fileNames.size() == 2) {
        if (!fileNames.isEmpty()) {
            _imageIsOpen = _cvImage->openImage(fileNames.at(0).toLocal8Bit().constData(), fileNames.at(1).toLocal8Bit().constData());

            if (_imageIsOpen == true) {

                _firstImageWidget->setImage(_cvImage->getFirstImage(_sizeImageLabelArea));
                _firstImageWidget->setQtRealImageSize(_cvImage->getQtRealImageSize());

                _secondImageWidget->setImage(_cvImage->getSecondImage(_sizeImageLabelArea));
                _secondImageWidget->setQtRealImageSize(_cvImage->getQtRealImageSize());

                this->setSlidersParams();
            }
            else {
                QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load by OpenCV."));
                return;
            }
        }
    }
}

    /// 3.1.2     Actions des menus
void MainWindow::about() {
    QMessageBox::about(this, tr("A propos de Mosaic"),
            tr("The <b>Grabber</b> example demonstrates two approaches for "
               "rendering OpenGL into a Qt pixmap."));
}



/// 3.2     Actions Utilisateurs : Traitement OpenCV
    /// 3.2.1     Reinitialisation des images lors d'un changement de selection des points (manuel - Automatique)
    ///           ou lors du passage de la séléction des points au panorama et inversement
void MainWindow::resetCurrentImages() {
    _firstImageWidget->resetPointsSelection();
    _secondImageWidget->resetPointsSelection();
    _firstImageWidget->setImage(_cvImage->getFirstImage(_sizeImageLabelArea));
    _secondImageWidget->setImage(_cvImage->getSecondImage(_sizeImageLabelArea));
    _firstImageWidget->setQtRealImageSize(_cvImage->getQtRealImageSize());
    _secondImageWidget->setQtRealImageSize(_cvImage->getQtRealImageSize());

    _panoramaImageWidget->resetImage();
}

    /// 3.2.2     Cache/Decouvre les sliders lors d'un changement de selection des points ou (manuel - Automatique)
    ///           ou lors du passage de la séléction des points au panorama et inversement (texte, etc ...)
void MainWindow::hideSliders() {

    if (_tasksComboBox->currentIndex() == 0) { // Selection des points
        _useRansac->hide();
        _ransacLabel->hide();
        _ransacSlider->hide();
        _typeOfBlurringLabel->hide();
        _typeOfBlurring->hide();

        _alphaSlider->hide();
        _alphaLabel->hide();
        _betaSlider->hide();
        _betaLabel->hide();
        _gammaSlider->hide();
        _gammaLabel->hide();


        _pointsSelectionLabel->show();
        _pointsSelectionComboBox->show();

        // Automatique
         if (_pointsSelectionComboBox->currentIndex() == 0)  {
             _pointsSizeLabel->hide();
             _pointsSizeSlider->hide();


             _numberOctavesLabel->show();
             _numberIntervalsLabel->show();
             _numberOctavesBox->show();
             _numberIntervalsBox->show();

             _contrastThresholdLabel->show();
             _contrastThresholdSlider->show();
             _sigmaAntiAliasingLabel->show();
             _sigmaAntiAliasingSlider->show();

             this->processFeaturesMatching();
         }

         // Manuelle
         else if (_pointsSelectionComboBox->currentIndex() == 1) {
             _numberOctavesLabel->hide();
             _numberOctavesBox->hide();
             _numberIntervalsLabel->hide();
             _numberIntervalsBox->hide();

             _contrastThresholdLabel->hide();
             _contrastThresholdSlider->hide();
             _sigmaAntiAliasingLabel->hide();
             _sigmaAntiAliasingSlider->hide();


             _pointsSizeLabel->show();
             _pointsSizeSlider->show();

             this->resetCurrentImages();
         }
    }


    else  if (_tasksComboBox->currentIndex() == 1) { //Panorama

        _pointsSelectionLabel->hide();
        _pointsSelectionComboBox->hide();

        _pointsSizeLabel->hide();
        _pointsSizeSlider->hide();


        _numberOctavesLabel->hide();
        _numberIntervalsLabel->hide();
        _numberOctavesBox->hide();
        _numberIntervalsBox->hide();

        _contrastThresholdLabel->hide();
        _contrastThresholdSlider->hide();
        _sigmaAntiAliasingLabel->hide();
        _sigmaAntiAliasingSlider->hide();

        _useRansac->show();
        _ransacLabel->show();
        _ransacSlider->show();
        _typeOfBlurringLabel->show();
        _typeOfBlurring->show();


        if(_typeOfBlurring->currentIndex() == 2) {
            _alphaSlider->show();
            _alphaLabel->show();

            _betaSlider->show();
            _betaLabel->show();

            _gammaSlider->show();
            _gammaLabel->show();
        }

        else {
            _alphaSlider->hide();
            _alphaLabel->hide();

            _betaSlider->hide();
            _betaLabel->hide();

            _gammaSlider->hide();
            _gammaLabel->hide();
        }

        this->processMosaic();
    }

}

    /// 3.2.3     Change les paramètres des sliders lors d'un changement de selection des points ou (manuel - Automatique)
    ///           ou lors du passage de la séléction des points au panorama et inversement (texte, etc ...)
void MainWindow::setSlidersParams() {

  if(_imageIsOpen == true) {

      if(_tasksComboBox->currentIndex() == 0) {

          _variant.setValue((double)_contrastThresholdSlider->value()/1000);
          _contrastThresholdLabel->setText("Seuil de contraste : " + _variant.toString());

          _variant.setValue((double)_sigmaAntiAliasingSlider->value()/10);
          _sigmaAntiAliasingLabel->setText("Sigma Anti-Aliasing : " + _variant.toString());

          this->processFeaturesMatching();
      }

      else if (_tasksComboBox->currentIndex() == 1) {
          if(_typeOfBlurring->currentIndex() == 2) {
              _cvImage->setBlurringParams((double)_alphaSlider->value()/100,
                                          (double)_betaSlider->value()/100,
                                          (double)_gammaSlider->value()/10);

              _variant.setValue((double)_alphaSlider->value()/100);
              _alphaLabel->setText("Alpha : " + _variant.toString());

              _variant.setValue((double)_betaSlider->value()/100);
              _betaLabel->setText("Beta  : " + _variant.toString());

              _variant.setValue((double)_gammaSlider->value()/10);
              _gammaLabel->setText("Gamma : " + _variant.toString());
          }

         else {
             _alphaSlider->setValue(_alphaSlider->value());
             _variant.setValue((double)_alphaSlider->value()/100);
             _alphaLabel->setText("Alpha : " + _variant.toString());

             _betaSlider->setValue(_betaSlider->value());
             _variant.setValue((double)_betaSlider->value()/100);
             _betaLabel->setText("Beta  : " + _variant.toString());

             _gammaSlider->setValue(_gammaSlider->value());
             _variant.setValue((double)_gammaSlider->value()/10);
             _gammaLabel->setText("Gamma : " + _variant.toString());
         }

            this->processMosaic();
       }
    }
}

    /// 3.2.4     Tailles des points lors de la séléction manuelle
void MainWindow::setPointSize() {
    if(_imageIsOpen == true) {
        _firstImageWidget->setPointsOnImageSize(_pointsSizeSlider->value());
        _secondImageWidget->setPointsOnImageSize(_pointsSizeSlider->value());

        _pointsSizeLabel->setText("Points Size : " + QString::number(2*_pointsSizeSlider->value() + 1));
    }
}

    /// 3.2.5     SIFT -- APPEL AUX FONCTIONS OPENCV DE LA CLASSE cvImage
void MainWindow::processFeaturesMatching() {
    this->_firstImageWidget->setImage(_cvImage->processSiftFirst(_sizeImageLabelArea, _numberOctavesBox->value(), _numberIntervalsBox->value(),
                                                                 (double)_contrastThresholdSlider->value()/1000, (double)_sigmaAntiAliasingSlider->value()/10));
    this->_secondImageWidget->setImage(_cvImage->processSiftSecond(_sizeImageLabelArea,
                                                                   _numberOctavesBox->value(), _numberIntervalsBox->value(),
                                                                   (double)_contrastThresholdSlider->value()/1000, (double)_sigmaAntiAliasingSlider->value()/10));

    _panoramaImageWidget->setImage(_cvImage->processMatch(_sizeImagePanoramaLabelArea));
}

    /// 3.2.6     MOSAIC -- APPEL AUX FONCTIONS OPENCV DE LA CLASSE cvImage
void MainWindow::processMosaic() {
    if(_imageIsOpen == true) {

        if (_tasksComboBox->currentIndex() == 1)  {  // Panorama

            if (_pointsSelectionComboBox->currentIndex() == 0)  {  // Automatique
                _useRansac->setEnabled(true);
                _ransacLabel->show();
                _ransacSlider->show();
                if (_useRansac->isChecked()) {
                    _variant.setValue((double)_ransacSlider->value()/10);
                    _ransacLabel->setText("Threshold : " + _variant.toString());
                }
                else {
                    _ransacLabel->hide();
                    _ransacSlider->hide();
                }

                this->resetCurrentImages();

                _cvImage->computeHomography(_useRansac->isChecked(), (double)_ransacSlider->value()/10);
                _cvImage->processProjection(_typeOfBlurring->currentIndex());
                _panoramaImageWidget->setImage(_cvImage->getMosaicImage(_sizeImagePanoramaLabelArea));
            }

            else if (_pointsSelectionComboBox->currentIndex() == 1)  {  // Manuelle

                _ransacLabel->hide();
                _ransacSlider->hide();
                _useRansac->setDisabled(true);

                _cvImage->computeHomography(_firstImageWidget->getPointsArray(), _secondImageWidget->getPointsArray());
                _cvImage->processProjection(_typeOfBlurring->currentIndex());
                _panoramaImageWidget->setImage(_cvImage->getMosaicImage(_sizeImagePanoramaLabelArea));
            }
        }
    }
}


/** *****       ************************           fin Actions Utilisateurs               ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/




