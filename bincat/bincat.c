/* vim: set tabstop=8 shiftwidth=8: */
//=============================================================================
// Name: bincat.c
// Purpose: generate text data file with bin data file
// To build: gcc -std=c99 -o bincat bincat.c
// Copyright (C) 2008 by ZHOU Cheng. All right reserved.
//=============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcmp, etc

#include "error.h"
#include "if.h"

//=============================================================================
// Variables definition:
//=============================================================================
static FILE *fd_i = NULL;
static char file_i[FILENAME_MAX] = "";
static int npline = 188; // data number per line

//=============================================================================
// Sub-function declare:
//=============================================================================
static int deal_with_parameter(int argc, char *argv[]);
static void show_help();
static void show_version();

//=============================================================================
// The main function:
//=============================================================================
int main(int argc, char *argv[])
{
        unsigned char bbuf[ LINE_LENGTH_MAX / 3 + 10]; // bin data buffer
        char tbuf[LINE_LENGTH_MAX + 10]; // txt data buffer

        if(0 != deal_with_parameter(argc, argv))
        {
                return -1;
        }

        fd_i = fopen(file_i, "rb");
        if(NULL == fd_i)
        {
                DBG(ERR_FOPEN_FAILED);
                return -ERR_FOPEN_FAILED;
        }

        while(1 == fread(bbuf, npline, 1, fd_i))
        {
                b2t(tbuf, bbuf, npline);
                puts(tbuf);
        }

        fclose(fd_i);

        return 0;
}

//=============================================================================
// Subfunctions definition:
//=============================================================================
static int deal_with_parameter(int argc, char *argv[])
{
        int i;
        int dat;

        if(1 == argc)
        {
                // no parameter
                fprintf(stderr, "No binary file to process...\n\n");
                show_help();
                return -1;
        }

        for(i = 1; i < argc; i++)
        {
                if('-' == argv[i][0])
                {
                        if(0 == strcmp(argv[i], "-n"))
                        {
                                sscanf(argv[++i], "%i" , &dat);
                                if(0 < dat && dat <= (LINE_LENGTH_MAX / 3))
                                {
                                        npline = dat;
                                }
                                else
                                {
                                        fprintf(stderr,
                                                "bad variable for '-n': %d, use %d instead!\n",
                                                dat, npline);
                                }
                        }
                        else if(0 == strcmp(argv[i], "-h") ||
                                0 == strcmp(argv[i], "--help")
                        )
                        {
                                show_help();
                                return -1;
                        }
                        else if(0 == strcmp(argv[i], "-v") ||
                                0 == strcmp(argv[i], "--version")
                        )
                        {
                                show_version();
                                return -1;
                        }
                        else
                        {
                                fprintf(stderr, "Wrong parameter: %s\n", argv[i]);
                                DBG(ERR_BAD_ARG);
                                return -ERR_BAD_ARG;
                        }
                }
                else
                {
                        strcpy(file_i, argv[i]);
                }
        }

        return 0;
}

static void show_help()
{
        puts("'bincat' read TS file, translate 0xXX to 'XY ' format, then send to stdout.");
        puts("");
        puts("Usage: bincat [OPTION] file [OPTION]");
        puts("");
        puts("Options:");
        puts("");
        puts(" -n <npl>         data count per line, [1,10922], default: 188");
        puts(" -h, --help       display this information");
        puts(" -v, --version    display my version");
        puts("");
        puts("Examples:");
        puts("  bincat xxx.ts");
        puts("");
        puts("Report bugs to <zhoucheng@tsinghua.org.cn>.");
        return;
}

static void show_version()
{
        //fprintf(stdout, "bincat 0.1.0 (by Cygwin), %s %s\n", __TIME__, __DATE__);
        puts("bincat 1.0.0");
        puts("");
        puts("Copyright (C) 2009,2010 ZHOU Cheng.");
        puts("This is free software; contact author for additional information.");
        puts("There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR");
        puts("A PARTICULAR PURPOSE.");
        puts("");
        puts("Written by ZHOU Cheng.");
        return;
}

#if 0
static void sync_input(obj_t *obj)
{
        int sync_byte;
        URL *url = obj->url;

        obj->addr = 0;
        url_seek(url, 0, SEEK_SET);
        do
        {
                if(EOF == (sync_byte = url_getc(obj->url)))
                {
                        break;
                }
                else if(0x47 == sync_byte)
                {
                        url_seek(url, obj->ts_size - 1, SEEK_CUR);
                        if(EOF == (sync_byte = url_getc(obj->url)))
                        {
                                break;
                        }
                        else if(0x47 == sync_byte)
                        {
                                // sync, go back
                                url_seek(url, -(obj->ts_size + 1), SEEK_CUR);
                                break;
                        }
                        else
                        {
                                // not real sync byte
                                url_seek(url, -(obj->ts_size), SEEK_CUR);
                        }
                }
                else
                {
                        (obj->addr)++;
                }
        }while(1);

        if(0 != obj->addr)
        {
                fprintf(stdout, "Find first sync byte at 0x%lX in %s.\n",
                        obj->addr, obj->file_i);
        }
}
#endif

//=============================================================================
// THE END.
//=============================================================================
