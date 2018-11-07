//
// Created by leo on 18-11-6.
//

#ifndef YOLOV3_YOLOV3_H
#define YOLOV3_YOLOV3_H

#include <python2.7/Python.h>
#include <numpy/arrayobject.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <stdlib.h>
#include <malloc.h>

using namespace cv;
using namespace std;

class TransMethod;

class YOLOSystem
{
    struct DetectResult
    {
        string mName;
        float mConfidence;
        vector<float> mvBoudingBox;
    };

public:
    YOLOSystem();
    ~YOLOSystem();
    void ShowImage(Mat Image);
    vector<DetectResult> Detect(Mat Image);

private:
    PyObject *mpModule;
    PyObject *mpClass;
    PyObject *mpInstance;
    PyByteArrayObject* TransMat2Array(Mat Image);
    TransMethod* mpTransMethod;

};



class TransMethod
{
public:
    typedef struct {
        int w;
        int h;
        int c;
        float *data;
    }image;

    image mat_to_image(Mat m)
    {
        IplImage ipl = m;
        image im = ipl_to_image(&ipl);
        rgbgr_image(im);
        return im;
    }

    void rgbgr_image(image im)
    {
        int i;
        for(i = 0; i < im.w*im.h; ++i){
            float swap = im.data[i];
            im.data[i] = im.data[i+im.w*im.h*2];
            im.data[i+im.w*im.h*2] = swap;
        }
    }

    image ipl_to_image(IplImage* src)
    {
        int h = src->height;
        int w = src->width;
        int c = src->nChannels;
        image im = make_image(w, h, c);
        unsigned char *data = (unsigned char *)src->imageData;
        int step = src->widthStep;
        int i, j, k;

        for(i = 0; i < h; ++i){
            for(k= 0; k < c; ++k){
                for(j = 0; j < w; ++j){
                    im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
                }
            }
        }
        return im;
    }

    image make_image(int w, int h, int c)
    {
        image out = make_empty_image(w,h,c);
        out.data = (float*)calloc(h*w*c, sizeof(float));
        return out;
    }

    image make_empty_image(int w, int h, int c)
    {
        image out;
        out.data = 0;
        out.h = h;
        out.w = w;
        out.c = c;
        return out;
    }
};


#endif //YOLOV3_YOLOV3_H
