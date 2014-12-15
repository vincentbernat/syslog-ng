#include "debugger/tracer.h"

void
tracer_stop_on_breakpoint(Tracer *self)
{
  g_mutex_lock(self->breakpoint_mutex);

  /* send break point */
  self->breakpoint_hit = TRUE;
  g_cond_signal(self->breakpoint_cond);

  /* wait for resume */
  while (!self->resume_requested)
    g_cond_wait(self->resume_cond, self->breakpoint_mutex);
  self->resume_requested = FALSE;
  g_mutex_unlock(self->breakpoint_mutex);
}

void
tracer_wait_for_breakpoint(Tracer *self)
{
  g_mutex_lock(self->breakpoint_mutex);
  while (!self->breakpoint_hit)
    g_cond_wait(self->breakpoint_cond, self->breakpoint_mutex);
  self->breakpoint_hit = FALSE;
  g_mutex_unlock(self->breakpoint_mutex);
}

void
tracer_resume_after_breakpoint(Tracer *self)
{
  g_mutex_lock(self->breakpoint_mutex);
  self->resume_requested = TRUE;
  g_cond_signal(self->resume_cond);
  g_mutex_unlock(self->breakpoint_mutex);
}

Tracer *
tracer_new(GlobalConfig *cfg)
{
  Tracer *self = g_new0(Tracer, 1);

  self->breakpoint_mutex = g_mutex_new();
  self->breakpoint_cond = g_cond_new();
  self->resume_cond = g_cond_new();

  return self;
}
