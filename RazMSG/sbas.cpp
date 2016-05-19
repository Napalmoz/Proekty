#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#include "sbas.h"


/* decode type 2-5,0: fast corrections ---------------------------------------*/
//static int decode_sbstype2(const sbsmsg_t *msg, sbssat_t *sbssat)
//{
//    int i,j,iodf,type,udre;
//    double prc,dt;
//    gtime_t t0;

//    trace(4,"decode_sbstype2:\n");

//    if (sbssat->iodp!=(int)getbitu(msg->msg,16,2)) return 0;

//    type=getbitu(msg->msg, 8,6);
//    iodf=getbitu(msg->msg,14,2);

//    for (i=0;i<13;i++) {
//        if ((j=13*((type==0?2:type)-2)+i)>=sbssat->nsat) break;
//        udre=getbitu(msg->msg,174+4*i,4);
//        t0 =sbssat->sat[j].fcorr.t0;
//        prc=sbssat->sat[j].fcorr.prc;
//        sbssat->sat[j].fcorr.t0=gpst2time(msg->week,msg->tow);
//        sbssat->sat[j].fcorr.prc=getbits(msg->msg,18+i*12,12)*0.125f;
//        sbssat->sat[j].fcorr.udre=udre+1;
//        dt=timediff(sbssat->sat[j].fcorr.t0,t0);
//        if (t0.time==0||dt<=0.0||18.0<dt||sbssat->sat[j].fcorr.ai==0) {
//            sbssat->sat[j].fcorr.rrc=0.0;
//            sbssat->sat[j].fcorr.dt=0.0;
//        }
//        else {
//            sbssat->sat[j].fcorr.rrc=(sbssat->sat[j].fcorr.prc-prc)/dt;
//            sbssat->sat[j].fcorr.dt=dt;
//        }
//        sbssat->sat[j].fcorr.iodf=iodf;
//    }
//    trace(5,"decode_sbstype2: type=%d iodf=%d\n",type,iodf);
//    return 1;
//}



extern int sbsupdatecorr(const sbsmsg_t *msg, nav_t *nav)
{
    //int type=getbitu(msg->msg,8,6),stat=-1;

    //trace(3,"sbsupdatecorr: type=%d\n",type);

    if (msg->week==0) return -1;

//    switch (type) {

//        case  0: stat=decode_sbstype2 (msg,&nav->sbssat); break;
//        case  1: stat=decode_sbstype1 (msg,&nav->sbssat); break;
//        case  2:
//        case  3:
//        case  4:
//        case  5: stat=decode_sbstype2 (msg,&nav->sbssat); break;
//        case  6: stat=decode_sbstype6 (msg,&nav->sbssat); break;
//        case  7: stat=decode_sbstype7 (msg,&nav->sbssat); break;
//        case  9: stat=decode_sbstype9 (msg,nav);          break;
//        case 18: stat=decode_sbstype18(msg,nav ->sbsion); break;
//        case 24: stat=decode_sbstype24(msg,&nav->sbssat); break;
//        case 25: stat=decode_sbstype25(msg,&nav->sbssat); break;
//        case 26: stat=decode_sbstype26(msg,nav ->sbsion); break;
//        case 63: break; /* null message */

        /*default: trace(2,"unsupported sbas message: type=%d\n",type); break;*/
//    }
    //return stat?type:-1;
    //return type;
}

/* decode sbas message ---------------------------------------------------------
* decode sbas message frame words and check crc
* args   : gtime_t time     I   reception time
*          int    prn       I   sbas satellite prn number
*          unsigned int *word I message frame words (24bit x 10)
*          sbsmsg_t *sbsmsg O   sbas message
* return : status (1:ok,0:crc error)
*-----------------------------------------------------------------------------*/
extern int sbsdecodemsg(gtime_t time, int prn, const unsigned int *words,
                        sbsmsg_t *sbsmsg)
{
    int i,j;
    unsigned char f[29];
    double tow;

    //trace(5,"sbsdecodemsg: prn=%d\n",prn);

    if (time.time==0) return 0;
//    tow=time2gpst(time,&sbsmsg->week);
//    sbsmsg->tow=(int)(tow+DTTOL);
//    sbsmsg->prn=prn;
//    for (i=0;i<7;i++) for (j=0;j<4;j++) {
//        sbsmsg->msg[i*4+j]=(unsigned char)(words[i]>>((3-j)*8));
//    }
//    sbsmsg->msg[28]=(unsigned char)(words[7]>>18)&0xC0;
//    for (i=28;i>0;i--) f[i]=(sbsmsg->msg[i]>>6)+(sbsmsg->msg[i-1]<<2);
//    f[0]=sbsmsg->msg[0]>>6;

//    return crc24q(f,29)==(words[7]&0xFFFFFF); /* check crc */
}

/* decode sbas message ---------------------------------------------------------
* decode sbas message frame words and check crc
* args   : gtime_t time     I   reception time
*          int    prn       I   sbas satellite prn number
*          unsigned int *word I message frame words (24bit x 10)
*          sbsmsg_t *sbsmsg O   sbas message
* return : status (1:ok,0:crc error)
*-----------------------------------------------------------------------------*/
extern int sbsdecodemsg(gtime_t time, int prn, void *words,
                        sbsmsg_t *sbsmsg)
{
    int i,j;
    unsigned char f[29];
    double tow;

    //trace(5,"sbsdecodemsg: prn=%d\n",prn);

    if (time.time==0) return 0;
//    tow=time2gpst(time,&sbsmsg->week);
//    sbsmsg->tow=(int)(tow+DTTOL);
//    sbsmsg->prn=prn;
//    for (i=0;i<7;i++) for (j=0;j<4;j++) {
//        sbsmsg->msg[i*4+j]=(unsigned char)(words[i]>>((3-j)*8));
//    }
//    sbsmsg->msg[28]=(unsigned char)(words[7]>>18)&0xC0;
//    for (i=28;i>0;i--) f[i]=(sbsmsg->msg[i]>>6)+(sbsmsg->msg[i-1]<<2);
//    f[0]=sbsmsg->msg[0]>>6;

//    return crc24q(f,29)==(words[7]&0xFFFFFF); /* check crc */
}

