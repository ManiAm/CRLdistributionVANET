CRL Distribution in VANETs
==========================

This repos contains the implemetation of different CRL distribution methods in VANETs based on my master thesis:

M.S. Thesis: Certificate Revocation List Distribution in Vehicular Communication Systems
Under supervision of professor Panos Papadimitratos
More information here: http://goo.gl/zinfj

Open this project in OMNET++
It needs veins framework




###############################
Step 1: Download pre-requisites
###############################

You need Veins framework to run this project.
Download it from here: http://veins.car2x.org/

You also need eigen. download it and extract it on the desktop (it's a header only library)

Then you need to install the boost:
libboost1.46-dev
libboost-serialization1.46-dev

Then install openSSL
libssl-dev



#########################################
Step 2: you should change the mixim code
#########################################

1. mixim/src/base/modules/BatteryAccess.cc

- commenting line 26:  // opp_warning("No battery module defined!");


2. mixim/src/base/connectionManager/BaseConnectionManager.cc

- adding these lines after line 275

        	// TODO: MANI: *********************************************

        	cModule *mod1 = nic->nicPtr->getParentModule();
        	cModule *mod2 = nic_i->nicPtr->getParentModule();

            // now send connect signal for statistics
            simsignal_t Signal_In_Range = registerSignal("In_Range");
        	mod1->emit(Signal_In_Range, mod2->getFullName());
        	mod2 ->emit(Signal_In_Range, mod1->getFullName());

    	    // ***************************************************


3. mixim/src/modules/mac/Mac80211.cc

- add after line 28 in header:

// TODO: MANI
#include "FindModule.h"

- add after line 275:

        // TODO: MANI: **************************************
        {
        // most of the time, the reason is because of the BITERROR and not COLLISION !!
        cModule *nodePtr = FindModule<>::findHost(this);

        // send signal to application layer
        simsignal_t Signal_fromMAC = registerSignal("fromMAC");
        nodePtr->emit(Signal_fromMAC, "RxError");

        // send signal for statistics
        simsignal_t Signal_RxErrors = registerSignal("RxErrors");
        nodePtr->emit(Signal_RxErrors, 1);
        }
        // ***************************************************
	    
- add after line 305:

        // TODO: MANI: ***************************************
        {
        // send signal to application layer
        simsignal_t Signal_fromMAC = registerSignal("fromMAC");
        cModule *nodePtr = FindModule<>::findHost(this);
        nodePtr->emit(Signal_fromMAC, "TxOver");
        }
        // ***************************************************
    	    

4. mixim/src/modules/mobility/BonnMotionMobility.cc

- change line 103 from handleIfOutside(RAISEERROR, stepTarget, dummy, dummy, dum);
to handleIfOutside(REFLECT, stepTarget, dummy, dummy, dum);


5. mixim/src/modules/mobility/traci/TraCIScenarioManager.cc

- add after line 665

	// TODO: MANI: ***************************************

	// save the name of the module, for later use.
	// We must copy the name and should not use pointer!
	char name[10];
	strcpy(name, mod->getFullName());

	// ***************************************************

- add afte line 680

	// TODO: MANI: ***************************************

    // send signal for statistics
	simsignal_t Signal_Module_Deleted = registerSignal("V_Deleted");
	simulation.getSystemModule()->emit(Signal_Module_Deleted, name);

    // ***************************************************
    
    
    
    
##########################################################
Step 3: change mixim to make it compatible with older gcc
(running on KTH server)
##########################################################

according to this link:
https://github.com/omnetpp/mixim/commit/ec194baa813429b17626b90e2a6e80e3f47f0d07    
    
    
1. src/base/connectionManager/ChannelAccess.h
    
typedef AccessModuleWrap<BaseMobility>       ChannelMobilityAccessType;
typedef ChannelMobilityAccessType::wrapType* ChannelMobilityPtrType;



2. src/base/phyLayer/MappingUtils.h

- Line 681:

typedef base_class_type::storage_type     storage_type;
typedef base_class_type::container_type   container_type;
typedef base_class_type::key_type         key_type;
typedef base_class_type::key_cref_type    key_cref_type;
typedef base_class_type::mapped_type      mapped_type;
typedef base_class_type::mapped_cref_type mapped_cref_type;
typedef base_class_type::pair_type        pair_type;
typedef base_class_type::iterator         iterator;
typedef base_class_type::const_iterator   const_iterator;
typedef base_class_type::comparator_type  comparator_type;
typedef base_class_type::interpolated     interpolated;

- Line 1655:

argument_value_cref_t           argVal = pos.getArgValue(myDimension);
interpolator_map_type::iterator posIt  = entries.lower_bound(argVal);

3. Change the Eigen library path in the makefile of application folder
                
4. Change the sumo path in suo-launchd.xml


##########################################################################
Step 4: issue the following command to run the simulation in command-line
##########################################################################

run the script.sh which has screen support.


