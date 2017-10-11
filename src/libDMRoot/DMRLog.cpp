#include "snprintf.h"
#include "Varargs.h"
#include "TVirtualMutex.h"
#include "TSystem.h"
#include "TDatime.h"
#include "DMRLog.h"

#if !defined(R__VA_COPY)
#  if defined(__GNUC__) && !defined(__FreeBSD__)
#     define R__VA_COPY(to, from) __va_copy((to), (from))
#  elif defined(__va_copy)
#     define R__VA_COPY(to, from) __va_copy((to), (from))
#  elif defined(va_copy)
#     define R__VA_COPY(to, from) va_copy((to), (from))
#  elif defined (R__VA_COPY_AS_ARRAY)
#     define R__VA_COPY(to, from) memmove((to), (from), sizeof(va_list))
#  elif defined(_WIN32) && _MSC_VER < 1310
#     define R__VA_COPY(to, from) (*(to) = *(from))
#  else
#     define R__VA_COPY(to, from) ((to) = (from))
#  endif
#endif

Int_t    gDMRDEBUGLEVEL = 2;
FILE    *gDMRLOGFILE=0;

//______________________________________________________________________________
bool Log(int level, const char *location, const char *fmt, ...)
{
// Print message to the logfile and to stdout.
  if(level>gDMRDEBUGLEVEL) return 0;
  va_list ap;
  va_start(ap,va_(fmt));
  Log0(level, location, va_(fmt), ap);
  va_end(ap);
  return 1;
}

//______________________________________________________________________________
void Log0(int level, const char *location, const char *fmt, va_list ap)
{
// Print message to the logfile and to stdout.

  //R__LOCKGUARD2(gErrorMutex);

  static Int_t buf_size = 2048;
  static char *buf = 0;

  va_list sap;
  R__VA_COPY(sap, ap);

 again:
  if (!buf)
    buf = new char[buf_size];

  if (!fmt)
    fmt = "no error message provided";
  
  Int_t n = vsnprintf(buf, buf_size, fmt, ap);
  // old vsnprintf's return -1 if string is truncated new ones return
  // total number of characters that would have been written
  if (n == -1 || n >= buf_size) {
    if (n == -1)
      buf_size *= 2;
    else
      buf_size = n+1;
    delete [] buf;
    buf = 0;
    va_end(ap);
    R__VA_COPY(ap, sap);
    goto again;
  }
  va_end(sap);
  
  if(level<=gDMRDEBUGLEVEL)   {
	  fprintf(stdout, "%-16s: ", location);
	  fprintf(stdout, "%s\n", buf);
  }
  if(level>2)       return;     // do not print to file not important mesages
  TDatime t;
  if(gDMRLOGFILE) {
    fprintf(gDMRLOGFILE, "%s> ", t.AsSQLString());
    fprintf(gDMRLOGFILE, "%-16s: ", location);
    fprintf(gDMRLOGFILE, "%s\n", buf);
  }
}

/*
//______________________________________________________________________________
void ErrorHandler(Int_t level, const char *location, const char *fmt, va_list ap)
{
   // General error handler function. It calls the user set error handler.

   R__LOCKGUARD2(gErrorMutex);

   static Int_t buf_size = 2048;
   static char *buf = 0;

   va_list sap;
   R__VA_COPY(sap, ap);

again:
   if (!buf)
      buf = new char[buf_size];

   if (!fmt)
      fmt = "no error message provided";

   Int_t n = vsnprintf(buf, buf_size, fmt, ap);
   // old vsnprintf's return -1 if string is truncated new ones return
   // total number of characters that would have been written
   if (n == -1 || n >= buf_size) {
      if (n == -1)
         buf_size *= 2;
      else
         buf_size = n+1;
      delete [] buf;
      buf = 0;
      va_end(ap);
      R__VA_COPY(ap, sap);
      goto again;
   }
   va_end(sap);


   char *bp;
   if (level >= kSysError && level < kFatal)
      bp = Form("%s (%s)", buf, gSystem->GetError());
   else
      bp = buf;

   if (level != kFatal)
      gErrorHandler(level, level >= gErrorAbortLevel, location, bp);
   else
      gErrorHandler(level, kTRUE, location, bp);
}
*/
