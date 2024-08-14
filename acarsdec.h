/*
 *  Copyright (c) 2015 Thierry Leconte
 *
 *   
 *   This code is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2
 *   published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef acarsdec_h
#define acarsdec_h

#include <sys/time.h>
#include <time.h>
#include <complex.h>
#ifdef HAVE_LIBACARS
#include <libacars/libacars.h>
#include <libacars/reassembly.h>
#endif

#define ACARSDEC_VERSION "3.X"

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif

#define INTRATE 12500U	// 12.5kHz: ACARS is 2400Bd NRZI, AM 10kHz BW with a 1800Hz Fc, 1200Hz shift MSK.

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

typedef float sample_t;

typedef struct mskblk_s {
	struct mskblk_s *prev;
	int chn;
	struct timeval tv;
	int len;
	int err;
	float lvl;
	char txt[250];
	unsigned char crc[2];
} msgblk_t;

typedef struct {
	int chn;

	unsigned int Fr;		// channel frequency (in Hz)
	unsigned int counter;
	float complex *oscillator;
	float complex D;

	float *dm_buffer;		// INTRATE-sampled signal buffer
	double MskPhi;
	double MskDf;
	float MskClk;
	double MskLvlSum;
	int MskBitCount;
	unsigned int MskS, idx;
	float complex *inb;

	unsigned char outbits;
	int nbits;

	enum { WSYN, SYN2, SOH1, TXT, CRC1, CRC2, END } Acarsstate;
	msgblk_t *blk;
} channel_t;

typedef struct output_s {
	enum { FMT_ONELINE = 1, FMT_FULL, FMT_MONITOR, FMT_PP, FMT_NATIVE, FMT_JSON, FMT_ROUTEJSON } fmt;
	enum { DST_FILE = 1, DST_UDP, DST_MQTT } dst;
	void *params;
	void *priv;
	struct output_s *next;
} output_t;

typedef struct {
	channel_t *channels;
	unsigned int nbch;

	enum { IN_NONE = 0, IN_ALSA, IN_SNDFILE, IN_RTL, IN_AIR, IN_SDRPLAY, IN_SOAPY } inmode;
	int verbose;
	int airflt;
	int emptymsg;
	int mdly;

	float gain;
	int ppm;
	int bias;
	unsigned int rateMult;
	int lnaState;
	int GRdB;
	unsigned int freq;

	char *idstation;

#ifdef HAVE_LIBACARS
	int skip_reassembly;
#endif

#ifdef WITH_SOAPY
	char *antenna;
#endif

	output_t *outputs;
} runtime_t;

extern runtime_t R;

void demodMSK(channel_t *ch, int len);

#endif /* acarsdec_h */
