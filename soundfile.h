#ifndef soundfile_h
#define soundfile_h

int initSoundfile(char **argv, int optind);
int runSoundfileSample(void);

#ifdef DEBUG
void initSndWrite(void);
void SndWrite(float *in);
void SndWriteClose(void);
#endif

#endif /* soundfile_h */