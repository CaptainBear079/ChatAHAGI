#include "../../../Entity.h"
#include "../../../plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CPLUGIN__NAME "dev-test"
#define CPLUGIN__MAJOR_V 0
#define CPLUGIN__MINOR_V 1
#define CPLUGIN__PATCH 0

#define EXIT_CODE__INTERFACE_NULL -1
#define EXIT_CODE__EXITFLAG_NULL -2
#define EXIT_CODE__ENTITY_NULL -3
#define EXIT_CODE__NEURONCOUNT_ZERO -4
#define EXIT_CODE__NEURONS_NULL -5

#define NEURAL_NETWORK ((NeuralNetwork*)((EM_Entity*)interface->entity)->data)
#define INPUT ((Input*)((EM_Entity*)interface->entity)->input)
#define INPUT_COUNT ((int)((EM_Entity*)interface->entity)->inputCount)
#define OUTPUT ((Output*)((EM_Entity*)interface->entity)->output)
#define OUTPUT_COUNT ((int)((EM_Entity*)interface->entity)->outputCount)

typedef struct _Input_ {
    double input; // Input current
    void* target; // Target neuron for the input
} Input;

typedef struct _Output_ {
    void* source;  // Source neuron
    double output; // Output current
} Output;

typedef struct _Neuron_ {
    double input;             // Input current
    void* output;             // Neuron for output
    double tau;               // Leak intensity
    double V;                 // Membrane potential
    double V_threshold;       // Membrane threshold
    double V_rest;            // Membrane resting potential
    double V_reset;           // Membrane reset potential
    clock_t now, then;        // Delta time variables
    double shutdownDeltaTime; // Delta time from before the shutdown
    double restTime;          // Reset time
    bool resetFlag;           // Reset flag
} Neuron;

typedef struct _NeuralNetwork_ {
    Neuron* neurons; // Array of neurons
    int neuronCount; // Number of neurons
} NeuralNetwork;
