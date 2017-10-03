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




#include "cvsift.h"


#define SIGMA_PREBLUR			1.0
#define CURVATURE_THRESHOLD		5.0
#define _PI                             3.1415926535897932384626433832795
#define NUM_BINS                        36
#define MAX_KERNEL_SIZE			20
#define FEATURE_WINDOW_SIZE		16
#define DESC_NUM_BINS			8
#define FVSIZE				128
#define	FV_THRESHOLD			0.2



            /** *****       ************************        **********************************        ************************       **** **/
            /** *****                                                                                                                **** **/
            /** *****                                                  Classe cvSift                                                 **** **/
            /** *****                                                                                                                **** **/
            /** *****                   Trouve les KeyPoints SIFT d'une image et calcul les Descripteurs de chacun                   **** **/
            /** *****                                                                                                                **** **/
            /** *****                                   0. Macro : Calcul de la moyenne (mean)                                       **** **/
            /** *****                                   1. Constructeur & Destructeur                                                **** **/
            /** *****                                   2. Communication avec cvImage                                                **** **/
            /** *****                                   3. Traitements OpenCV                                                        **** **/
            /** *****                                                                                                                **** **/
            /** *****       ************************        **********************************        ************************       **** **/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************         0. Macro : Calcul de la moyenne          ************************       **** **/

/// 0.0     Fonction permettant de sauver une image OpenCV sous le path 'filename'
static void SaveFloatingPointImage(const char *filename, IplImage* img) {
        IplImage* dup = cvCreateImage(cvGetSize(img), 8, 1);
        cvCvtScale(img, dup, 255.0);

        cvSaveImage(filename, dup);
        cvReleaseImage(&dup);
}


/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************       fin  0. Macro : Calcul de la moyenne       ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            1. Constructeur & Destructeur         ************************       **** **/

/// 1.1     Constructeur
cvSift::cvSift(const IplImage* cvImage) {

     _cvCurrentImage = cvCloneImage(cvImage);

     _contrastThreshold = 0.005;
     _sigmaAntiAliasing = 0.5;
}


/// 1.2 Initialisation des tableaux 2D IplImage
void cvSift::createWorking2DArrays()  {
    // Create a 2D array of gaussian blurred images
    _cvGaussianImages2DArray = new IplImage**[_numOctaves];
    for(int i=0;i<_numOctaves;i++)
            _cvGaussianImages2DArray[i] = new IplImage*[_numIntervals+3];

    // Create a 2D array to store images generated after the
    // DoG operation
    _cvDOGImages2DArray = new IplImage**[_numOctaves];
    for(int i=0;i<_numOctaves;i++)
            _cvDOGImages2DArray[i] = new IplImage*[_numIntervals+2];

    // Create a 2D array that will hold if a particular point
    // is an extrema or not
    _cvBinarieImages2DArray = new IplImage**[_numOctaves];
    for(int i=0;i<_numOctaves;i++)
            _cvBinarieImages2DArray[i] = new IplImage*[_numIntervals];

    // Create a 2D array of decimal numbers. It holds the sigma
    // used to blur the gaussian images.
    _sigma2DArray = new double*[_numOctaves];
    for(int  i=0;i<_numOctaves;i++)
            _sigma2DArray[i] = new double[_numIntervals+3];
}


/// 1.3 Destructeur
cvSift::~cvSift() {
    for(int i = 0; i < _numOctaves; i++)    {

            // Release all images in that particular octave
        for(int j=0;j<_numIntervals+3;j++)	{
            cvReleaseImage(&_cvGaussianImages2DArray[i][j]);
        }

        for(int j = 0; j < _numIntervals+2; j++) {
            cvReleaseImage(&_cvDOGImages2DArray[i][j]);
        }

        for(int j=0;j<_numIntervals;j++)  {
            cvReleaseImage(&_cvBinarieImages2DArray[i][j]);
        }

        // Delete memory for that array
        delete [] _cvGaussianImages2DArray[i];
        delete [] _cvDOGImages2DArray[i];
        delete [] _cvBinarieImages2DArray[i];
        delete [] _sigma2DArray[i];
    }

    // Delete the 2D arrays
    delete [] _cvGaussianImages2DArray;
    delete [] _cvDOGImages2DArray;
    delete [] _cvBinarieImages2DArray;
    delete [] _sigma2DArray;
}


/** *****       ************************          fin Constructeur & Destructeur          ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************          2.    Communication avec cvImage        ************************       **** **/

/// 2.1     Calcul des descripteurs SIFT d'une image
void cvSift::processSift(int numOctaves, int numIntervals, double contrastThreshold, double sigmaAntiAliasing){//, double contrastThreshold) {

    _numOctaves     = numOctaves;
    _numIntervals   = numIntervals;

    _contrastThreshold = contrastThreshold;
    _sigmaAntiAliasing = sigmaAntiAliasing;


    this->createWorking2DArrays();
    this->BuildScaleSpace();
    this->processExtremasDetection();
    this->processOrientation();
    this->extractKeypointDescriptors();
}

