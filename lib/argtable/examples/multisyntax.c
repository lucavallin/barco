/*******************************************************************************
 * Example source code for using the argtable3 library to implement
 * a multi-syntax command line argument program
 * 
 *     usage 1: multisyntax [-nvR] insert <file> [<file>]... [-o <output>]
 *     usage 2: multisyntax [-nv] remove <file>
 *     usage 3: multisyntax [-v] search <pattern> [-o <output>]
 *     usage 4: multisyntax [--help] [--version]
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

#define REG_EXTENDED 1
#define REG_ICASE (REG_EXTENDED << 1)

/* mymain1 implements the actions for syntax 1 */
int mymain1(int n, int v, int R, const char *outfile,
            const char **infiles, int ninfiles)
    {
    int i;
    printf("syntax 1 matched OK:\n");
    printf("n=%d\n", n);
    printf("v=%d\n", v);
    printf("R=%d\n", R);
    printf("outfile=\"%s\"\n", outfile);
    for (i=0; i<ninfiles; i++)
        printf("infile[%d]=\"%s\"\n",i,infiles[i]);
    return 0;
    }


/* mymain2 implements the actions for syntax 2 */
int mymain2(int n, int v, const char *infile)
    {
    printf("syntax 2 matched OK:\n");
    printf("n=%d\n", n);
    printf("v=%d\n", v);
    printf("infile=\"%s\"\n", infile);
    return 0;
    }


/* mymain3 implements the actions for syntax 3 */
int mymain3(int v, const char *pattern, const char *outfile)
    {
    printf("syntax 3 matched OK:\n");
    printf("v=%d\n", v);
    printf("pattern=\"%s\"\n", pattern);
    printf("outfile=\"%s\"\n", outfile);
    return 0;
    }


/* mymain4 implements the actions for syntax 4 */
int mymain4(int help, int version, const char *progname,
            void *argtable1, void *argtable2, void *argtable3, void *argtable4)
    {
    /* --help option */
    if (help)
        {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout,argtable1,"\n");
        printf("       %s", progname);
        arg_print_syntax(stdout,argtable2,"\n");
        printf("       %s", progname);
        arg_print_syntax(stdout,argtable3,"\n");
        printf("       %s", progname);
        arg_print_syntax(stdout,argtable4,"\n");
        printf("This program demonstrates the use of the argtable2 library\n");
        printf("for parsing multiple command line syntaxes.\n");
        arg_print_glossary(stdout,argtable1,"      %-20s %s\n");
        arg_print_glossary(stdout,argtable2,"      %-20s %s\n");
        arg_print_glossary(stdout,argtable3,"      %-20s %s\n");
        arg_print_glossary(stdout,argtable4,"      %-20s %s\n");
        return 0;
        }

    /* --version option */
    if (version)
        {
        printf("'%s' example program for the \"argtable\" command line argument parser.\n",progname);
        return 0;
        }

    /* no command line options at all */
    printf("Try '%s --help' for more information.\n", progname);
    return 0;
    }


