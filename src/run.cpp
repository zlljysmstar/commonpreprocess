// author: Zhu, Liang. email: zhuliang@360.cn build-date: 2014-05-06

#include <iostream>
#include "common_core.h"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr<<"[ERROR.00].THE PROGRAM MUST HAVE ONE PARAMETER! PLEASE CHECK!"<<std::endl;
        return -1;
    }
    
    CProcessUnify process_unify;
	if(!process_unify.run(argv[1]))
    {
        std::cerr<<"[ERROR.01].CONDITIONS OF RUN-TIME NOT SATISFIED! PLEASE CHECK!"<<std::endl;
        return -1;
    }

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 number: */
