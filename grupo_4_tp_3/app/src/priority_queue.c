#include "priority_queue.h"
#include "logger.h"

#define PRIORITY_QUEUE_ITEM_SIZE_ (sizeof(PriorityMessage))

static PriorityQueue priorityQueue;
static uint32_t currentTimestamp = 0;
static SemaphoreHandle_t priorityQueueMutex;

void PriorityQueue_Init() {
    priorityQueue.size = 0;
    // Crear el mutex para proteger la cola
	priorityQueueMutex = xSemaphoreCreateMutex();
	if (priorityQueueMutex == NULL) {
		// Manejo de error si no se puede crear el mutex
		configASSERT(priorityQueueMutex);
	}
}

bool PriorityQueue_Send(uint8_t priority) {
	if (xSemaphoreTake(priorityQueueMutex, portMAX_DELAY) != pdTRUE) {
		return false; // No se pudo tomar el mutex
	}

    if (priorityQueue.size >= MAX_PRIORITY_QUEUE_SIZE) {
    	xSemaphoreGive(priorityQueueMutex); // Liberar el mutex antes de salir
        return false;  // Cola llena
    }

    // Inserta el mensaje
    PriorityMessage newMessage = {
        .timestamp = currentTimestamp++,
        .priority = priority,
    };
    priorityQueue.messages[priorityQueue.size++] = newMessage;

    // Reorganiza la cola por prioridad y timestamp (criterios de desempate)
    for (uint8_t i = 0; i < priorityQueue.size - 1; i++) {
        for (uint8_t j = i + 1; j < priorityQueue.size; j++) {
            if ((priorityQueue.messages[i].priority < priorityQueue.messages[j].priority) ||
                (priorityQueue.messages[i].priority == priorityQueue.messages[j].priority &&
                 priorityQueue.messages[i].timestamp > priorityQueue.messages[j].timestamp)) {

            	// Intercambia los mensajes
                PriorityMessage temp = priorityQueue.messages[i];
                priorityQueue.messages[i] = priorityQueue.messages[j];
                priorityQueue.messages[j] = temp;
            }
        }
    }
    xSemaphoreGive(priorityQueueMutex); // Liberar el mutex al final
    return true;
}

bool PriorityQueue_Receive(EventPriority *prt) {
	if (xSemaphoreTake(priorityQueueMutex, portMAX_DELAY) != pdTRUE) {
		return false; // No se pudo tomar el mutex
	}

    if (priorityQueue.size == 0) {
    	xSemaphoreGive(priorityQueueMutex); // Liberar el mutex antes de salir
        return false;  // Cola vacía
    }
    EventPriority rtrn;
    // Devuelve el mensaje de mayor prioridad
    *prt = priorityQueue.messages[0].priority;

    // Desplaza los mensajes hacia adelante
    for (uint8_t i = 1; i < priorityQueue.size; i++) {
        priorityQueue.messages[i - 1] = priorityQueue.messages[i];
    }
    priorityQueue.size--;

    xSemaphoreGive(priorityQueueMutex); // Liberar el mutex al final
    return true;
}
