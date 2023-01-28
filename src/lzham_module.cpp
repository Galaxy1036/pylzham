#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "lib/lzham++.h"


lzham_static_lib lzham_lib;


PyObject *lzham_exception(const char *text) {
    PyErr_SetString(PyExc_ValueError, text);
    return (PyObject *)0;
}

static PyObject* pylzham_get_version(PyObject* self, PyObject* args)
{
    return Py_BuildValue("i", lzham_lib.lzham_get_version());
}

static PyObject* pylzham_decompress_init(PyObject* self, PyObject* args, PyObject* kw)
{
    unsigned int m_dict_size_log2 = 26;
    unsigned int m_table_update_rate = LZHAM_DEFAULT_TABLE_UPDATE_RATE;
    unsigned int m_table_max_update_interval = 0;
    unsigned int m_table_update_interval_slow_rate = 0;

    bool m_unbuffered_decompression = false;
    bool m_compute_adler32_during_decomp = true;

    static const char *kwlist[] = {"dict_size_log2",
                                   "table_update_rate",
                                   "table_max_update_interval",
                                   "table_update_interval_slow_rate",
                                   "compute_adler32_during_decomp",
                                   "unbuffered_decompression",
                                   0};

    if (!PyArg_ParseTupleAndKeywords(args, kw,
                                     "|iiiipp:lzham_decompress_init",
                                     (char **)kwlist,
                                     &m_dict_size_log2,
                                     &m_table_update_rate,
                                     &m_table_max_update_interval,
                                     &m_table_update_interval_slow_rate,
                                     &m_compute_adler32_during_decomp,
                                     &m_unbuffered_decompression)) {
        return (PyObject *)0;
    }

    if ((m_dict_size_log2 < LZHAM_MIN_DICT_SIZE_LOG2) || (m_dict_size_log2 > LZHAM_MAX_DICT_SIZE_LOG2_X64))
        return lzham_exception("Invalid dict size !");

    lzham_decompress_params params;
    memset(&params, 0, sizeof(params));

    params.m_struct_size = sizeof(lzham_decompress_params);

    params.m_dict_size_log2 = m_dict_size_log2;
    params.m_table_update_rate = m_table_update_rate;
    params.m_table_max_update_interval = m_table_max_update_interval;
    params.m_table_update_interval_slow_rate = m_table_update_interval_slow_rate;

    if (m_compute_adler32_during_decomp)
        params.m_decompress_flags |= LZHAM_DECOMP_FLAG_COMPUTE_ADLER32;
    if (m_unbuffered_decompression)
        params.m_decompress_flags |= LZHAM_DECOMP_FLAG_OUTPUT_UNBUFFERED;

    lzham_decompress_state_ptr pDecomp_state = lzham_lib.lzham_decompress_init(&params);

    if (pDecomp_state) {
        return PyLong_FromVoidPtr(pDecomp_state);
    }
    else {
        return lzham_exception("Failed initializing decompressor !");
    }
}

