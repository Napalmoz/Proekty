#ifndef SBAS
#define SBAS

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>


typedef struct {        /* time struct */
    time_t time;        /* time (s) expressed by standard time_t */
    double sec;         /* fraction of second under 1 s */
} gtime_t;

typedef struct {        /* observation data record */
    gtime_t time;       /* receiver sampling time (GPST) */
    unsigned char sat,rcv; /* satellite/receiver number */
//    unsigned char SNR [NFREQ+NEXOBS]; /* signal strength (0.25 dBHz) */
//    unsigned char LLI [NFREQ+NEXOBS]; /* loss of lock indicator */
//    unsigned char code[NFREQ+NEXOBS]; /* code indicator (CODE_???) */
//    double L[NFREQ+NEXOBS]; /* observation data carrier-phase (cycle) */
//    double P[NFREQ+NEXOBS]; /* observation data pseudorange (m) */
//    float  D[NFREQ+NEXOBS]; /* observation data doppler frequency (Hz) */
} obsd_t;

typedef struct {        /* SBAS message type */
    int week,tow;       /* receiption time */
    int prn;            /* SBAS satellite PRN number */
    unsigned char msg[29]; /* SBAS message (226bit) padded by 0 */
} sbsmsg_t;

typedef struct {        /* SBAS messages type */
    int n,nmax;         /* number of SBAS messages/allocated */
    sbsmsg_t *msgs;     /* SBAS messages */
} sbs_t;

typedef struct {        /* SBAS satellite corrections type */
    int iodp;           /* IODP (issue of date mask) */
    int nsat;           /* number of satellites */
    int tlat;           /* system latency (s) */
//    sbssatp_t sat[MAXSAT]; /* satellite correction */
} sbssat_t;

typedef struct {        /* navigation data type */
    int n,nmax;         /* number of broadcast ephemeris */
    int ng,ngmax;       /* number of glonass ephemeris */
    int ns,nsmax;       /* number of sbas ephemeris */
    int ne,nemax;       /* number of precise ephemeris */
    int nc,ncmax;       /* number of precise clock */
    int na,namax;       /* number of almanac data */
    int nt,ntmax;       /* number of tec grid data */
    int nn,nnmax;       /* number of stec grid data */
    double utc_gps[4];  /* GPS delta-UTC parameters {A0,A1,T,W} */
    double utc_glo[4];  /* GLONASS UTC GPS time parameters */
    double utc_gal[4];  /* Galileo UTC GPS time parameters */
    double utc_qzs[4];  /* QZS UTC GPS time parameters */
    double utc_cmp[4];  /* BeiDou UTC parameters */
    double utc_sbs[4];  /* SBAS UTC parameters */
    double ion_gps[8];  /* GPS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
    double ion_gal[4];  /* Galileo iono model parameters {ai0,ai1,ai2,0} */
    double ion_qzs[8];  /* QZSS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
    double ion_cmp[8];  /* BeiDou iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
    int leaps;          /* leap seconds (s) */
    double glo_cpbias[4];    /* glonass code-phase bias {1C,1P,2C,2P} (m) */
    sbssat_t sbssat;    /* SBAS satellite corrections */
} nav_t;

typedef struct {        /* SBAS ephemeris type */
    int sat;            /* satellite number */
    gtime_t t0;         /* reference epoch time (GPST) */
    gtime_t tof;        /* time of message frame (GPST) */
    int sva;            /* SV accuracy (URA index) */
    int svh;            /* SV health (0:ok) */
    double pos[3];      /* satellite position (m) (ecef) */
    double vel[3];      /* satellite velocity (m/s) (ecef) */
    double acc[3];      /* satellite acceleration (m/s^2) (ecef) */
    double af0,af1;     /* satellite clock-offset/drift (s,s/s) */
} seph_t;

extern unsigned int getbitu(const unsigned char *buff, int pos, int len);
extern void trace    (int level, const char *format, ...);
extern int sbsdecodemsg(gtime_t time, int prn, const unsigned int *words, sbsmsg_t *sbsmsg);
extern int sbsdecodemsg(gtime_t time, int prn, void *words, sbsmsg_t *sbsmsg);

#endif // SBAS

