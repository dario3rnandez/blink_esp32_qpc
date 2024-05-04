#ifndef BLINKY_H
#define BLINKY_H

#include "qpc.h" /* QP/C public interface */

enum BlinkySignals {
    DUMMY_SIG = Q_USER_SIG,
    MAX_PUB_SIG,          /* the last published signal */

    TIMEOUT_SIG,
    MAX_SIG               /* the last signal */
};

void Blinky_ctor(void);
extern QActive * const AO_Blinky; /* opaque pointer */

#endif /* BLINKY_H */