/// 2.2     Retourne l'image initiale sur laquelle sont dessinés les KeyPoints
IplImage *cvSift::getKeyPointsImage( ) {

    IplImage* _cvKeyPointImage = cvCloneImage(_cvCurrentImage);

    // cout << endl << endl << " getKeyPointsImage " << endl << endl;

    for(int i=0;i<_numKeypoints;i++)    {

            Keypoint kp = _keyPoints[i];

            cvLine(_cvKeyPointImage, cvPoint(kp.xi, kp.yi), cvPoint(kp.xi, kp.yi), CV_RGB(255,255,255), 3);
            cvLine(_cvKeyPointImage, cvPoint(kp.xi, kp.yi), cvPoint(kp.xi+10*cos(kp.orien[0]), kp.yi+10*sin((double)kp.orien[0])), CV_RGB(255,255,255), 1);
    }

    return  _cvKeyPointImage;
}


/// 2.3     Récupération des tableaux (vecteurs) des KeyPoints et des Descripteurs SIFT
    /// 2.3.1   KeyPoints
vector<Keypoint> cvSift::getKeyPoints() {
    return _keyPoints;
}

    /// 2.3.2   Descripteurs
vector<Descriptor> cvSift::getDescriptionList() {
    return _keyDescs;
}

    /// 2.3.3   Nombre de Keypoints trouvés
int cvSift::getNumKeyPoints() {
    return _numKeypoints;
}


/** *****       ************************          fin Communication avec cvImage          ************************       **** **/
/** *****       ************************        **********************************        ************************       **** **/



                                                /********************************/
                                                /********************************/



/** *****       ************************        **********************************        ************************       **** **/
/** *****       ************************            3.    Traitements OpenCV              ************************       **** **/


/// 3.1      Création des KeyPoints
    /// 3.1.1   Construction des images lissées et des images de différences de gaussiennes
    ///         pour chaque octave
void cvSift::BuildScaleSpace()  {

        printf("Generating scale space...\n");

        // floating point grayscale image
        IplImage* cvImageGray = cvCreateImage(cvGetSize(_cvCurrentImage), IPL_DEPTH_32F , 1);
        IplImage* cvImageTemp = cvCreateImage(cvGetSize(_cvCurrentImage), 8 , 1);

        // Create a duplicate. We don't want to mess the original
        // If the image is colour, it is converted to grayscale
        if(_cvCurrentImage->nChannels==3)   {
                cvCvtColor(_cvCurrentImage, cvImageTemp, CV_BGR2GRAY);
        }

        else   {
                cvCopy(_cvCurrentImage, cvImageTemp);
        }

        // Finally, generate the floating point image... convert 0..255 range into 0..1
        for(int x=0;x<cvImageTemp->width;x++) {

                for(int y=0;y<cvImageTemp->height;y++)    {
                        cvSetReal2D(cvImageGray, y, x, cvGetReal2D(cvImageTemp, y, x)/255.0);
                }
        }

        // Lowe claims blur the image with a sigma of 0.5 and double it's dimensions
        // to increase the number of stable keypoints
        cvSmooth(cvImageGray, cvImageGray, CV_GAUSSIAN, 0, 0, _sigmaAntiAliasing);

        // Create an image double the dimensions, resize cvImageGray and store it in _cvGaussianImages2DArray[0][0]
        _cvGaussianImages2DArray[0][0] = cvCreateImage(cvSize(cvImageGray->width*2, cvImageGray->height*2), IPL_DEPTH_32F , 1);
        cvPyrUp(cvImageGray, _cvGaussianImages2DArray[0][0]);

        // Preblur this base image
        cvSmooth(_cvGaussianImages2DArray[0][0], _cvGaussianImages2DArray[0][0], CV_GAUSSIAN, 0, 0, SIGMA_PREBLUR);

        // SaveFloatingPointImage("C:\\SIFT Test\\Gaussian\\g_octave_0_scale_0.jpg", _cvGaussianImages2DArray[0][0]);

        double initSigma = sqrt(2.0f);

        // Keep a track of the sigmas
        _sigma2DArray[0][0] = initSigma * 0.5;

        // Now for the actual image generation
        for(int  i=0;i<_numOctaves;i++)   {

                // Reset sigma for each octave
                double sigma = initSigma;
                CvSize currentSize = cvGetSize(_cvGaussianImages2DArray[i][0]);

                for(int  j=1;j<_numIntervals+3;j++)  {

                    // Allocate memory
                    _cvGaussianImages2DArray[i][j] = cvCreateImage(currentSize, 32, 1);

                    // Calculate a sigma to blur the current image to get the next one
                    double sigma_f = sqrt(pow(2.0,2.0/_numIntervals)-1) * sigma;
                    sigma = pow(2.0,1.0/_numIntervals) * sigma;

                    // Store sigma values (to be used later on)
                    _sigma2DArray[i][j] = sigma * 0.5 * pow(2.0f, (float)i);

                    // Apply gaussian smoothing)
                    cvSmooth(_cvGaussianImages2DArray[i][j-1], _cvGaussianImages2DArray[i][j], CV_GAUSSIAN, 0, 0, sigma_f);

                    // Calculate the DoG image
                    _cvDOGImages2DArray[i][j-1] = cvCreateImage(currentSize, 32, 1);
                    cvSub(_cvGaussianImages2DArray[i][j-1], _cvGaussianImages2DArray[i][j], _cvDOGImages2DArray[i][j-1]);

                    // Save the images generated for fun :)
//                    char* filename = new char[200];
//                    sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/g_octave_%d_scale_%d.jpg", i, j);
//                    SaveFloatingPointImage(filename, _cvGaussianImages2DArray[i][j]);
//
//                    sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/dog_octave_%d_scale_%d.jpg", i, j-1);
//                    SaveFloatingPointImage(filename, _cvDOGImages2DArray[i][j-1]);
                }

                // If we're not at the last octave
                if(i<_numOctaves-1)
                {
                    // Reduce size to half
                    currentSize.width/=2;
                    currentSize.height/=2;

                    // Allocate memory and resample the image
                    _cvGaussianImages2DArray[i+1][0] = cvCreateImage(currentSize, 32, 1);
                    cvPyrDown(_cvGaussianImages2DArray[i][0], _cvGaussianImages2DArray[i+1][0]);
                    _sigma2DArray[i+1][0] = _sigma2DArray[i][_numIntervals];

                    // Store the image
                  //  char* filename = new char[200];
                  //  sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/g_octave_%d_scale_0.jpg", i+1);
                  //  SaveFloatingPointImage(filename, _cvGaussianImages2DArray[i+1][0]);
                }
        }
}

    /// 3.1.2   Calcul des extrema des Différences de Gaussiennes (KeyPoints)
