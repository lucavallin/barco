/*******************************************************************************
 * This example source code is an alternate version of myprog.c
 * that adheres to ansi C89 standards rather than ansi C99.
 * The only difference being that C89 does not permit the argtable array
 * to be statically initialized with the contents of variables set at
 * runtime whereas C99 does.
 * Hence we cannot declare and initialize the argtable array in one declaration
 * as
 *     void* argtable[] = {list, recurse, repeat, defines, outfile, verbose,
 *                         help, version, infiles, end};
 * Instead, we must declare 
 *     void* argtable[10];
 * and initialize the contents of the array separately.
 * 
 * This file is part of the argtable3 library.
 *
 * Copyright (C) 1998-2001,2003-2011,2013 Stewart Heitmann
 * <sheitmann@users.sourceforge.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of STEWART HEITMANN nor the  names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL STEWART HEITMANN BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "argtable3.h"

int mymain(int l, int R, int k,
           const char **defines, int ndefines,
           const char *outfile,
           int v,
           const char **infiles, int ninfiles)
    {
    int i;

    if (l>0) printf("list files (-l)\n");
    if (R>0) printf("recurse through directories (-R)\n");
    if (v>0) printf("verbose is enabled (-v)\n");
    printf("scalar k=%d\n",k);
    printf("output is \"%s\"\n", outfile);

    for (i=0; i<ndefines; i++)
        printf("user defined macro \"%s\"\n",defines[i]);

    for (i=0; i<ninfiles; i++)
        printf("infile[%d]=\"%s\"\n",i,infiles[i]);

    return 0;
    }


int main(int argc, char **argv)
    {
    struct arg_lit  *list    = arg_lit0("lL",NULL,                      "list files");
    struct arg_lit  *recurse = arg_lit0("R",NULL,                       "recurse through subdirectories");
    struct arg_int  *repeat  = arg_int0("k","scalar",NULL,              "define scalar value k (default is 3)");
    struct arg_str  *defines = arg_strn("D","define","MACRO",0,argc+2,  "macro definitions");
    struct arg_file *outfile = arg_file0("o",NULL,"<output>",           "output file (default is \"-\")");
    struct arg_lit  *verbose = arg_lit0("v","verbose,debug",            "verbose messages");
    struct arg_lit  *help    = arg_lit0(NULL,"help",                    "print this help and exit");
    struct arg_lit  *version = arg_lit0(NULL,"version",                 "print version information and exit");
    struct arg_file *infiles = arg_filen(NULL,NULL,NULL,1,argc+2,       "input file(s)");
    struct arg_end  *end     = arg_end(20);
    void* argtable[10];
    const char* progname = "myprog_C89";
    int nerrors;
    int exitcode=0;

    /* initialize the argtable array with ptrs to the arg_xxx structures constructed above */
    argtable[0] = list;
    argtable[1] = recurse;
    argtable[2] = repeat;
    argtable[3] = defines;
    argtable[4] = outfile;
    argtable[5] = verbose;
    argtable[6] = help;
    argtable[7] = version;
    argtable[8] = infiles;
    argtable[9] = end;
        
    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
        {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n",progname);
        exitcode=1;
        goto exit;
        }

    /* set any command line default values prior to parsing */
    repeat->ival[0]=3;
    outfile->filename[0]="-";

    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
        {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout,argtable,"\n");
        printf("This program demonstrates the use of the argtable2 library\n");
        printf("for parsing command line arguments.\n");
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        exitcode=0;
        goto exit;
        }

    /* special case: '--version' takes precedence error reporting */
    if (version->count > 0)
        {
        printf("'%s' example program for the \"argtable\" command line argument parser.\n",progname);
        printf("September 2003, Stewart Heitmann\n");
        exitcode=0;
        goto exit;
        }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
        {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout,end,progname);
        printf("Try '%s --help' for more information.\n",progname);
        exitcode=1;
        goto exit;
        }

    /* special case: uname with no command line options induces brief help */
    if (argc==1)
        {
        printf("Try '%s --help' for more information.\n",progname);
        exitcode=0;
        goto exit;
        }

    /* normal case: take the command line options at face value */
    exitcode = mymain(list->count, recurse->count, repeat->ival[0],
                      defines->sval, defines->count,
                      outfile->filename[0], verbose->count,
                      infiles->filename, infiles->count);

    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

    return exitcode;
    }
