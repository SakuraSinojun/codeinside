

#pragma once

#ifndef __LOCATION_H__
#define __LOCATION_H__


typedef struct GPSINFO 
{
        char    utc_time[64];
        char    status;
        float   latitude_value;
        char    latitude;
        float   longtitude_value;
        char    longtitude;
        float   speed;
        float   azimuth_angle;
        char    utc_data[64];

}GPSINFO;

class CGPS
{
public:
        CGPS(const char * com, int bps, int bits, int stopbits, int parity);
        ~CGPS();

        GPSINFO& GetLocation();

private:
        GPSINFO         gpsinfo;
        int             fd;

};

#endif

