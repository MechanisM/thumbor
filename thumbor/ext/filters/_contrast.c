#include "filter.h"

static PyObject*
_contrast_apply(PyObject *self, PyObject *args)
{
    PyObject *delta = NULL, *buffer = NULL, *image_mode = NULL;

    if (!PyArg_UnpackTuple(args, "apply", 3, 3, &image_mode, &delta, &buffer)) {
        return NULL;
    }

    char *image_mode_str = PyString_AsString(image_mode);
    Py_ssize_t size = PyString_Size(buffer);
    unsigned char *ptr = (unsigned char *) PyString_AsString(buffer);
    int delta_int = (int) PyInt_AsLong(delta);
    int num_bytes = bytes_per_pixel(image_mode_str);

    delta_int = delta_int + 100;
    delta_int = (delta_int * delta_int) / 100;

    int i = 0, r, g, b;
    size -= num_bytes;
    for (; i <= size; i += num_bytes) {
        r = ptr[i];
        g = ptr[i + 1];
        b = ptr[i + 2];

        r = ((delta_int * (r - 128)) / 100) + 128;
        g = ((delta_int * (g - 128)) / 100) + 128;
        b = ((delta_int * (b - 128)) / 100) + 128;

        ptr[i] = ADJUST_COLOR(r);
        ptr[i + 1] = ADJUST_COLOR(g);
        ptr[i + 2] = ADJUST_COLOR(b);
    }

    Py_INCREF(buffer);
    return buffer;
}

FILTER_MODULE(_contrast,
    "apply(delta, buffer) -> string\n"
    "Applies a contrast filter assuming 'delta' as an integer value between -100 and 100, "
    "and 'buffer' as a Python string. Returns the received buffer."
)
