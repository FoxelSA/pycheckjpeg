/*
 * pycheckjpeg - A python module to check integrity of JPEG files
 *
 * Copyright (c) 2013-2014 FOXEL SA - http://foxel.ch
 * Please read <http://foxel.ch/license> for more information.
 *
 *
 * Author(s):
 *
 *      Kevin Velickovic <k.velickovic@foxel.ch>
 *
 *
 * This file is part of the FOXEL project <http://foxel.ch>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Additional Terms:
 *
 *      You are required to preserve legal notices and author attributions in
 *      that material or in the Appropriate Legal Notices displayed by works
 *      containing it.
 *
 *      You are required to attribute the work as explained in the "Usage and
 *      Attribution" section of <http://foxel.ch/license>.
 */

#include "Python.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <ctype.h>
#include <jpeglib.h>
#include <sys/stat.h>

/* Global error counter */
int total_errors = 0;
char * Errors[16];

/* Custom error manager */
struct my_error_mgr {
	struct jpeg_error_mgr pub; /* "public" fields */
	jmp_buf setjmp_buffer; /* for return to caller */
};

struct my_error_mgr jerr;

/* Function to insert an error message */
void insertError(const char* message)
{

	/* Determine length of message */
	int message_len = strlen(message);

	/* Allocate memory to store message in array */
	Errors[total_errors] = malloc(sizeof(char) * message_len);
	memset(Errors[total_errors], 0x00, message_len);

	/* Copy message to array */
	strncpy(Errors[total_errors], message, message_len);

	/* Increment errors count */
	total_errors++;
}

/* Custom error struct */
typedef struct my_error_mgr * my_error_ptr;

/* Method to catch error exit */
METHODDEF(void)
exit_method (j_common_ptr cinfo)
{

	/* Gets the error pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	/* Display error message */
	(*cinfo->err->output_message) (cinfo);

	/* Restore the error environment */
	longjmp(myerr->setjmp_buffer, 1);
}

/* Method to error output */
METHODDEF(void)
output_method (j_common_ptr cinfo)
{
	/* Initialize buffer to store error message */
	char buffer[JMSG_LENGTH_MAX];

	/* Format error message */
	(*cinfo->err->format_message) (cinfo, buffer);

	/* Allocate memory to store message in array */
	Errors[total_errors] = malloc(sizeof(char) * JMSG_LENGTH_MAX);
	memset(Errors[total_errors], 0x00, JMSG_LENGTH_MAX);

	/* Copy message to array */
	strncpy(Errors[total_errors], buffer, JMSG_LENGTH_MAX);

	/* Increment errors count */
	total_errors++;
}

/* Function to verify integrity of a JPEG file from buffer */
void validate_jpeg_from_buffer(unsigned char * in_buffer, int in_length)
{
	/* Reset the error counter */
	total_errors = 0;

	/* Scope variables */
	struct jpeg_decompress_struct cinfo = {0};
	JSAMPARRAY buffer;
	int row_stride;

	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = exit_method;
	jerr.pub.output_message = output_method;

	/* Speed optimisations */
	cinfo.out_color_space = JCS_GRAYSCALE;
	cinfo.scale_denom = 8;
	cinfo.scale_num = 1;

	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) {

		/* If we get here, the JPEG code has signaled an error.
		* We need to clean up the JPEG object, close the input file, and return.
		*/
		jpeg_destroy_decompress(&cinfo);

		return;
	}

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);

	/* specify data source (eg, a file) */
	jpeg_mem_src(&cinfo, in_buffer, (long unsigned int)in_length);

	/* read file parameters with jpeg_read_header() */
	(void) jpeg_read_header(&cinfo, TRUE);

	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);

	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
	((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	/* Here we use the library's state variable cinfo.output_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	*/
	while (cinfo.output_scanline < cinfo.output_height) {

		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could ask for
		* more than one scanline at a time if that's more convenient.
		*/
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
	}

	/* Finish decompression */
	(void) jpeg_finish_decompress(&cinfo);

	/* Release JPEG decompression object */
	jpeg_destroy_decompress(&cinfo);

	/* Return result (Ok) */
	return;
}

/* Function to create a python list object from a C char* list */
PyObject * makelist(char* array[], size_t size) {

	/* Create new python object */
	PyObject *l = PyList_New(size);

	/* Loop index */
	size_t i = 0;

	/* Iterate over elements */
	for (i = 0; i != size; ++i) {

		/* Insert item into list */
		PyList_SET_ITEM(l, i, PyString_FromString(array[i]));
	}

	/* Return result */
	return l;
}

/* The module doc string */
PyDoc_STRVAR(pycheckjpeg__doc__,
"A jpeg file checker");

/* The functions doc strings */
PyDoc_STRVAR(validate_jpeg_from_file__doc__,
"Function to check validity of a JPEG file from file");

PyDoc_STRVAR(validate_jpeg_from_buffer__doc__,
"Function to check validity of a JPEG file from buffer");

/* The wrapper to the underlying C function of validate_jpeg */
static PyObject *
py_validate_jpeg_from_file(PyObject *self, PyObject *args)
{
	/* Arguments containers */
	char* path = "";

	/* Try to parse arguments */
	if (!PyArg_ParseTuple(args, "s:validate_jpeg_from_file", &path))
		return NULL;

	/* File variables */
	FILE *file;
	unsigned char *buffer;
	unsigned long fileLen;

	/* Open file */
	file = fopen(path, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", path);
		return 0;
	}

	/* Get file length */
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	/* Allocate memory */
	buffer=(unsigned char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
        fclose(file);

		return 0;
	}

	/* Read file contents into buffer */
	if(fread(buffer, fileLen, 1, file));
	fclose(file);

	/* Validate image */
	validate_jpeg_from_buffer(buffer, fileLen);
	PyObject * errors = makelist(Errors, total_errors);

	/* Free file buffer */
	free(buffer);

	/* Return result */
	return errors;
}

/* The wrapper to the underlying C function for validate_jpeg_from_buffer */
static PyObject *
py_validate_jpeg_from_buffer(PyObject *self, PyObject *args)
{
	/* Arguments containers */
	unsigned char * buffer;
	int buffer_length = 0;

	/* Try to parse arguments */
	if (!PyArg_ParseTuple(args, "s#:validate_jpeg_from_buffer", &buffer, &buffer_length))
		return NULL;

	/* Validate image */
	validate_jpeg_from_buffer(buffer, buffer_length);
	PyObject * errors = makelist(Errors, total_errors);

	/* Return result */
	return errors;
}

/* Internal python methods bindings */
static PyMethodDef pycheckjpeg_methods[] = {
	{"validate_jpeg_from_file",  py_validate_jpeg_from_file, METH_VARARGS, validate_jpeg_from_file__doc__},
	{"validate_jpeg_from_buffer",  py_validate_jpeg_from_buffer, METH_VARARGS, validate_jpeg_from_buffer__doc__},
	{NULL, NULL}      /* sentinel */
};

/* Internal python module initializer */
PyMODINIT_FUNC
initpycheckjpeg(void)
{

	/* Initialize module */
	Py_InitModule3("pycheckjpeg", pycheckjpeg_methods,
                   pycheckjpeg__doc__);
}
