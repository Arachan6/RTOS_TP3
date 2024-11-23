#ifndef INC_PRIORITY_QUEUE_H_
#define INC_PRIORITY_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>
#include "ao_ui.h"

#define MAX_PRIORITY_QUEUE_SIZE 10

typedef struct {
    uint32_t timestamp;
    EventPriority priority;
} PriorityMessage;

typedef struct {
    PriorityMessage messages[MAX_PRIORITY_QUEUE_SIZE];
    uint8_t size;
} PriorityQueue;

void PriorityQueue_Init();
bool PriorityQueue_Send(uint8_t priority);
bool PriorityQueue_Receive(EventPriority *priority);

#endif /* INC_PRIORITY_QUEUE_H_ */
