#ifndef __OWA4X_IO_ADAP_H
	#define __OWA4X_IO_ADAP_H

	// System internal includes
	#include <stdlib.h>
	#include <signal.h>

	// OWA libraries to include
	#include "owa4x/owcomdefs.h"
	#include "owa4x/pm_messages.h"
	#include "owa4x/IOs_ModuleDefs.h"
	#include "owa4x/RTUControlDefs.h"

	// Constants defines

		// Library with the functions of IO's
	#define LIBRTU  "/lib/libRTU_Module.so"
	#define LIBIO	"/lib/libIOs_Module.so"

	#define SW_VERSION	"0.0.1"

	#ifdef __OWA4X_IO_ADAP_CPP
		static int get_dig_in(int pin_number);
		static int set_dig_out(int pin_number, int value);
		static int get_ana_in(int pin_number);
	#endif
#endif

