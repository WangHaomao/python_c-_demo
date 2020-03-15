/*
cpython demo

compile
g++ -fPIC -shared -I/usr/include/python3.6 test1.cpp -o test1.so

python scripts
import numpy as np
import test1
img = np.ones((5,5,3),dtype=np.uint8) * 128
test1.test_PyNp(img)

*/
//#include <D:/Softwares/Anaconda3/include/Python.h>
//#include <D:/Softwares/Anaconda3/Lib/site-packages/numpy/core/include/numpy/ndarrayobject.h>

#include <Python.h>
#include <numpy/ndarrayobject.h>
#include <iostream>

int Add(int x, int y)
{
    return x + y;
}

PyObject* WrappAdd(PyObject* self, PyObject* args)
{
    int x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y))
    {
        return nullptr;
    }
    return Py_BuildValue("i", Add(x, y));
}

PyObject* justPrint(PyObject* self, PyObject* args){
    int x;
    if (!PyArg_ParseTuple(args, "i", &x))
    {
        return nullptr;
    }
    return Py_BuildValue("i", x + 2);
}

PyObject* testPyNp(PyObject* self, PyObject *args){

    PyArrayObject *image;

    if (!PyArg_ParseTuple(args, "O", &image))
    {
        std::cout << "Some Thing Wrong @@@@@" << std::endl;
        return nullptr;
    }
    std::cout << "success" << std::endl;
    int width = image -> dimensions[0];
    int height = image -> dimensions[1];
    std::cout << width << height << std::endl;
//    unsigned char cImage[width][height][3];
//    return Py_BuildValue("i", image->data[0][0]);
    int count = 0;
    for(int i=0;i<width;i++){
        for(int j = 0;j<height;j++){
            for(int z = 0;z < 3;z++){
                std::cout << *((uint8_t *)PyArray_GETPTR3(image, i, j, z)) << std::endl;
                count ++;
            }
        }
    }
    std::cout << count << std::endl;
    return Py_BuildValue("i", PyArray_NDIM(image));
}


static PyMethodDef test_methods[] = {
        {"Add", WrappAdd, METH_VARARGS, "something"},
        {"test_PyNp",testPyNp,METH_VARARGS, "something"},
        {"just_print", justPrint,METH_VARARGS, "something"},
        {nullptr, nullptr, 0, nullptr}
};

/* // Python 2.x
extern "C"
void inittest1()
{
    Py_InitModule("test1", test_methods);
}
*/

/* Python 3.x
 * https://docs.python.org/zh-cn/3/extending/extending.html#the-module-s-method-table-and-initialization-function
 * */

static struct PyModuleDef TestModels =
        {
                PyModuleDef_HEAD_INIT,
                "test1",     /* name of module */
                "",          /* module documentation, may be NULL */
                -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
                test_methods
        };

PyMODINIT_FUNC PyInit_test1(void)
{
    import_array();
    return PyModule_Create(&TestModels);
}