void cvSift::processExtremasDetection()  {
    // Looping variables
     int i, j, xi, yi;

    // Some variables we'll use later on
    double curvature_ratio, curvature_threshold;
    IplImage *middle, *up, *down;
    int scale;
    double dxx, dyy, dxy, trH, detH;

    int num=0;				// Number of keypoins detected
    int numRemoved=0;		// The number of key points rejected because they failed a test

    curvature_threshold = (CURVATURE_THRESHOLD+1)*(CURVATURE_THRESHOLD+1)/CURVATURE_THRESHOLD;

    // Detect extrema in the DoG images
    for(i=0;i<_numOctaves;i++)
    {
            scale = (int)pow(2.0, (double)i);

    for(j=1;j<_numIntervals+1;j++)
    {


        // Allocate memory and set all points to zero ("not key point")
        _cvBinarieImages2DArray[i][j-1] = cvCreateImage(cvGetSize(_cvDOGImages2DArray[i][0]), 8, 1);
        cvZero(_cvBinarieImages2DArray[i][j-1]);

        // Images just above and below, in the current octave
        middle = _cvDOGImages2DArray[i][j];
        up = _cvDOGImages2DArray[i][j+1];
        down = _cvDOGImages2DArray[i][j-1];

        for(xi=1;xi<_cvDOGImages2DArray[i][j]->width-1;xi++)
        {
            for(yi=1;yi<_cvDOGImages2DArray[i][j]->height-1;yi++)
            {
                // true if a keypoint is a maxima/minima
                // but needs to be tested for contrast/edge thingy
                bool justSet = false;

                double currentPixel = cvGetReal2D(middle, yi, xi);

                // Check for a maximum
            if (currentPixel > cvGetReal2D(middle, yi-1, xi  )	&&
            currentPixel > cvGetReal2D(middle, yi+1, xi  )  &&
            currentPixel > cvGetReal2D(middle, yi  , xi-1)  &&
            currentPixel > cvGetReal2D(middle, yi  , xi+1)  &&
            currentPixel > cvGetReal2D(middle, yi-1, xi-1)	&&
            currentPixel > cvGetReal2D(middle, yi-1, xi+1)	&&
            currentPixel > cvGetReal2D(middle, yi+1, xi+1)	&&
            currentPixel > cvGetReal2D(middle, yi+1, xi-1)	&&
            currentPixel > cvGetReal2D(up, yi  , xi  )      &&
            currentPixel > cvGetReal2D(up, yi-1, xi  )      &&
            currentPixel > cvGetReal2D(up, yi+1, xi  )      &&
            currentPixel > cvGetReal2D(up, yi  , xi-1)      &&
            currentPixel > cvGetReal2D(up, yi  , xi+1)      &&
            currentPixel > cvGetReal2D(up, yi-1, xi-1)		&&
            currentPixel > cvGetReal2D(up, yi-1, xi+1)		&&
            currentPixel > cvGetReal2D(up, yi+1, xi+1)		&&
            currentPixel > cvGetReal2D(up, yi+1, xi-1)		&&
            currentPixel > cvGetReal2D(down, yi  , xi  )    &&
            currentPixel > cvGetReal2D(down, yi-1, xi  )    &&
            currentPixel > cvGetReal2D(down, yi+1, xi  )    &&
            currentPixel > cvGetReal2D(down, yi  , xi-1)    &&
            currentPixel > cvGetReal2D(down, yi  , xi+1)    &&
            currentPixel > cvGetReal2D(down, yi-1, xi-1)	&&
            currentPixel > cvGetReal2D(down, yi-1, xi+1)	&&
            currentPixel > cvGetReal2D(down, yi+1, xi+1)	&&
            currentPixel > cvGetReal2D(down, yi+1, xi-1)   )
            {
                    cvSetReal2D(_cvBinarieImages2DArray[i][j-1], yi, xi, 255);
                    num++;
                    justSet = true;
            }
            // The contrast check
            if(justSet && fabs(cvGetReal2D(middle, yi, xi)) < _contrastThreshold)
            {
                    cvSetReal2D(_cvBinarieImages2DArray[i][j-1], yi, xi, 0);
                    num--;
                    numRemoved++;

                    justSet=false;
            }

            // The edge check
            if(justSet)
            {
                    dxx = (cvGetReal2D(middle, yi-1, xi) +
                              cvGetReal2D(middle, yi+1, xi) -
                              2.0*cvGetReal2D(middle, yi, xi));

                    dyy = (cvGetReal2D(middle, yi, xi-1) +
                              cvGetReal2D(middle, yi, xi+1) -
                              2.0*cvGetReal2D(middle, yi, xi));

                    dxy = (cvGetReal2D(middle, yi-1, xi-1) +
                              cvGetReal2D(middle, yi+1, xi+1) -
                              cvGetReal2D(middle, yi+1, xi-1) -
                              cvGetReal2D(middle, yi-1, xi+1)) / 4.0;

                    trH = dxx + dyy;
                    detH = dxx*dyy - dxy*dxy;

                    curvature_ratio = trH*trH/detH;
                    //printf("Threshold: %f - Ratio: %f\n", curvature_threshold, curvature_ratio);
                    if(detH<0 || curvature_ratio>curvature_threshold)
                    {
                        cvSetReal2D(_cvBinarieImages2DArray[i][j-1], yi, xi, 0);
                        num--;
                        numRemoved++;

                        justSet=false;
                    }
             }
        }
    }

    // Save the image
    char* filename = new char[200];
    sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/extrema_oct_%d_scale_%d.jpg", i, j-1);
    cvSaveImage(filename, _cvBinarieImages2DArray[i][j-1]);
            }
    }


    _numKeypoints = num;
    printf("Found %d keypoints\n", num);
    printf("Rejected %d keypoints\n", numRemoved);
}

    /// 3.1.3   Calcul des orientations des KeyPoints
