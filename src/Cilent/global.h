#pragma once

#include <Cilent/Cilent.h>

#ifndef CILENT_GLOBAL_FINAL
extern Cilent* cilent;
#else
Cilent* cilent = NULL;
#endif
