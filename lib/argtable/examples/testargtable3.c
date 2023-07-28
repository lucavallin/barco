#include "argtable3.h"

/* global arg_xxx structs */
struct arg_lit *a, *b, *c, *verb, *help, *version;
struct arg_int *scal;
struct arg_file *o, *file;
struct arg_end *end;

int main(int argc, char *argv[])
{
    /* the global arg_xxx structs are initialised within the argtable */
    void *argtable[] = {
        help    = arg_lit0(NULL, "help", "display this help and exit"),
        version = arg_lit0(NULL, "version", "display version info and exit"),
        a       = arg_lit0("a", NULL,"the -a option"),
        b       = arg_lit0("b", NULL, "the -b option"),
        c       = arg_lit0("c", NULL, "the -c option"),
        scal    = arg_int0(NULL, "scalar", "<n>", "foo value"),
        verb    = arg_lit0("v", "verbose", "verbose output"),
        o       = arg_file0("o", NULL, "myfile", "output file"),
        file    = arg_filen(NULL, NULL, "<file>", 1, 100, "input files"),
        end     = arg_end(20),
    };
    
    int exitcode = 0;
    char progname[] = "testargtable2.exe";
    
    int nerrors;
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n");
        printf("List information about the FILE(s) "
               "(the current directory by default).\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        exitcode = 0;
        goto exit;
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, end, progname);
        printf("Try '%s --help' for more information.\n", progname);
        exitcode = 1;
        goto exit;
    }

exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}