static PyObject* pylzham_decompress_reinit(PyObject* self, PyObject* args, PyObject* kw)
{
    PyObject *pyDecomp_state;
    lzham_decompress_state_ptr pDecomp_state;

    unsigned int m_dict_size_log2 = 26;
    unsigned int m_table_update_rate = LZHAM_DEFAULT_TABLE_UPDATE_RATE;
    unsigned int m_table_max_update_interval = 0;
    unsigned int m_table_update_interval_slow_rate = 0;

    bool m_unbuffered_decompression = false;
    bool m_compute_adler32_during_decomp = true;

    static const char *kwlist[] = {"decomp_state_ptr",
                                   "dict_size_log2",
                                   "table_update_rate",
                                   "table_max_update_interval",
                                   "table_update_interval_slow_rate",
                                   "compute_adler32_during_decomp",
                                   "unbuffered_decompression",
                                   0};

    if (!PyArg_ParseTupleAndKeywords(args, kw,
                                     "O|iiiipp:lzham_decompress_reinit",
                                     (char **)kwlist,
                                     &pyDecomp_state,
                                     &m_dict_size_log2,
                                     &m_table_update_rate,
                                     &m_table_max_update_interval,
                                     &m_table_update_interval_slow_rate,
                                     &m_compute_adler32_during_decomp,
                                     &m_unbuffered_decompression)) {
        return (PyObject *)0;
    }

    if ((m_dict_size_log2 < LZHAM_MIN_DICT_SIZE_LOG2) || (m_dict_size_log2 > LZHAM_MAX_DICT_SIZE_LOG2_X64))
        return lzham_exception("Invalid dict size !");

    lzham_decompress_params params;
    memset(&params, 0, sizeof(params));

    params.m_struct_size = sizeof(lzham_decompress_params);

    params.m_dict_size_log2 = m_dict_size_log2;
    params.m_table_update_rate = m_table_update_rate;
    params.m_table_max_update_interval = m_table_max_update_interval;
    params.m_table_update_interval_slow_rate = m_table_update_interval_slow_rate;

    if (m_compute_adler32_during_decomp)
        params.m_decompress_flags |= LZHAM_DECOMP_FLAG_COMPUTE_ADLER32;
    if (m_unbuffered_decompression)
        params.m_decompress_flags |= LZHAM_DECOMP_FLAG_OUTPUT_UNBUFFERED;

    pDecomp_state = PyLong_AsVoidPtr(pyDecomp_state);

    if (pDecomp_state)
    {
        pDecomp_state = lzham_lib.lzham_decompress_reinit(pDecomp_state, &params);

        if (pDecomp_state) {
            return PyLong_FromVoidPtr(pDecomp_state);
        }
        else {
            return lzham_exception("Failed re-initializing decompressor !");
        }
    }
    else {
        return lzham_exception("Corrupted handler !");
    }
}

static PyObject* pylzham_decompress(PyObject* self, PyObject* args, PyObject* kw)
{
    PyObject *pyDecomp_state;
    unsigned long long decompressed_size;
    lzham_decompress_state_ptr pDecomp_state;
    const unsigned char *compressed_data;

    static const char *kwlist[] = {"decomp_state_ptr", 
                                   "compressed_data",
                                   "decompressed_size",
                                   0};

    Py_ssize_t compressed_data_len = 0;
    PyObject *ret;

    if (!PyArg_ParseTupleAndKeywords(args, kw,
                                     "Os#K:lzham_decompress",
                                     (char **)kwlist,
                                     &pyDecomp_state,
                                     (char **)&compressed_data, &compressed_data_len,
                                     &decompressed_size)) {
        return (PyObject *)0;
    }

    pDecomp_state = PyLong_AsVoidPtr(pyDecomp_state);

    if (pDecomp_state){
        if (!compressed_data_len)
        {
            return PyBytes_FromStringAndSize((char *)0, 0);
        }

        ret = PyBytes_FromStringAndSize((char *)0, decompressed_size);

        if (!ret)
            return lzham_exception("Failed to allocate the output buffer !");

        lzham_decompress_status_t status;
        
        unsigned long long in_buf_ofs = 0;
        unsigned long long out_buf_ofs = 0;
        unsigned long long dst_bytes_left = decompressed_size;

        for ( ; ; )
        {
            
            size_t num_in_bytes = compressed_data_len - in_buf_ofs;
            size_t out_buf_len = decompressed_size - out_buf_ofs;

            status = lzham_lib.lzham_decompress(pDecomp_state,
                                                compressed_data + in_buf_ofs, 
                                                &num_in_bytes,
                                                (unsigned char *)PyBytes_AS_STRING(ret) + out_buf_ofs,
                                                &out_buf_len,
                                                true);

            if (num_in_bytes)
                in_buf_ofs += (unsigned int)num_in_bytes;

            if (out_buf_len){
                out_buf_ofs += (unsigned int)out_buf_len;

                if (out_buf_len > dst_bytes_left)
                    return lzham_exception("Decompressor wrote too many bytes to destination buffer");

                dst_bytes_left -= out_buf_len;
            }

            if (status >= LZHAM_DECOMP_STATUS_FIRST_SUCCESS_OR_FAILURE_CODE)
                break;
        }

        if (status != LZHAM_DECOMP_STATUS_SUCCESS){
           char buffer[100];
           sprintf(buffer, "Decompression failed with status code: %i", status);
           return lzham_exception(buffer);
        }

        return ret;
    }
    else {
        return lzham_exception("Corrupted handler !");
    }
}

