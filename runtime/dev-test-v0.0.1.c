#include "dev-test-v0.0.1.h"

int CPlugin_main(PluginInterface* interface) {
	// Security check
	if(interface == NULL) {
		fprintf(
			stderr,
			"[%s V%d.%d.%d][Inst: ?] FATAL ERROR: interface is NULL.\n    Exit code: %d\n",
			CPLUGIN__NAME, CPLUGIN__MAJOR_V, CPLUGIN__MINOR_V, CPLUGIN__PATCH, EXIT_CODE__INTERFACE_NULL
		);
		return EXIT_CODE__INTERFACE_NULL; // interface is NULL
	}
	if(interface->entity == NULL) {
		fprintf(
			stderr,
			"[%s V%d.%d.%d][Inst: ?] FATAL ERROR: interface->entity is NULL.\n    Exit code: %d\n",
			CPLUGIN__NAME, CPLUGIN__MAJOR_V, CPLUGIN__MINOR_V, CPLUGIN__PATCH, EXIT_CODE__ENTITY_NULL
		);
		return EXIT_CODE__ENTITY_NULL; // interface->entity is NULL
	}
	if(interface->exitFlag == NULL) {
		fprintf(
			stderr,
			"[%s V%d.%d.%d][Inst: %s] FATAL ERROR: interface->exitFlag is NULL.\n    Exit code: %d\n",
			CPLUGIN__NAME, CPLUGIN__MAJOR_V, CPLUGIN__MINOR_V, CPLUGIN__PATCH, ((EM_Entity*)interface->entity)->name, EXIT_CODE__EXITFLAG_NULL
		);
		return EXIT_CODE__EXITFLAG_NULL; // interface->exitFlag is NULL
	}

	if(NEURAL_NETWORK->neuronCount < 1) {
		fprintf(
			stderr,
			"[%s V%d.%d.%d][Inst: %s] FATAL ERROR: Model doesn't have neurons.\n    Exit code: %d\n",
			CPLUGIN__NAME, CPLUGIN__MAJOR_V, CPLUGIN__MINOR_V, CPLUGIN__PATCH, ((EM_Entity*)interface->entity)->name, EXIT_CODE__NEURONCOUNT_ZERO
		);
		return EXIT_CODE__NEURONCOUNT_ZERO; // NEURAL_NETWORK->neuronCount is zero
	}
	if(NEURAL_NETWORK->neurons == NULL) {
		fprintf(
			stderr,
			"[%s V%d.%d.%d][Inst: %s] FATAL ERROR: NEURAL_NETWORK->neurons is NULL.\n    Exit code: %d\n",
			CPLUGIN__NAME, CPLUGIN__MAJOR_V, CPLUGIN__MINOR_V, CPLUGIN__PATCH, ((EM_Entity*)interface->entity)->name, EXIT_CODE__NEURONS_NULL
		);
		return EXIT_CODE__NEURONS_NULL; // NEURAL_NETWORK->neurons is NULL
	}

	// Main loop
	// - Variables
	double deltaTime = 0.0;

	// - deltaTime setup
	NEURAL_NETWORK->neurons[0].then = clock();
	for(int n = 1; n < NEURAL_NETWORK->neuronCount; n++) {
		NEURAL_NETWORK->neurons[n] = NEURAL_NETWORK->neurons[0];
	}

	// Process neurons
	while(!(*(interface->exitFlag))) {
		// - Fetch input from the Entity
		for(int i = 0; i < INPUT_COUNT; i++) {
			((Neuron*)INPUT[i].target)->input += INPUT[i].input;
			INPUT[i].input = 0.0;
		}

		// - Update neuron
		for(int n = 0; n < NEURAL_NETWORK->neuronCount; n++) {
			// Get deltaTime
			NEURAL_NETWORK->neurons[n].now = clock();
			deltaTime = (double)(NEURAL_NETWORK->neurons[n].now - NEURAL_NETWORK->neurons[n].then) / (double)CLOCKS_PER_SEC;

			// Update .resetFlag
			if(deltaTime >= NEURAL_NETWORK->neurons[n].restTime) {
				NEURAL_NETWORK->neurons[n].resetFlag = false;
			}

			// Check if the neuron is resting
			if(NEURAL_NETWORK->neurons[n].resetFlag) {
				continue;
			}
			else {
				// Calculate membrane potrntial
				NEURAL_NETWORK->neurons[n].V += (deltaTime / NEURAL_NETWORK->neurons[n].tau) * (-(NEURAL_NETWORK->neurons[n].V - NEURAL_NETWORK->neurons[n].V_rest) + NEURAL_NETWORK->neurons[n].input);
				NEURAL_NETWORK->neurons[n].input = 0.0;

				if(NEURAL_NETWORK->neurons[n].V > NEURAL_NETWORK->neurons[n].V_threshold) {
					NEURAL_NETWORK->neurons[n].V = NEURAL_NETWORK->neurons[n].V_reset;
					double temp = NEURAL_NETWORK->neurons[n].V / NEURAL_NETWORK->neurons[n].outputCount;
					for(int o = 0; o <= (int)NEURAL_NETWORK->neurons[n].outputCount; o++) {
						((Neuron*)((Neuron**)NEURAL_NETWORK->neurons[n].output)[o])->input += (temp / NEURAL_NETWORK->neurons[n].synapticStrength[o]);
					}
				}
				NEURAL_NETWORK->neurons[n].then = NEURAL_NETWORK->neurons[n].now;
			}
		}

		// - Update output INDEV
		for(int o = 0; o < OUTPUT_COUNT; o++) {
			OUTPUT[o].output += ((Neuron*)OUTPUT[o].source)->V;
		}
	}

	// Return normally
	return 0;
}
