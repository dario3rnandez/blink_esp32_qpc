
#include "qpc.h"
#include "blinky.h"
#include "bsp.h"


/*..........................................................................*/
typedef struct {     /* the Blinky active object */
    QActive super;   /* inherit QActive */

    QTimeEvt timeEvt; /* private time event generator */
} Blinky;

static Blinky l_blinky; /* the Blinky active object */

QActive * const AO_Blinky = &l_blinky.super;

/* hierarchical state machine ... */
static QState Blinky_initial(Blinky * const me, QEvt const * const e);
static QState Blinky_off    (Blinky * const me, QEvt const * const e);
static QState Blinky_on     (Blinky * const me, QEvt const * const e);

/*..........................................................................*/
void Blinky_ctor(void) {
    Blinky * const me = &l_blinky;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

/* HSM definition ----------------------------------------------------------*/
QState Blinky_initial(Blinky * const me, QEvt const * const e) {
    (void)e; /* avoid compiler warning about unused parameter */

    /* arm the time event to expire in half a second and every half second */
    QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC/2U, BSP_TICKS_PER_SEC/2U);
    return Q_TRAN(&Blinky_off);
}
/*..........................................................................*/
QState Blinky_off(Blinky * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_ledOff();
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            status = Q_TRAN(&Blinky_on);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Blinky_on(Blinky * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_ledOn();
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            status = Q_TRAN(&Blinky_off);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

