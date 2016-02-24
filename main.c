#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//const char * pCmd = "r240/p9/sh120HC/shM7-20MC/ih230-20MA-p5/ih230-MA-p0/sl240MA";
const char * pCmd = "r240/p9/sh120HC/shM7-20MC/ih230-30MA-p5/ih230MA-p0/sl240MA";


int main(void)
{
    char * pCmdBuf = (char * ) malloc(1024);

    if (pCmdBuf == NULL)
    {
        fprintf(stderr,"out of memory");
    }

    memcpy(pCmdBuf,pCmd,strlen(pCmd));

    fprintf(stderr,"解析命令\n");
    fprintf(stderr,"%s\n",pCmd);

    SplitCmd(pCmdBuf);

    free(pCmdBuf);
}