int main(int argc, char **argv)
    {
    /* SYNTAX 1: insert [-nvR] <file> [file]...  -o <file> */
    struct arg_rex  *cmd1     = arg_rex1(NULL,  NULL,  "insert", NULL, REG_ICASE, NULL);
    struct arg_lit  *noact1   = arg_lit0("n",   NULL,  "take no action");
    struct arg_lit  *verbose1 = arg_lit0("v", "verbose", "verbose messages");
    struct arg_lit  *recurse1 = arg_lit0("R",   NULL,  "recurse through subdirectories");
    struct arg_file *infiles1 = arg_filen(NULL, NULL,  NULL, 1,argc+2, "input file(s)");
    struct arg_file *outfile1 = arg_file0("o",  NULL,  "<output>", "output file (default is \"-\")");
    struct arg_end  *end1     = arg_end(20);
    void* argtable1[] = {cmd1,noact1,verbose1,recurse1,infiles1,outfile1,end1};
    int nerrors1;

    /* SYNTAX 2: remove [-nv] <file> */
    struct arg_rex  *cmd2     = arg_rex1(NULL, NULL, "remove", NULL, REG_ICASE, NULL);
    struct arg_lit  *noact2   = arg_lit0("n",  NULL, NULL);
    struct arg_lit  *verbose2 = arg_lit0("v",  "verbose", NULL);
    struct arg_file *infiles2 = arg_file1(NULL, NULL, NULL, NULL);
    struct arg_end  *end2     = arg_end(20);
    void* argtable2[] = {cmd2,noact2,verbose2,infiles2,end2};
    int nerrors2;

    /* SYNTAX 3: search [-v] <pattern> [-o <file>] [--help] [--version] */
    struct arg_rex  *cmd3     = arg_rex1(NULL, NULL, "search", NULL, REG_ICASE, NULL);
    struct arg_lit  *verbose3 = arg_lit0("v",  "verbose", NULL);
    struct arg_str  *pattern3 = arg_str1(NULL, NULL, "<pattern>", "search string");
    struct arg_file *outfile3 = arg_file0("o", NULL, "<output>", NULL);
    struct arg_end  *end3     = arg_end(20);
    void* argtable3[] = {cmd3,verbose3,pattern3,outfile3,end3};
    int nerrors3;

    /* SYNTAX 4: [-help] [-version] */
    struct arg_lit  *help4    = arg_lit0(NULL,"help",     "print this help and exit");
    struct arg_lit  *version4 = arg_lit0(NULL,"version",  "print version information and exit");
    struct arg_end  *end4     = arg_end(20);
    void* argtable4[] = {help4,version4,end4};
    int nerrors4;

    const char* progname = "multisyntax";
    int exitcode=0;

    /* verify all argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable1)!=0 ||
        arg_nullcheck(argtable2)!=0 ||
        arg_nullcheck(argtable3)!=0 ||
        arg_nullcheck(argtable4)!=0 )
        {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n",progname);
        exitcode=1;
        goto exit;
        }

    /* set any command line default values prior to parsing */
    outfile1->filename[0]="-";
    outfile3->filename[0]="-";

    /* Above we defined a separate argtable for each possible command line syntax */
    /* and here we parse each one in turn to see if any of them are successful    */
    nerrors1 = arg_parse(argc,argv,argtable1);
    nerrors2 = arg_parse(argc,argv,argtable2);
    nerrors3 = arg_parse(argc,argv,argtable3);
    nerrors4 = arg_parse(argc,argv,argtable4);

    /* Execute the appropriate main<n> routine for the matching command line syntax */
    /* In this example program our alternate command line syntaxes are mutually     */
    /* exclusive, so we know in advance that only one of them can be successful.    */
    if (nerrors1==0)
        exitcode = mymain1(noact1->count, verbose1->count, recurse1->count,
                           outfile1->filename[0], infiles1->filename, infiles1->count);
    else if (nerrors2==0)
        exitcode = mymain2(noact2->count, verbose2->count, infiles2->filename[0]);
    else if (nerrors3==0)
        exitcode = mymain3(verbose3->count, pattern3->sval[0], outfile3->filename[0]);
    else if (nerrors4==0)
        exitcode = mymain4(help4->count, version4->count, progname,
                           argtable1, argtable2, argtable3, argtable4);
    else
        {
        /* We get here if the command line matched none of the possible syntaxes */
        if (cmd1->count > 0)
            {
            /* here the cmd1 argument was correct, so presume syntax 1 was intended target */ 
            arg_print_errors(stdout,end1,progname);
            printf("usage: %s ", progname);
            arg_print_syntax(stdout,argtable1,"\n");
            }
        else if (cmd2->count > 0)
            {
            /* here the cmd2 argument was correct, so presume syntax 2 was intended target */ 
            arg_print_errors(stdout,end2,progname);
            printf("usage: %s ", progname);
            arg_print_syntax(stdout,argtable2,"\n");
            }
        else if (cmd3->count > 0)
            {
            /* here the cmd3 argument was correct, so presume syntax 3 was intended target */ 
            arg_print_errors(stdout,end3,progname);
            printf("usage: %s ", progname);
            arg_print_syntax(stdout,argtable3,"\n");
            }
        else
            {
            /* no correct cmd literals were given, so we cant presume which syntax was intended */
            printf("%s: missing <insert|remove|search> command.\n",progname); 
            printf("usage 1: %s ", progname);  arg_print_syntax(stdout,argtable1,"\n");
            printf("usage 2: %s ", progname);  arg_print_syntax(stdout,argtable2,"\n");
            printf("usage 3: %s ", progname);  arg_print_syntax(stdout,argtable3,"\n");
            printf("usage 4: %s",  progname);  arg_print_syntax(stdout,argtable4,"\n");
            }
        }

exit:
    /* deallocate each non-null entry in each argtable */
    arg_freetable(argtable1,sizeof(argtable1)/sizeof(argtable1[0]));
    arg_freetable(argtable2,sizeof(argtable2)/sizeof(argtable2[0]));
    arg_freetable(argtable3,sizeof(argtable3)/sizeof(argtable3[0]));
    arg_freetable(argtable4,sizeof(argtable4)/sizeof(argtable4[0]));

    return exitcode;
    }