void cvSift::processOrientation() {
    printf("Assigning orientations...\n");
    int i, j, k, xi, yi;
    int kk, tt;

    // These images hold the magnitude and direction of gradient
    // for all blurred out images
    IplImage*** magnitude = new IplImage**[_numOctaves];
    IplImage*** orientation = new IplImage**[_numOctaves];

    // Allocate some memory
    for(i=0;i<_numOctaves;i++)
    {
            magnitude[i] = new IplImage*[_numIntervals];
            orientation[i] = new IplImage*[_numIntervals];
    }

    // These two loops are to calculate the magnitude and orientation of gradients
    // through all octaces once and for all. We don't run around calculating things
    // again and again that way.

    // Iterate through all octaves
    for(i=0;i<_numOctaves;i++)
    {
        // Iterate through all scales
        for(j=1;j<_numIntervals+1;j++)
        {
            magnitude[i][j-1] = cvCreateImage(cvGetSize(_cvGaussianImages2DArray[i][j]), 32, 1);
            orientation[i][j-1] = cvCreateImage(cvGetSize(_cvGaussianImages2DArray[i][j]), 32, 1);

            cvZero(magnitude[i][j-1]);
            cvZero(orientation[i][j-1]);

            // Iterate over the gaussian image with the current octave and interval
            for(xi=1;xi<_cvGaussianImages2DArray[i][j]->width-1;xi++)
            {
                for(yi=1;yi<_cvGaussianImages2DArray[i][j]->height-1;yi++)
                {
                        // Calculate gradient
                        double dx = cvGetReal2D(_cvGaussianImages2DArray[i][j], yi, xi+1) - cvGetReal2D(_cvGaussianImages2DArray[i][j], yi, xi-1);
                        double dy = cvGetReal2D(_cvGaussianImages2DArray[i][j], yi+1, xi) - cvGetReal2D(_cvGaussianImages2DArray[i][j], yi-1, xi);

                        // Store magnitude
                        cvSetReal2D(magnitude[i][j-1], yi, xi, sqrt(dx*dx + dy*dy));

                        // Store orientation as radians
                        double ori=atan(dy/dx);
                        cvSet2D(orientation[i][j-1], yi, xi, cvScalar(ori));
                }
            }

            // Save these images for fun
            char* filename = new char[200];
            sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/mag_oct_%d_scl_%d.jpg", i, j-1);
            cvSaveImage(filename, magnitude[i][j-1]);

            sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/ori_oct_%d_scl_%d.jpg", i, j-1);
            cvSaveImage(filename, orientation[i][j-1]);
        }
    }

    // The histogram with 8 bins
    double* hist_orient = new double[NUM_BINS];

    // Go through all octaves
    for(i=0;i<_numOctaves;i++)
    {
        // Store current scale, width and height
         int scale = ( int)pow(2.0, (double)i);
         int width = _cvGaussianImages2DArray[i][0]->width;
        int height= _cvGaussianImages2DArray[i][0]->height;

        // Go through all intervals in the current scale
        for(j=1;j<_numIntervals+1;j++)
        {
        double abs_sigma = _sigma2DArray[i][j];

        // This is used for magnitudes
        IplImage* imgWeight = cvCreateImage(cvSize(width, height), 32, 1);
        cvSmooth(magnitude[i][j-1], imgWeight, CV_GAUSSIAN, 0, 0, 1.5*abs_sigma);

        // Get the kernel size for the Guassian blur
        int hfsz = getKernelSize(1.5*abs_sigma, 0.1)/2;

        // Temporarily used to generate a mask of region used to calculate
        // the orientations
        IplImage* imgMask = cvCreateImage(cvSize(width, height), 8, 1);
        cvZero(imgMask);

        // Iterate through all points at this octave and interval
        for(xi=0;xi<width;xi++)
        {
            for(yi=0;yi<height;yi++)
            {
                // We're at a keypoint
                if(cvGetReal2D(_cvBinarieImages2DArray[i][j-1], yi, xi)!=0)
                {
                    // Reset the histogram thingy
                    for(k=0;k<NUM_BINS;k++)
                            hist_orient[k]=0.0;

                    // Go through all pixels in the window around the extrema
                    for(kk=-hfsz;kk<=hfsz;kk++)
                    {
                        for(tt=-hfsz;tt<=hfsz;tt++)
                        {
                            // Ensure we're within the image
                            if(xi+kk<0 || xi+kk>=width || yi+tt<0 || yi+tt>=height)
                                    continue;

                            double sampleOrient = cvGetReal2D(orientation[i][j-1], yi+tt, xi+kk);

                            if(sampleOrient <=-_PI || sampleOrient>_PI)
                                    printf("Bad Orientation: %f\n", sampleOrient);

                            sampleOrient+=_PI;

                            // Convert to degrees
                            int sampleOrientDegrees = sampleOrient * 180 / _PI;
                            hist_orient[(int)sampleOrientDegrees / (360/NUM_BINS)] += cvGetReal2D(imgWeight, yi+tt, xi+kk);
                            cvSetReal2D(imgMask, yi+tt, xi+kk, 255);
                        }
                    }

                    // We've computed the histogram. Now check for the maximum
                    double max_peak = hist_orient[0];
                    int max_peak_index = 0;
                    for(k=1;k<NUM_BINS;k++)
                    {
                        if(hist_orient[k]>max_peak)
                        {
                                max_peak=hist_orient[k];
                                max_peak_index = k;
                        }
                    }

                    // List of magnitudes and orientations at the current extrema
                    vector<double> orien;
                    vector<double> mag;
                    for(k=0;k<NUM_BINS;k++)
                    {
                        // Do we have a good peak?
                        if(hist_orient[k]> 0.8*max_peak)
                        {
                                // Three points. (x2,y2) is the peak and (x1,y1)
                                // and (x3,y3) are the neigbours to the left and right.
                                // If the peak occurs at the extreme left, the "left
                                // neighbour" is equal to the right most. Similarly for
                                // the other case (peak is rightmost)
                                double x1 = k-1;
                                double y1;
                                double x2 = k;
                                double y2 = hist_orient[k];
                                double x3 = k+1;
                                double y3;

                                if(k==0)
                                {
                                        y1 = hist_orient[NUM_BINS-1];
                                        y3 = hist_orient[1];
                                }
                                else if(k==NUM_BINS-1)
                                {
                                        y1 = hist_orient[NUM_BINS-1];
                                        y3 = hist_orient[0];
                                }
                                else
                                {
                                        y1 = hist_orient[k-1];
                                        y3 = hist_orient[k+1];
                                }



                            double *b = new double[3];
                            CvMat *X = cvCreateMat(3, 3, CV_32FC1);
                            CvMat *matInv = cvCreateMat(3, 3, CV_32FC1);

                            cvSetReal2D(X, 0, 0, x1*x1);
                            cvSetReal2D(X, 1, 0, x1);
                            cvSetReal2D(X, 2, 0, 1);

                            cvSetReal2D(X, 0, 1, x2*x2);
                            cvSetReal2D(X, 1, 1, x2);
                            cvSetReal2D(X, 2, 1, 1);

                            cvSetReal2D(X, 0, 2, x3*x3);
                            cvSetReal2D(X, 1, 2, x3);
                            cvSetReal2D(X, 2, 2, 1);

                            // Invert the matrix
                            cvInv(X, matInv);

                            b[0] = cvGetReal2D(matInv, 0, 0)*y1 + cvGetReal2D(matInv, 1, 0)*y2 + cvGetReal2D(matInv, 2, 0)*y3;
                            b[1] = cvGetReal2D(matInv, 0, 1)*y1 + cvGetReal2D(matInv, 1, 1)*y2 + cvGetReal2D(matInv, 2, 1)*y3;
                            b[2] = cvGetReal2D(matInv, 0, 2)*y1 + cvGetReal2D(matInv, 1, 2)*y2 + cvGetReal2D(matInv, 2, 2)*y3;

                            double x0 = -b[1]/(2*b[0]);

                            // Anomalous situation
                            if(fabs(x0)>2*NUM_BINS)
                                    x0=x2;

                            while(x0<0)
                                    x0 += NUM_BINS;
                            while(x0>= NUM_BINS)
                                    x0-= NUM_BINS;

                            // Normalize it
                            double x0_n = x0*(2*_PI/NUM_BINS);

                            assert(x0_n>=0 && x0_n<2*_PI);
                            x0_n -= _PI;
                            assert(x0_n>=-_PI && x0_n<_PI);

                            orien.push_back(x0_n);
                            mag.push_back(hist_orient[k]);

                            // cout << endl << endl << "  processOrientation  " << endl << endl ;
                            // cout << "  x0_n  " <<  x0_n << endl ;
                            // cout << "  hist_orient[k]  " <<  hist_orient[k] << endl ;


                    }
                }

                    // cout << "  orien  size " <<  orien.size() << endl ;
                    // cout << "  mag size  " <<  mag.size()  << endl << endl;

                // Save this keypoint into the list
                    _keyPoints.push_back(Keypoint(xi*scale/2, yi*scale/2, mag, orien, i*_numIntervals+j-1));
                }
            }
        }

        // Save the regions!
        char* filename = new char[200];
        sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/ori_region_oct_%d_scl_%d.jpg", i, j-1);
        cvSaveImage(filename, imgMask);
        cvReleaseImage(&imgMask);
        }
    }

    // Finally, we're done with all the magnitude and orientation images.
    // Erase them from RAM
    assert((int)_keyPoints.size() == _numKeypoints);
    for(i=0;i<_numOctaves;i++)
    {
            for(j=1;j<_numIntervals+1;j++)
            {
                    cvReleaseImage(&magnitude[i][j-1]);
                    cvReleaseImage(&orientation[i][j-1]);
            }

            delete [] magnitude[i];
            delete [] orientation[i];
    }

    delete [] magnitude;
    delete [] orientation;
}

    /// 3.1.4   Retourne la taille d'un noyeau gaussien de variance sigma et cut_off
 int cvSift::getKernelSize(double sigma, double cut_off)
{
    int i;
    for (i=0;i<MAX_KERNEL_SIZE;i++)
        if (exp(-((double)(i*i))/(2.0*sigma*sigma))<cut_off)
            break;
    int size = 2*i-1;
    return size;
}