static PyObject* pylzham_compress_init(PyObject* self, PyObject* args, PyObject* kw)
{
    unsigned int m_dict_size_log2 = 26;
    lzham_compress_level m_level = LZHAM_COMP_LEVEL_DEFAULT;
    unsigned int m_max_helper_threads = 0;

    unsigned int m_table_update_rate = LZHAM_DEFAULT_TABLE_UPDATE_RATE;
    unsigned int m_table_max_update_interval = 0;
    unsigned int m_table_update_interval_slow_rate = 0;

    static const char *kwlist[] = {"dict_size_log2",
                                   "level",
                                   "table_update_rate",
                                   "max_helper_threads",
                                   "table_max_update_interval",
                                   "table_update_interval_slow_rate",
                                   0};

    if (!PyArg_ParseTupleAndKeywords(args, kw,
                                     "|iiiiii:lzham_compress_init",
                                     (char **)kwlist,
                                     &m_dict_size_log2,
                                     &m_level,
                                     &m_table_update_rate,
                                     &m_max_helper_threads,
                                     &m_table_max_update_interval,
                                     &m_table_update_interval_slow_rate)) {
        return (PyObject *)0;
    }


    if ((m_dict_size_log2 < LZHAM_MIN_DICT_SIZE_LOG2) || (m_dict_size_log2 > LZHAM_MAX_DICT_SIZE_LOG2_X64))
        return lzham_exception("Invalid dict size !");

    if (m_max_helper_threads > LZHAM_MAX_HELPER_THREADS)
        return lzham_exception("Invalid number of helper threads");

    if ((m_level != LZHAM_COMP_LEVEL_FASTEST)
         && (m_level != LZHAM_COMP_LEVEL_FASTER)
         && (m_level != LZHAM_COMP_LEVEL_DEFAULT)
         && (m_level != LZHAM_COMP_LEVEL_BETTER)
         && (m_level != LZHAM_COMP_LEVEL_UBER)
         && (m_level != LZHAM_TOTAL_COMP_LEVELS)
         && (m_level != LZHAM_COMP_LEVEL_FORCE_DWORD)){

        return lzham_exception("Invalid compression level !");
    }

    lzham_compress_params params;

    memset(&params, 0, sizeof(params));

    params.m_struct_size = sizeof(lzham_compress_params);

    params.m_dict_size_log2 = m_dict_size_log2;
    params.m_level = m_level;
    params.m_table_update_rate = m_table_update_rate;
    params.m_max_helper_threads = m_max_helper_threads;
    params.m_table_max_update_interval = m_table_max_update_interval;
    params.m_table_update_interval_slow_rate = m_table_update_interval_slow_rate;

    lzham_compress_state_ptr pComp_state = lzham_lib.lzham_compress_init(&params);

    if (pComp_state) {
        return PyLong_FromVoidPtr(pComp_state);
    }
    else {
        return lzham_exception("Failed initializing compressor !");
    }
}

static PyObject* pylzham_compress_reinit(PyObject* self, PyObject* args, PyObject* kw)
{
    PyObject *pyComp_state;
    lzham_compress_state_ptr pComp_state;

    static const char *kwlist[] = {"comp_state_ptr",
                                   0};

    if (!PyArg_ParseTupleAndKeywords(args, kw,
                                     "O:lzham_compress_reinit",
                                     (char **)kwlist,
                                     &pyComp_state)) {
        return (PyObject *)0;
    }

    pComp_state = PyLong_AsVoidPtr(pyComp_state);

    if (pComp_state)
    {
        pComp_state = lzham_lib.lzham_compress_reinit(pComp_state);

        if (pComp_state) {
            return PyLong_FromVoidPtr(pComp_state);
        }
        else {
            return lzham_exception("Failed re-initializing compressor !");
        }
    }
    else {
        return lzham_exception("Corrupted handler !");
    }
}

