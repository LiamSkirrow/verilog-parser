/*!
@file main.c
@brief A simple test program for the C library code.
*/

#include "stdio.h"

#include "verilog_parser.h"
#include "verilog_ast_common.h"
#include "verilog_preprocessor.h"
#include "verilog_ast_util.h"

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        printf("ERROR. Please supply at least one file path argument.\n");
        return 1;
    }
    else
    {
        // Initialise the parser.
        verilog_parser_init();

        int F = 0;
        for(F = 1; F < argc; F++)
        {
            // Setup the preprocessor to look in ./tests/ for include files.
            ast_list_append(yy_preproc -> search_dirs, "./tests/");
            ast_list_append(yy_preproc -> search_dirs, "./");
            printf("%s ", argv[F]);fflush(stdout);

            // Load the file.
            FILE * fh = fopen(argv[F], "r");

            verilog_preprocessor_set_file(yy_preproc, argv[F]);
            
            // Parse the file and store the result.
            int result = verilog_parse_file(fh);

            // Close the file handle
            fclose(fh);
            
            if(result == 0)
            {
                printf(" - Parse successful\n");
            }
            else
            {
                printf(" - Parse failed\n");
                if(argc<=2) return 1;
            }

        }
    }

    verilog_resolve_modules(yy_verilog_source_tree);
    printf("num items: %d\n", yy_verilog_source_tree->modules->items);
    for (int i = 0; i < yy_verilog_source_tree->modules->items; i++){
        ast_module_declaration *mod = ast_list_get(yy_verilog_source_tree->modules, i);
        printf("elem %d name: %s\n", i, mod->identifier->identifier);
        printf("num mod instantiation: %d\n", mod->module_instantiations->items);

        ast_module_instantiation *mod_dec = ast_list_get(mod->module_instantiations, 0);
        printf("is resolved?: %d\n", mod_dec->resolved);
        // ^^^ fine up to here I think ^^^

        // void *mod_dec_inst_name = ast_list_get(mod_dec->module_instances, 0);
        // printf("Name: %s\n", (char *) mod_dec->module_instances->tail->data);
    }

    return 0;
}
