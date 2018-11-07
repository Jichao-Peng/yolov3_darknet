//
// Created by leo on 18-11-6.
//

#include "yolov3.h"

YOLOSystem::YOLOSystem()
{
    //Python初始化
    Py_Initialize();
    _import_array();

    //链接到YOLO对应的python接口文件上去
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/leo/Desktop/yolov3_project/src/yolov3/darknet/python/')");

    //导入python脚本
    mpModule = PyImport_ImportModule("darknet");//导入Python脚本
    if(!mpModule)
    {
        cout << "Load python module failed" << endl;
    }
    else
    {
        cout << "Load python module successed!" << endl;
    }

    //导入python脚本中的类
    mpClass = PyObject_GetAttrString(mpModule, "DetectInterface");
    mpInstance = PyInstance_New(mpClass, NULL, NULL);
    if(!mpInstance)
    {
        cout << "Load python instance failed" << endl;
    }
    else
    {
        cout << "Load python instance successed!" << "\n";
    }


    mpTransMethod = new TransMethod();
}

YOLOSystem::~YOLOSystem()
{
    Py_Finalize();
}

PyByteArrayObject* YOLOSystem::TransMat2Array(Mat Image)
{
    cvtColor(Image,Image,CV_RGB2BGR);//讲RGB转化成BGR（python里面的图片格式）
    npy_intp ImageShape[1]= {Image.rows * Image.cols * 3};
    char* Data = new char[ImageShape[0]];
    memcpy(Data,Image.data,Image.total() * Image.elemSize() * sizeof(char));
    PyByteArrayObject *pArray = reinterpret_cast<PyByteArrayObject*>(PyArray_SimpleNewFromData(1, ImageShape, NPY_BYTE, reinterpret_cast<void *>(Data)));

    return pArray;
}

//这个函数其实没什么用，只是花了两天的时间写的用来传递图片的给PYTHON的函数结果用不上舍不得删[捂脸]darknet太特么坑了，为什么没有mat接口
void YOLOSystem::ShowImage(Mat Image)
{
    PyByteArrayObject *pArray = TransMat2Array(Image);//讲Mat转化成传输到python的格式
    cout<<"Finish trans"<<endl;
    //申请python入参
    PyObject* pArg = PyTuple_New(1);
    //对python入参进行赋值
    PyTuple_SetItem(pArg, 0, reinterpret_cast<PyObject*>(pArray));
    //执行函数
    PyObject *pResult = PyObject_CallMethodObjArgs(mpInstance, Py_BuildValue("s","ShowImage"), pArg, NULL);
}

vector<YOLOSystem::DetectResult> YOLOSystem::Detect(Mat mat)
{
    TransMethod::image Image = mpTransMethod->mat_to_image(mat);
    //打包成byte*
    float* pImage = Image.data;
    //申请python入参
    PyObject *pArg = PyTuple_New(1);
    //对python入参进行赋值; s代表char*格式, #代表传入指定长度
    PyTuple_SetItem(pArg, 0, reinterpret_cast<PyObject*>(pImage));
    //执行函数
    PyObject *pResult = PyObject_CallMethodObjArgs(mpInstance, Py_BuildValue("s","Detect"), pArg, NULL);

    if(pResult)
    {
        cout<<"Get result"<<endl;
        int ResultSize = (int)PyList_Size(pResult);
        cout<<ResultSize<<endl;
        PyObject* pClass = PyList_GetItem(pResult,0);
        int ClassSize = (int)PyTuple_Size(pClass);
        cout<<ClassSize<<endl;
        PyObject* pClassName = PyTuple_GetItem(pClass,0);
        String Name = PyString_AsString(pClassName);
        cout<<Name<<endl;
    }
}

int main()
{
    YOLOSystem YOLO;
    Mat mat = imread("/home/leo/Desktop/yolov3_project/src/yolov3/darknet/data/girl.jpeg");
    YOLO.Detect(mat);
}
