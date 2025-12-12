#ifndef STATUS_H
#define STATUS_H

//=====================================
//  STATUS TYPE
//=====================================
typedef enum {
    BURN,
    POISON,

    STUN,
    FREEZE,
} StatusType;

//=====================================
//  STATUS MAIN
//=====================================
typedef struct {
    StatusType type;
    float baseAmount;
    int duration;
} Status;

#endif