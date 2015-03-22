#include <vector>
#include <limits>

extern "C" {
#include <Python.h>
}

using std::vector;

static PyObject * Fast_Floyd(PyObject* module, PyObject* args)
{
	PyObject * graph_matrix = PyTuple_GetItem(args, 0);
	
	int vertnum = PyObject_Length(graph_matrix);
	//int inf = std::numeric_limits<int>::infinity();
	vector<vector<double> > dists(vertnum, vector<double> (vertnum));
	
	for(int j = 0; j < vertnum; j++)
	{
		PyObject * matrix_row = PyList_GetItem(graph_matrix, j);
		
		for (int i = 0; i < vertnum; i++)
		{
			PyObject * element = PyList_GetItem(matrix_row, i);
			
			dists[j][i] = PyFloat_AsDouble(element);
		}
	}

	for (int k = 0; k < vertnum; k++)
	{
		for (int i = 0; i < vertnum; i++)
		{
			for (int j = 0; j < vertnum; j++)
			{
				if (i != j)
				{
					dists[i][j] = 1.0 / (1.0 / (dists[i][k] + dists[k][j]) + 1.0 / dists[i][j]);
				}
			}
		}
	}

    PyObject * py_matrix = PyList_New(vertnum);
	for (int i = 0; i < vertnum; ++i) 
	{
		PyObject * py_row = PyList_New(vertnum);
		PyList_SetItem(py_matrix, i, py_row);
		for (int j = 0; j < vertnum; ++j) 
		{
			PyObject * py_elem = PyFloat_FromDouble(dists[i][j]);
			PyList_SetItem(py_row, j, py_elem);
		}
	}

	return py_matrix;
}

PyMODINIT_FUNC PyInit_fastgraph()
{
	static PyMethodDef ModuleMethods[] = {
		{ "Fast_Floyd", Fast_Floyd, METH_VARARGS, "C++ realization of Floyd-Warshall Algorithm for resistances. This method returns the matrix of pairwise distances" },
		{ NULL, NULL, 0, NULL }
	};
	static PyModuleDef ModuleDef = {
		PyModuleDef_HEAD_INIT,
		"fastgraph",
		"Graph algorithms",
		-1, ModuleMethods, 
		NULL, NULL, NULL, NULL
	};
	PyObject * module = PyModule_Create(&ModuleDef);
	return module;
}
