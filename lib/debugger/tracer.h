#ifndef DEBUGGER_TRACER_H_INCLUDED
#define DEBUGGER_TRACER_H_INCLUDED 1

#include "syslog-ng.h"

typedef struct _Tracer
{
  GMutex *breakpoint_mutex;
  GCond *breakpoint_cond;
  GCond *resume_cond;
  gboolean breakpoint_hit;
  gboolean resume_requested;
} Tracer;

void tracer_stop_on_breakpoint(Tracer *self);
void tracer_wait_for_breakpoint(Tracer *self);
void tracer_resume_after_breakpoint(Tracer *self);

Tracer *tracer_new(GlobalConfig *cfg);

#endif
