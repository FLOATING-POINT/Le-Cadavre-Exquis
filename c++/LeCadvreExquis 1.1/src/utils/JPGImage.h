#ifndef JPGIMAGE_H_INCLUDED
#define JPGIMAGE_H_INCLUDED

class JPGImage : public ofImage
{
    public :

        void saveAsJpg(string fileName, int quality)
        {
           fileName = ofToDataPath(fileName);

           if(quality > 100) quality = 100;
           else if(quality < 1)quality = 1;

           if (myPixels.bAllocated){

              FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
              fif = FreeImage_GetFileType(fileName.c_str(), 0);

              if(fif == FIF_UNKNOWN)  fif = FreeImage_GetFIFFromFilename(fileName.c_str());

              if((fif == FIF_JPEG) && FreeImage_FIFSupportsReading(fif))
              {

                 #ifdef TARGET_LITTLE_ENDIAN
                     if (myPixels.bytesPerPixel != 1) swapRgb(myPixels);
                 #endif

                 FIBITMAP *fibmp = getBmpFromPixels(myPixels);

                 #ifdef TARGET_LITTLE_ENDIAN
                     if (myPixels.bytesPerPixel != 1) swapRgb(myPixels);
                 #endif

                 //FreeImage_FlipVertical( fibmp );
                 FreeImage_Save(fif, fibmp, fileName.c_str(), quality);
                 //FreeImage_FlipVertical( fibmp );
              }else{
                 printf("saveJpg - requires you to save with .jpg extension - image not saved\n");
                 return;
              }
           }
        }
};

#endif // JPGIMAGE_H_INCLUDED
