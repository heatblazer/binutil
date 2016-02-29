#include <stdio.h>
#include <stdlib.h>
#define DEBUG 1


static FILE* open_or_exit(const char* fname, char* perms)
{
    FILE* fp = fopen(fname, perms);
    if ( !fp )
        exit(EXIT_FAILURE);
    else
        return fp;
}


int main(int argc, char** argv)
{
    if ( argc < 3 )
    {
        fprintf(stderr, "ERROR usage!\ng2bin <infile> <outfile>\n");
        return 1;
    }
    char varname[128]={0};
    if ( argc == 4 )
    {
        sprintf(varname, "%s", argv[3]);
    } else
    {
        sprintf(varname, "%s", "default");
    }

    FILE *infile = open_or_exit(argv[1], "rb");
    FILE* outfile = open_or_exit(argv[2], "w");

    unsigned short buff[256]={0};
    size_t line=0;
    size_t nread=0;

    fprintf(outfile, "#include <stdlib.h>\n");
    fprintf(outfile, "const char ");
    fprintf(outfile, "_binary_%s", varname);
    fprintf(outfile, "[]={\n");
    do
    {
        nread = fread(buff, 1, sizeof(buff), infile);
#ifdef DEBUG
      printf("[%d] bytes read\n", nread);
#endif
        for(int i=0; i < (sizeof(buff)/sizeof(buff[0])); i++)
        {
            fprintf(outfile, "0x%02x", buff[i]);
            if ( nread > 0)
                fprintf(outfile, ",");
            else
                break;
            if ( line++ >= 10 )
            {
                fprintf(outfile, "\n");
                line = 0;
            }
        }

    } while ( nread > 0);
    fprintf(outfile, "};\n");
    fprintf(outfile, "const size_t _binary_%s_size=sizeof(_binary_%s);\n"
            ,varname, varname);

    fclose(infile);
    fclose(outfile);

    return 0;
}

