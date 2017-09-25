/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2017.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 4-1 */

/* copy.c

   Copy the file named argv[1] to a new file named in argv[2].
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif


int main(int argc, char *argv[])
{
    int inputFd, outputFd;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    long offset1;
    long offset2;

    // used for testing purposes
    //printf("argc = %d\n", argc);
    //printf(" argv[0] = %s\n argv[1] = %s\n argv[2] = %s\n argv[3] = %s\n argv[4] = %s\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
    
    //used for testing purposes
    // .evan_gallagher_hw1 offset1 fileSource offset2 destinationfile
    //  argv[0]             argv[1] argv[2]   argv[3]   argv[4]



    if ((argc < 3) | (argc > 5) || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);

        

    /* Open input and output files */

    if( access( argv[1], F_OK ) != -1 ) 
    {
        inputFd = open(argv[1], O_RDONLY);
        offset1 = 0; //set to default
        if (lseek(inputFd, offset1, SEEK_SET) == -1)
            errExit("lseek1");
    } 
    else 
    {
        offset1 = getLong(argv[1], GN_ANY_BASE, "OFFset1 Problem");
        inputFd = open(argv[2], O_RDONLY);
        if (lseek(inputFd, offset1, SEEK_SET) == -1)
            errExit("lseek1");
    }

    
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
  

    if( access( argv[2], F_OK ) != -1 &&((argv[3] == NULL) |(argv[4] == NULL)) )
    {   
        offset2 = 0; //Default
        outputFd = open(argv[2], O_WRONLY, filePerms);
        if (lseek(outputFd, offset2, SEEK_SET) == -1)
            errExit("lseek2");

    }
    else if(access( argv[3], F_OK ) != -1)
    {
        if(access( argv[1], F_OK ) == 0)
            offset2 = getLong(argv[2], GN_ANY_BASE, "OFFset2 Problem");
        else 
            offset2 = 0;

        
            outputFd = open(argv[3], O_WRONLY, filePerms);
        if (lseek(outputFd, offset2, SEEK_SET) == -1)
            errExit("lseek2");
    }
    else
    {
        offset2 = getLong(argv[3], GN_ANY_BASE, "OFFset2 Problem");
        outputFd = open(argv[4], O_WRONLY, filePerms);
        if (lseek(outputFd, offset2, SEEK_SET) == -1)
            errExit("lseek2");
    }

    /* Transfer data until we encounter end of input or an error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead)
            fatal("couldn't write whole buffer");
    if (numRead == -1)
        errExit("read");

    if (close(inputFd) == -1)
        errExit("close input");
    if (close(outputFd) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}