/// 3.2     Création des Descripteurs
    /// 3.2.1     Création d'un descriteur pour chaque KeyPoint
 void cvSift::extractKeypointDescriptors()
 {
         printf("Extract keypoint descriptors...\n");

         // For loops
          int i, j;

         // Interpolated thingy. We're dealing with "inbetween" gradient
         // magnitudes and orientations
         IplImage*** cvImageInterpolatedMagnitude = new IplImage**[_numOctaves];
         IplImage*** cvImageInterpolatedOrientation = new IplImage**[_numOctaves];
         for(i=0;i<_numOctaves;i++)
         {
                 cvImageInterpolatedMagnitude[i] = new IplImage*[_numIntervals];
                 cvImageInterpolatedOrientation[i] = new IplImage*[_numIntervals];
         }

         // These two loops calculate the interpolated thingy for all octaves
         // and subimages
         for(i=0;i<_numOctaves;i++)
         {
                 for(j=1;j<_numIntervals+1;j++)
                 {
                          int width = _cvGaussianImages2DArray[i][j]->width;
                          int height =_cvGaussianImages2DArray[i][j]->height;

                         // Create an image and zero it out.
                         IplImage* cvImageTemp = cvCreateImage(cvSize(width*2, height*2), 32, 1);
                         cvZero(cvImageTemp);

                         // Scale it up. This will give us "access" to in betweens
                         cvPyrUp(_cvGaussianImages2DArray[i][j], cvImageTemp);

                         // Allocate memory and zero them
                         cvImageInterpolatedMagnitude[i][j-1] = cvCreateImage(cvSize(width+1, height+1), 32, 1);
                         cvImageInterpolatedOrientation[i][j-1] = cvCreateImage(cvSize(width+1, height+1), 32, 1);
                         cvZero(cvImageInterpolatedMagnitude[i][j-1]);
                         cvZero(cvImageInterpolatedOrientation[i][j-1]);

                         // Do the calculations
                         for(float ii=1.5;ii<width-1.5;ii++)
                         {
                                 for(float jj=1.5;jj<height-1.5;jj++)
                                 {
                                         // "inbetween" change
                                         double dx = (cvGetReal2D(_cvGaussianImages2DArray[i][j], jj, ii+1.5) + cvGetReal2D(_cvGaussianImages2DArray[i][j], jj, ii+0.5))/2 - (cvGetReal2D(_cvGaussianImages2DArray[i][j], jj, ii-1.5) + cvGetReal2D(_cvGaussianImages2DArray[i][j], jj, ii-0.5))/2;
                                         double dy = (cvGetReal2D(_cvGaussianImages2DArray[i][j], jj+1.5, ii) + cvGetReal2D(_cvGaussianImages2DArray[i][j], jj+0.5, ii))/2 - (cvGetReal2D(_cvGaussianImages2DArray[i][j], jj-1.5, ii) + cvGetReal2D(_cvGaussianImages2DArray[i][j], jj-0.5, ii))/2;

                                          int iii = ii+1;
                                          int jjj = jj+1;
                                         assert(iii<=width && jjj<=height);

                                         // Set the magnitude and orientation
                                         cvSetReal2D(cvImageInterpolatedMagnitude[i][j-1], jjj, iii, sqrt(dx*dx + dy*dy));
                                         cvSetReal2D(cvImageInterpolatedOrientation[i][j-1], jjj, iii, (atan2(dy,dx)==_PI)? -_PI:atan2(dy,dx) );
                                 }
                         }

                             // Pad the edges with zeros
                             for(int iii=0;iii<width+1;iii++)
                             {
                                     cvSetReal2D(cvImageInterpolatedMagnitude[i][j-1], 0, iii, 0);
                                     cvSetReal2D(cvImageInterpolatedMagnitude[i][j-1], height, iii, 0);
                                     cvSetReal2D(cvImageInterpolatedOrientation[i][j-1], 0, iii, 0);
                                     cvSetReal2D(cvImageInterpolatedOrientation[i][j-1], height, iii, 0);
                             }

                             for(int jjj=0;jjj<height+1;jjj++)
                             {
                                     cvSetReal2D(cvImageInterpolatedMagnitude[i][j-1], jjj, 0, 0);
                                     cvSetReal2D(cvImageInterpolatedMagnitude[i][j-1], jjj, width, 0);
                                     cvSetReal2D(cvImageInterpolatedOrientation[i][j-1], jjj, 0, 0);
                                     cvSetReal2D(cvImageInterpolatedOrientation[i][j-1], jjj, width, 0);
                             }

                             // Now we have the imgInterpolated* ready. Store and get started
                             char* filename = new char[200];
                             sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/intmag_oct_%d_scl_%d.jpg", i, j-1);
                             cvSaveImage(filename, cvImageInterpolatedMagnitude[i][j-1]);


                             sprintf(filename, "/Users/juliencouet/Desktop/Cour_Luc/Programmes/PartieB_3DVision/Mosaic/test/intori_oct_%d_scl_%d.jpg", i, j-1);
                             cvSaveImage(filename, cvImageInterpolatedOrientation[i][j-1]);

                             cvReleaseImage(&cvImageTemp);

                     }
             }

             // Build an Interpolated Gaussian Table of size FEATURE_WINDOW_SIZE
             // Lowe suggests sigma should be half the window size
             // This is used to construct the "circular gaussian window" to weight
             // magnitudes
             CvMat *G = buildInterpolatedGaussianTable(FEATURE_WINDOW_SIZE, 0.5*FEATURE_WINDOW_SIZE);

             vector<double> hist(DESC_NUM_BINS);

             // Loop over all keypoints
             for(int ikp = 0;ikp<_numKeypoints;ikp++)
             {
                      int scale = _keyPoints[ikp].scale;
                     float kpxi = _keyPoints[ikp].xi;
                     float kpyi = _keyPoints[ikp].yi;




                     float descxi = kpxi;
                     float descyi = kpyi;

                      int ii = (int)(kpxi*2) / (int)(pow(2.0, (double)scale/_numIntervals));
                      int jj = (int)(kpyi*2) / (int)(pow(2.0, (double)scale/_numIntervals));

                      int width = _cvGaussianImages2DArray[scale/_numIntervals][0]->width;
                      int height = _cvGaussianImages2DArray[scale/_numIntervals][0]->height;

                     vector<double> orien = _keyPoints[ikp].orien;
                     vector<double> mag = _keyPoints[ikp].mag;

                     // cout << endl << endl << "main_mag : " << orien.size() << endl << endl;
                     // cout << "main_orien : " << mag.size() << endl << endl;


                     // Find the orientation and magnitude that have the maximum impact
                     // on the feature
                     double main_mag = mag[0];
                     double main_orien = orien[0];




                     for(int orient_count=1;orient_count<mag.size();orient_count++)
                     {
                             if(mag[orient_count]>main_mag)
                             {
                                     main_orien = orien[orient_count];
                                     main_mag = mag[orient_count];
                             }
                     }

                      int hfsz = FEATURE_WINDOW_SIZE/2;
                     CvMat *weight = cvCreateMat(FEATURE_WINDOW_SIZE, FEATURE_WINDOW_SIZE, CV_32FC1);
                     vector<double> fv(FVSIZE);

                     for(i=0;i<FEATURE_WINDOW_SIZE;i++)
                     {
                             for(j=0;j<FEATURE_WINDOW_SIZE;j++)
                             {
                                     if(ii+i+1<hfsz || ii+i+1>width+hfsz || jj+j+1<hfsz || jj+j+1>height+hfsz)
                         cvSetReal2D(weight, j, i, 0);
                                     else
                                             cvSetReal2D(weight, j, i, cvGetReal2D(G, j, i)*cvGetReal2D(cvImageInterpolatedMagnitude[scale/_numIntervals][scale%_numIntervals], jj+j+1-hfsz, ii+i+1-hfsz));
                             }
                     }

                     // Now that we've weighted the required magnitudes, we proceed to generating
                     // the feature vector

                     // The next two two loops are for splitting the 16x16 window
                     // into sixteen 4x4 blocks
                     for(i=0;i<FEATURE_WINDOW_SIZE/4;i++)			// 4x4 thingy
                     {
                             for(j=0;j<FEATURE_WINDOW_SIZE/4;j++)
                             {
                                     // Clear the histograms
                                     for( int t=0;t<DESC_NUM_BINS;t++)
                                             hist[t]=0.0;

                                     // Calculate the coordinates of the 4x4 block
                                     int starti = (int)ii - (int)hfsz + 1 + (int)(hfsz/2*i);
                                     int startj = (int)jj - (int)hfsz + 1 + (int)(hfsz/2*j);
                                     int limiti = (int)ii + (int)(hfsz/2)*((int)(i)-1);
                                     int limitj = (int)jj + (int)(hfsz/2)*((int)(j)-1);

                                     // Go though this 4x4 block and do the thingy :D
                                     for(int k=starti;k<=limiti;k++)
                                     {
                                             for(int t=startj;t<=limitj;t++)
                                             {
                                                     if(k<0 || k>width || t<0 || t>height)
                                                             continue;

                                                     // This is where rotation invariance is done
                                                     double sample_orien = cvGetReal2D(cvImageInterpolatedOrientation[scale/_numIntervals][scale%_numIntervals], t, k);
                                                     sample_orien -= main_orien;

                                                     while(sample_orien<0)
                                                             sample_orien+=2*_PI;

                                                     while(sample_orien>2*_PI)
                                                             sample_orien-=2*_PI;

                                                     // This should never happen
                                                     if(!(sample_orien>=0 && sample_orien<2*_PI))
                                                             printf("BAD: %f\n", sample_orien);
                                                     assert(sample_orien>=0 && sample_orien<2*_PI);

                                                     int sample_orien_d = sample_orien*180/_PI;
                                                     assert(sample_orien_d<360);

                                                     int bin = sample_orien_d/(360/DESC_NUM_BINS);					// The bin
                                                     double bin_f = (double)sample_orien_d/(double)(360/DESC_NUM_BINS);		// The actual entry

                                                     assert(bin<DESC_NUM_BINS);
                                                     assert(k+hfsz-1-ii<FEATURE_WINDOW_SIZE && t+hfsz-1-jj<FEATURE_WINDOW_SIZE);

                                                     // Add to the bin
                                                     hist[bin]+=(1-fabs(bin_f-(bin+0.5))) * cvGetReal2D(weight, t+hfsz-1-jj, k+hfsz-1-ii);
                                             }
                                     }

                                     // Keep adding these numbers to the feature vector
                                     for(int t=0;t<DESC_NUM_BINS;t++)
                                     {
                                             fv[(i*FEATURE_WINDOW_SIZE/4+j)*DESC_NUM_BINS+t] = hist[t];
                                     }
                             }
                     }

                     // Now, normalize the feature vector to ensure illumination independence
                     double norm=0;
                     for( int t=0;t<FVSIZE;t++)
                             norm+=pow(fv[t], 2.0);
                     norm = sqrt(norm);

                     for( int t=0;t<FVSIZE;t++)
                             fv[t]/=norm;

                     // Now, threshold the vector
                     for(int t=0;t<FVSIZE;t++)
                             if(fv[t]>FV_THRESHOLD)
                                     fv[t] = FV_THRESHOLD;

                     // Normalize yet again
                     norm=0;
                     for(int t=0;t<FVSIZE;t++)
                             norm+=pow(fv[t], 2.0);
                     norm = sqrt(norm);

                     for(int t=0;t<FVSIZE;t++)
                             fv[t]/=norm;

                     // We're done with this descriptor. Store it into a list
                     _keyDescs.push_back(Descriptor(descxi, descyi, fv));
             }

             assert(_keyDescs.size()==_numKeypoints);

             // Get rid of memory we don't need anylonger
             for(i=0;i<_numOctaves;i++)
             {
                     for(j=1;j<_numIntervals+1;j++)
                     {
                             cvReleaseImage(&cvImageInterpolatedMagnitude[i][j-1]);
                             cvReleaseImage(&cvImageInterpolatedOrientation[i][j-1]);
                     }

                     delete [] cvImageInterpolatedMagnitude[i];
                     delete [] cvImageInterpolatedOrientation[i];
             }

             delete [] cvImageInterpolatedMagnitude;
             delete [] cvImageInterpolatedOrientation;
     }

 CvMat* cvSift::buildInterpolatedGaussianTable( int size, double sigma)
 {
         int i, j;
         double half_kernel_size = size/2 - 0.5;

         double sog=0;
         CvMat* ret = cvCreateMat(size, size, CV_32FC1);

         assert(size%2==0);

         double temp=0;
         for(i=0;i<size;i++)
         {
                 for(j=0;j<size;j++)
                 {
                         temp = gaussian2D(i-half_kernel_size, j-half_kernel_size, sigma);
                         cvSetReal2D(ret, j, i, temp);
                         sog+=temp;
                 }
         }

         for(i=0;i<size;i++)
         {
                 for(j=0;j<size;j++)
                 {
                         cvSetReal2D(ret, j, i, 1.0/sog * cvGetReal2D(ret, j, i));
                 }
         }

         return ret;
 }

///     3.2.2   Calcul de la valeur de la gausienne 2D de variance sigma au point (x, y)
 double cvSift::gaussian2D(double x, double y, double sigma)
 {
         double ret = 1.0/(2*_PI*sigma*sigma) * exp(-(x*x+y*y)/(2.0*sigma*sigma));


         return ret;
 }


 /** *****       ************************              fin Traitements OpenCV              ************************       **** **/
 /** *****       ************************        **********************************        ************************       **** **/

