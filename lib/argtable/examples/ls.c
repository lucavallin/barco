/*******************************************************************************
 * Example source code for using the argtable3 library to implement:
 * 
 *   ls [-aAbBcCdDfFgGhHiklLmnNopqQrRsStuUvxX1] [--author]
 *   [--block-size=SIZE] [--color=[WHEN]] [--format=WORD] [--full-time]
 *   [--si] [--dereference-command-line-symlink-to-dir] [--indicator-style=WORD]
 *   [-I PATTERN] [--show-control-chars] [--quoting-style=WORD] [--sort=WORD]
 *   [--time=WORD] [--time-style=STYLE] [-T COLS] [-w COLS] [--help]
 *   [--version] [FILE]...
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

/* These variables hold the values parsed from the comand line by arg_parse() */
struct arg_lit *a, *A, *author, *b, *B, *c, *C, *d, *D, *f, *F, *fulltime;
struct arg_lit *g, *G, *h, *H, *si, *deref, *i, *k, *l, *L, *m, *n, *N, *o, *p;
struct arg_lit *q, *shcont, *Q, *r, *R, *s, *S, *t, *u, *U, *v, *x, *X, *one;
struct arg_lit *help, *version;
struct arg_int *blocksize, *T, *w;
struct arg_str *color, *format, *indic, *I, *Qstyle, *sort, *Time, *timesty;
struct arg_file *files;
struct arg_end *end;

/* Here we simply echo the command line option values as a demonstration. */
/* In a real program, this is where we would perform the main processing. */
int mymain(void)
    {
    int j;

    if (a->count > 0)
        printf("a=YES\n");
    if (A->count > 0)
        printf("A=YES\n");
    if (author->count > 0)
        printf("author=YES\n");
    if (b->count > 0)
        printf("b=YES\n");
    if (blocksize->count > 0)
        printf("blocksize=%d\n",blocksize->count);
    if (B->count > 0)
        printf("B=YES\n");
    if (c->count > 0)
        printf("c=YES\n");
    if (C->count > 0)
        printf("C=YES\n");
    if (color->count > 0)
        printf("color=%s\n",color->sval[0]);
    if (d->count > 0)
        printf("d=YES\n");
    if (D->count > 0)
        printf("D=YES\n");
    if (f->count > 0)
        printf("f=YES\n");
    if (F->count > 0)
        printf("F=YES\n");
    if (format->count > 0)
        printf("format=%s\n",format->sval[0]);
    if (fulltime->count > 0)
        printf("fulltime=YES\n");
    if (g->count > 0)
        printf("g=YES\n");
    if (G->count > 0)
        printf("G=YES\n");
    if (h->count > 0)
        printf("h=YES\n");
    if (si->count > 0)
        printf("si=YES\n");
    if (H->count > 0)
        printf("H=YES\n");
    if (deref->count > 0)
        printf("deref=YES\n");
    if (indic->count > 0)
        printf("indic=%s\n",indic->sval[0]);
    if (i->count > 0)
        printf("i=YES\n");
    if (I->count > 0)
        printf("I=%s\n",I->sval[0]);
    if (k->count > 0)
        printf("k=YES\n");
    if (l->count > 0)
        printf("l=YES\n");
    if (L->count > 0)
        printf("L=YES\n");
    if (m->count > 0)
        printf("m=YES\n");
    if (n->count > 0)
        printf("n=YES\n");
    if (N->count > 0)
        printf("N=YES\n");
    if (o->count > 0)
        printf("o=YES\n");
    if (p->count > 0)
        printf("p=YES\n");
    if (q->count > 0)
        printf("q=YES\n");
    if (shcont->count > 0)
        printf("shcont=YES\n");
    if (Q->count > 0)
        printf("Q=YES\n");
    if (Qstyle->count > 0)
        printf("Qstyle=%s\n",Qstyle->sval[0]);
    if (r->count > 0)
        printf("r=YES\n");
    if (R->count > 0)
        printf("R=YES\n");
    if (s->count > 0)
        printf("s=YES\n");
    if (S->count > 0)
        printf("S=YES\n");
    if (sort->count > 0)
        printf("sort=%s\n",sort->sval[0]);
    if (Time->count > 0)
        printf("time=%s\n",Time->sval[0]);
    if (timesty->count > 0)
        printf("timesty=%s\n",timesty->sval[0]);
    if (t->count > 0)
        printf("t=YES\n");
    if (T->count > 0)
        printf("T=%d\n",T->ival[0]);
    if (u->count > 0)
        printf("u=YES\n");
    if (U->count > 0)
        printf("U=YES\n");
    if (v->count > 0)
        printf("v=YES\n");
    if (w->count > 0)
        printf("w=%d\n",w->ival[0]);
    if (x->count > 0)
        printf("x=YES\n");
    if (X->count > 0)
        printf("X=YES\n");
    if (one->count > 0)
        printf("1=YES\n");

    /* print the filenames */
    for (j=0; j<files->count; j++)
        printf("filename[%d] = \"%s\"\n", j, files->filename[j]);

    return 0;
    }


