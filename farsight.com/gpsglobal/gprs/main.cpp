
#include "gprs.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

int main()
{

        const char   * devfile;
        devfile = getenv("GG_GPRSPORT");
        if(devfile == NULL)
        {
                devfile = "/dev/ttyS0";
        }

        std::cerr << devfile << std::endl;

        CGPRS   gprs(devfile); 

        return 0; 

}
