#ifndef ROOT_DMRLog
#define ROOT_DMRLog

R__EXTERN Int_t   gDMRDEBUGLEVEL;
R__EXTERN FILE   *gDMRLOGFILE;

extern void Log0(Int_t level, const char *location, const char *msgfmt, va_list va);
extern bool Log(Int_t level, const char *location, const char *msgfmt, ...);

#endif /* ROOT_DMRLog */
