#ifndef STATUS_H
#define STATUS_H

typedef enum {
    STATUS_NONE = 0,
    STATUS_DAMAGE_REDUCTION,
    STATUS_HP_REDUCTION,
} StatusType;

typedef struct {
    StatusType type;
    float value;
    int duration;
    int maxDuration;
} Status;

#endif