static PyObject* pylzham_compress(PyObject* self, PyObject* args, PyObject* kw)
{
    PyObject *ret;
    PyObject *pyComp_state;
    lzham_compress_state_ptr pComp_state;
    
    const unsigned char *data;
    Py_ssize_t data_len = 0;

    static const char *kwlist[] = {"comp_state_ptr", 
                                   "data", 
                                   0};

    if (!PyArg_ParseTupleAndKeywords(args, kw,
                                     "Os#:lzham_compress",
                                     (char **)kwlist,
                                     &pyComp_state,
                                     (char **)&data, &data_len)) {
        return (PyObject *)0;
    }

    unsigned long long in_buf_ofs = 0;
    unsigned long long out_buf_ofs = 0;

    pComp_state = PyLong_AsVoidPtr(pyComp_state);

    if (pComp_state)
    {
        ret = PyBytes_FromStringAndSize((char *)0, 2 << 12);
        lzham_compress_status_t status;

        for ( ; ; )
        {

            size_t num_in_bytes = data_len - in_buf_ofs;
            size_t out_buf_len = PyBytes_GET_SIZE(ret) - out_buf_ofs;

            status = lzham_lib.lzham_compress(pComp_state,
                                              data + in_buf_ofs,
                                              &num_in_bytes,
                                              (unsigned char *)PyBytes_AS_STRING(ret) + out_buf_ofs,
                                              &out_buf_len,
                                              true
                                              );

            if (num_in_bytes)
                in_buf_ofs += (unsigned int)num_in_bytes;

            if (out_buf_len)
                out_buf_ofs += (unsigned int)out_buf_len;

            if (status >= LZHAM_COMP_STATUS_FIRST_SUCCESS_OR_FAILURE_CODE)
            {   
                if (status != LZHAM_COMP_STATUS_SUCCESS)
                {
                   char buffer[100];
                   sprintf(buffer, "Compression failed with status code: %i", status);
                   return lzham_exception(buffer);
                
                }
                break;
            }
            else if (status == LZHAM_COMP_STATUS_HAS_MORE_OUTPUT) {
                // Taken from https://github.com/peterjc/backports.lzma/blob/6555d8b8e493a35159025b4cfc204dfb54c33d3e/backports/lzma/_lzmamodule.c#L223
                size_t size = PyBytes_GET_SIZE(ret);
                if (_PyBytes_Resize(&ret, size + (size >> 3) + 6) == -1)
                    return lzham_exception("Cannot resize the output buffer !");
            }
        }

        if (_PyBytes_Resize(&ret, out_buf_ofs) == -1)
            return lzham_exception("Cannot resize the output buffer !");

        return ret;
    }
    else {
        return lzham_exception("Corrupted handler !");
    }
}


static PyMethodDef lzham_methods[] = {
    { "get_lzham_version", pylzham_get_version, METH_NOARGS, "Return the used lzham lib version" },
    { "lzham_decompress_init", (PyCFunction)pylzham_decompress_init, METH_VARARGS | METH_KEYWORDS, "Initializes a lzham decompressor"},
    { "lzham_decompress_reinit", (PyCFunction)pylzham_decompress_reinit, METH_VARARGS | METH_KEYWORDS, "Quickly re-initializes the given lzham decompressor"},
    { "lzham_decompress", (PyCFunction)pylzham_decompress, METH_VARARGS | METH_KEYWORDS, "Decompresses a sized block of compressed data"},
    { "lzham_compress_init", (PyCFunction)pylzham_compress_init, METH_VARARGS | METH_KEYWORDS, "Initializes a lzham compressor"},
    { "lzham_compress_reinit", (PyCFunction)pylzham_compress_reinit, METH_VARARGS | METH_KEYWORDS, "Quickly re-initializes the given lzham compressor"},
    { "lzham_compress", (PyCFunction)pylzham_compress, METH_VARARGS | METH_KEYWORDS, "Compresses an arbitrarily sized block of data"},
    { NULL, NULL, 0, NULL }
};


static struct PyModuleDef lzham_def = {
    PyModuleDef_HEAD_INIT,
    "pylzham",
    "Python LZHAM Codec Wrapper",
    -1,
    lzham_methods
};

PyMODINIT_FUNC PyInit__lzham(void)
{
    lzham_lib.load();
    return PyModule_Create(&lzham_def);
}