int main(int argc, char **argv)
    {
    /* The argtable[] entries define the command line options */
    void *argtable[] = {
                a = arg_lit0("a", "all",                 "do not hide entries starting with ."),
                A = arg_lit0("A", "almost-all",          "do not list implied . and .."),
           author = arg_lit0(NULL,"author",              "print the author of each file"),
                b = arg_lit0("b", "escape",              "print octal escapes for nongraphic characters"),
        blocksize = arg_int0(NULL,"block-size","SIZE",   "use SIZE-byte blocks"),
                B = arg_lit0("B", "ignore-backups",      "do not list implied entries ending with ~"),
                c = arg_lit0("c", NULL,                  "with -lt: sort by, and show, ctime (time of last"),
                    arg_rem(NULL,                        "  modification of file status information)"),
                    arg_rem(NULL,                        "  with -l: show ctime and sort by name"),
                    arg_rem(NULL,                        "  otherwise: sort by ctime"),
                C = arg_lit0("C", NULL,                  "list entries by columns"),
            color = arg_str0(NULL,"color","WHEN",        "control whether color is used to distinguish file"),
                    arg_rem(NULL,                        "  types.  WHEN may be `never', `always', or `auto'"),
                d = arg_lit0("d", "directory",           "list directory entries instead of contents,"),
                    arg_rem(NULL,                        "  and do not dereference symbolic links"),
                D = arg_lit0("D", "dired",               "generate output designed for Emacs' dired mode"),
                f = arg_lit0("f", NULL,                  "do not sort, enable -aU, disable -lst"),
                F = arg_lit0("F", "classify",            "append indicator (one of */=@|) to entries"),
           format = arg_str0(NULL,"format","WORD",       "across -x, commas -m, horizontal -x, long -l,"),
                    arg_rem (NULL,                       "  single-column -1, verbose -l, vertical -C"),
         fulltime = arg_lit0(NULL,"full-time",           "like -l --time-style=full-iso"),
                g = arg_lit0("g", NULL,                  "like -l, but do not list owner"),
                G = arg_lit0("G", "no-group",            "inhibit display of group information"),
                h = arg_lit0("h", "human-readable",      "print sizes in human readable format (e.g., 1K 234M 2G)"),
               si = arg_lit0(NULL,"si",                  "likewise, but use powers of 1000 not 1024"),
                H = arg_lit0("H", "dereference-command-line","follow symbolic links listed on the command line"),
            deref = arg_lit0(NULL,"dereference-command-line-symlink-to-dir","follow each command line symbolic link"),
                    arg_rem(NULL,                       "  that points to a directory"),
            indic = arg_str0(NULL,"indicator-style","WORD","append indicator with style WORD to entry names:"),
                    arg_rem (NULL,                       "  none (default), classify (-F), file-type (-p)"),
                i = arg_lit0("i", "inode",               "print index number of each file"),
                I = arg_str0("I", "ignore","PATTERN",    "do not list implied entries matching shell PATTERN"),
                k = arg_lit0("k", NULL,                  "like --block-size=1K"),
                l = arg_lit0("l", NULL,                  "use a long listing format"),
                L = arg_lit0("L", "dereference",         "when showing file information for a symbolic"),
                    arg_rem (NULL,                       "  link, show information for the file the link"),
                    arg_rem (NULL,                       "  references rather than for the link itself"),
                m = arg_lit0("m", NULL,                  "fill width with a comma separated list of entries"),
                n = arg_lit0("n", "numeric-uid-gid",     "like -l, but list numeric UIDs and GIDs"),
                N = arg_lit0("N", "literal",             "print raw entry names (don't treat e.g. control"),
                    arg_rem (NULL,                       "  characters specially)"),
                o = arg_lit0("o", NULL,                  "like -l, but do not list group information"),
                p = arg_lit0("p", "file-type",           "append indicator (one of /=@|) to entries"),
                q = arg_lit0("q", "hide-control-chars",  "print ? instead of non graphic characters"),
           shcont = arg_lit0(NULL,"show-control-chars",  "show non graphic characters as-is (default"),
                    arg_rem (NULL,                       "unless program is `ls' and output is a terminal)"),
                Q = arg_lit0("Q", "quote-name",          "enclose entry names in double quotes"),
           Qstyle = arg_str0(NULL,"quoting-style","WORD","use quoting style WORD for entry names:"),
                    arg_rem (NULL,                       "  literal, locale, shell, shell-always, c, escape"),
                r = arg_lit0("r", "reverse",             "reverse order while sorting"),
                R = arg_lit0("R", "recursive",           "list subdirectories recursively"),
                s = arg_lit0("s", "size",                "print size of each file, in blocks"),
                S = arg_lit0("S", NULL,                  "sort by file size"),
             sort = arg_str0(NULL,"sort","WORD",         "extension -X, none -U, size -S, time -t, version -v,"),
                    arg_rem (NULL,                       "status -c, time -t, atime -u, access -u, use -u"),
             Time = arg_str0(NULL,"time","WORD",         "show time as WORD instead of modification time:"),
                    arg_rem (NULL,                       "  atime, access, use, ctime or status; use"),
                    arg_rem (NULL,                       "  specified time as sort key if --sort=time"),
          timesty = arg_str0(NULL, "time-style","STYLE", "show times using style STYLE:"),
                    arg_rem (NULL,                       "  full-iso, long-iso, iso, locale, +FORMAT"),
                    arg_rem (NULL,                       "FORMAT is interpreted like `date'; if FORMAT is"),
                    arg_rem (NULL,                       "FORMAT1<newline>FORMAT2, FORMAT1 applies to"),
                    arg_rem (NULL,                       "non-recent files and FORMAT2 to recent files;"),
                    arg_rem (NULL,                       "if STYLE is prefixed with `posix-', STYLE"),
                    arg_rem (NULL,                       "takes effect only outside the POSIX locale"),
                t = arg_lit0("t", NULL,                  "sort by modification time"),
                T = arg_int0("T", "tabsize", "COLS",     "assume tab stops at each COLS instead of 8"),
                u = arg_lit0("u", NULL,                  "with -lt: sort by, and show, access time"),
                    arg_rem (NULL,                       "  with -l: show access time and sort by name"),
                    arg_rem (NULL,                       "  otherwise: sort by access time"),
                U = arg_lit0("U", NULL,                  "do not sort; list entries in directory order"),
                v = arg_lit0("v", NULL,                  "sort by version"),
                w = arg_int0("w", "width", "COLS",       "assume screen width instead of current value"),
                x = arg_lit0("x", NULL,                  "list entries by lines instead of by columns"),
                X = arg_lit0("X", NULL,                  "sort alphabetically by entry extension"),
              one = arg_lit0("1", NULL,                  "list one file per line"),
             help = arg_lit0(NULL,"help",                "display this help and exit"),
          version = arg_lit0(NULL,"version",             "display version information and exit"),
            files = arg_filen(NULL, NULL, "FILE", 0, argc+2, NULL),
              end = arg_end(20),
        };
    const char *progname = "ls";
    int exitcode=0;
    int nerrors;

    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
        {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n",progname);
        exitcode=1;
        goto exit;
        }

    /* allow optional argument values for --color */
    /* and set the default value to "always" */
    color->hdr.flag |= ARG_HASOPTVALUE;
    color->sval[0] = "always";

    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
        {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout,argtable,"\n");
        printf("List information about the FILE(s) (the current directory by default).\n");
        printf("Sort entries alphabetically if none of -cftuSUX nor --sort.\n\n");
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        printf("\nSIZE may be (or may be an integer optionally followed by) one of following:\n"
               "kB 1000, K 1024, MB 1,000,000, M 1,048,576, and so on for G, T, P, E, Z, Y.\n\n"
               "By default, color is not used to distinguish types of files.  That is\n"
               "equivalent to using --color=none.  Using the --color option without the\n"
               "optional WHEN argument is equivalent to using --color=always.  With\n"
               "--color=auto, color codes are output only if standard output is connected\n"
               "to a terminal (tty).\n\n"
               "Report bugs to <foo@bar>.\n");
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

    /* Command line parsing is complete, do the main processing */
    exitcode = mymain();

exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

    return exitcode;
    }


