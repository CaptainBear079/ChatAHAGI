#pragma once

#include <stdlib.h>
#include <pthread.h>

typedef struct _EM_Entity_ {
	pthread_t thread;  // Thread
	char* name;        // Entity name
	unsigned int id;   // Unique ID for the Entity
	unsigned int type; // Entity AI type (C plugin/Entity Engine)
	char* path;        // The path to the C Plugin/Entity Engine script
	void* input;       // Input data (dynamically defined at runtime)
	int inputCount;    // Amount of inputs
	void* output;      // Output data (dynamically defined at runtime)
	int outputCount;   // Amount of outputs
	void* data;        // Save data
	int* retCode;      // Return code pointer
} EM_Entity;
