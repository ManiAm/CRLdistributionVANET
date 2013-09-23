#include <Statistics.h>

Define_Module(Statistics);

// initialize private static variables
int Statistics::Var_CRLsize_CA = 0;
int Statistics::Var_Thresh = 0;

int Statistics::Var_NoRSUs = 0;
int Statistics::Var_NoBroadcast_RSU = 0;
double Statistics::Var_I2V_tho;

int Statistics::Var_NoV = 0;

int Statistics::Var_IN_RSU_RANGE = 0;
int Statistics::Var_IN_V_RANGE = 0;

int Statistics::Var_CRL_I2V = 0;
int Statistics::Var_CRL_V2V = 0;
int Statistics::Var_NoDupCRLs_RSU = 0;
int Statistics::Var_NoDupCRLs_V = 0;
double Statistics::Var_V2V_tho;

int Statistics::Var_Beacon_RSU = 0;
int Statistics::Var_Beacon_V_B = 0;
int Statistics::Var_Beacon_V_U = 0;

int Statistics::Var_NoMod_Deleted = 0;

std::vector<NodeEntry *> Statistics::Vec_NoCRLs_V;
std::vector<NodeEntry *> Statistics::Vec_RxErrors_V;
std::vector<NodeEntry *> Statistics::Vec_RxErrors_RSU;
std::vector<NodeEntry *> Statistics::Vec_GetCRL_RSU;
std::vector<NodeEntry *> Statistics::Vec_NewRSU_Encounter;
std::vector<NodeEntry *> Statistics::Vec_Received_Message;
std::vector<RxMsg *> Statistics::Vec_Sent_Message;


// constructor
Statistics::Statistics()
{

}


void Statistics::initialize(int stage)
{
    if(stage == 0)
	{
        ErasureCode =  simulation.getSystemModule()->par("ErasureCode").boolValue();
    	SumoMode =  simulation.getSystemModule()->par("SumoMode").boolValue();

    	// TraCIScenarioManager.cc emits this signal.
    	Signal_V_Deleted = registerSignal("V_Deleted");

    	// BaseConnectionManage.cc emits this signal.
    	Signal_In_Range = registerSignal("In_Range");

		// CA emits this signal to report the size of CRL.
    	Signal_CRLsize_CA = registerSignal("CRLsize_CA");

        // CA emits this signal to report the threshold for number of CRL pieces.
        Signal_Thresh = registerSignal("Thresh");

    	// RSUs emit these signals.
    	Signal_NoRSUs = registerSignal("NoRSUs");
        Signal_Beacon_RSU = registerSignal("Beacon_RSU");
        Signal_Broadcast_RSU = registerSignal("Broadcast_RSU");
        Signal_I2V_tho = registerSignal("I2V_tho");

    	// Vehicles emit these signals.
  		Signal_NoV = registerSignal("NoV");
  		Signal_NoCRLs_V = registerSignal("NoCRLs_V");
  		Signal_DupCRLs_V = registerSignal("DupCRLs_V");
    	Signal_Beacon_V_B = registerSignal("Beacon_V_B");
  		Signal_Beacon_V_U = registerSignal("Beacon_V_U");
  		Signal_CRL_V2V = registerSignal("CRL_V2V");
  		Signal_CRL_I2V = registerSignal("CRL_I2V");
  		Signal_GetCRL_RSU = registerSignal("GetCRL_RSU");
        Signal_V2V_tho = registerSignal("V2V_tho");
        Signal_Msg_received_V = registerSignal("Msg_received_V");
        Signal_Msg_sent_V = registerSignal("Msg_sent_V");

  		// RSUs or Vehicles emit this signal.
    	Signal_RxErrors = registerSignal("RxErrors");

        // Now subscribe locally to all these signals
  		simulation.getSystemModule()->subscribe(Signal_V_Deleted, this);
  		simulation.getSystemModule()->subscribe(Signal_In_Range, this);
    	simulation.getSystemModule()->subscribe(Signal_CRLsize_CA, this);
        simulation.getSystemModule()->subscribe(Signal_Thresh, this);
    	simulation.getSystemModule()->subscribe(Signal_NoRSUs, this);
    	simulation.getSystemModule()->subscribe(Signal_Beacon_RSU, this);
    	simulation.getSystemModule()->subscribe(Signal_Broadcast_RSU, this);
        simulation.getSystemModule()->subscribe(Signal_I2V_tho, this);
  		simulation.getSystemModule()->subscribe(Signal_NoV, this);
  		simulation.getSystemModule()->subscribe(Signal_NoCRLs_V, this);
  		simulation.getSystemModule()->subscribe(Signal_DupCRLs_V, this);
  		simulation.getSystemModule()->subscribe(Signal_Beacon_V_B, this);
  		simulation.getSystemModule()->subscribe(Signal_Beacon_V_U, this);
  		simulation.getSystemModule()->subscribe(Signal_CRL_V2V, this);
        simulation.getSystemModule()->subscribe(Signal_CRL_I2V, this);
        simulation.getSystemModule()->subscribe(Signal_GetCRL_RSU, this);
  		simulation.getSystemModule()->subscribe(Signal_V2V_tho, this);
        simulation.getSystemModule()->subscribe(Signal_Msg_received_V, this);
        simulation.getSystemModule()->subscribe(Signal_Msg_sent_V, this);

  		simulation.getSystemModule()->subscribe(Signal_RxErrors, this);

        if (ev.isGUI())
        {
  		    Timer1 = new cMessage( "Timer_Statistics_SnapShot", Timer_Statistics_SnapShot );
  		    scheduleAt(simTime() + 3, Timer1);
        }
    }
}


void Statistics::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        if( msg->getKind() == Timer_Statistics_SnapShot )
        {
            printToFile_SnapShots();
        }
    }
}


// is called frequently during simulation and
// takes snapshots from the PiecesCRLcollected of each vehicle
void Statistics::printToFile_SnapShots()
{
    // note that we removed the number of nodes in the file name
    // in Sumo-mode the vehicles gradually enter the network, thus
    // each time this method is called, a different file will be created!

    // choose file name
    char fileName [30];

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(_WIN64)
    sprintf( fileName, "results\\1.SnapShots.txt");
#else
    sprintf( fileName, "results//1.SnapShots.txt");
#endif

    // open the file in a+ mode
    FILE *pFile = fopen(fileName,"a+");

    if(pFile == NULL)
    {
        EV << "ERROR !!!" << endl;
    }

    fprintf (pFile, "SnapShot at t;  %f;\n", simTime().dbl());

    // iterate through all nodes in the network
    for(int i=0; i<Var_NoV; i++)
    {
        cModule *module = simulation.getSystemModule()->getSubmodule("V", i);
        // if we are in Sumo-mode, and a vehicle goes out of the network, when iterating
        // through vehicles, the module pointer of that vehicle would be null
        if(module == NULL)
            continue;

        cModule *applMod = module->getSubmodule("appl");
        ApplV *pointer = static_cast<ApplV *>(applMod);

        fprintf (pFile, "Pieces of vehicle %d;   ", i);

        // iterate through all pieces of a vehicle
        for(unsigned int j=0; j<pointer->PiecesCRLcollected.size();j++)
        {
            fprintf (pFile, "%d,", pointer->PiecesCRLcollected[j]->getSeqNo());
        }

        // write new line
        fputs(";\n", pFile);
    }

    // write new line
    fputs("end of section;", pFile);

    // write new line
    fputs("\n\n", pFile);

    // close the file
    fclose(pFile);

    scheduleAt(simTime() + 3, Timer1);
}


void Statistics::receiveSignal(cComponent *source, simsignal_t signalID, long i)
{
	Enter_Method_Silent();

	EV << "*** Statistics module received signal " << signalID;
    EV << " from module " << source->getFullName();
	EV << " with value " << i << endl;

	int nodeIndex = getNodeIndex(source ->getFullName());

	// the signal comes from CA
	if(signalID == Signal_CRLsize_CA)
	{
		Var_CRLsize_CA = i;
		EV << "*** Statistics -> Var_CRLsize_CA = " << Var_CRLsize_CA << endl;
	}
    // the signal comes from CA
	else if(signalID == Signal_Thresh)
	{
	    Var_Thresh = i;
        EV << "*** Statistics -> Var_Thresh = " << Var_Thresh << endl;
	}
	// the signal coming from RSU
	else if(signalID == Signal_NoRSUs)
	{
		Var_NoRSUs++;
		EV << "*** Statistics -> Var_NoRSUs = " << Var_NoRSUs << endl;
	}
	// the signal coming from RSU
	else if(signalID == Signal_Beacon_RSU)
	{
		Var_Beacon_RSU++;
		EV << "*** Statistics -> Var_Beacon_RSU = " << Var_Beacon_RSU << endl;
	}
	// the signal coming from RSU
	else if(signalID == Signal_Broadcast_RSU)
	{
		Var_NoBroadcast_RSU++;
		EV << "*** Statistics -> Var_NoBroadcast_RSU = " << Var_NoBroadcast_RSU << endl;
	}
	// signal coming from V
	else if(signalID == Signal_NoV)
	{
		Var_NoV++;
		EV << "*** Statistics -> Var_NoV = " << Var_NoV << endl;
	}
	// the vehicle signals the number of CRLs received so far (non-Sumo mode).
	else if((signalID == Signal_NoCRLs_V) && !SumoMode)
	{
		EV << "*** Statistics -> A new CRL piece is received by " << source->getFullName() << endl;

		// Var_Thresh, determines the minimum number of CRL pieces that
		// is used for CRL re-construction.

		// Case 1: when Erasure code is disable, Var_Thresh = NoSegments.
		// in this case, normal nodes and magic cars, reach Var_Thresh only once.

		// Case 2: when Erasure code is enable, Var_Thresh = M.
		// in this case, magic cars reaches Var_Thresh only once, but normal
		// vehicles may exceed Var_Thresh. So we should search Vec_NoCRLs_V to make
		// sure that we add it only once.

		if(i >= Var_Thresh)
		{
		    int counter = findInVector(Vec_NoCRLs_V, source->getFullName());

	        // its a new entry, so we add it.
	        if(counter == -1)
	        {
	            NodeEntry *tmp = new NodeEntry(source->getFullName(), nodeIndex, i, simTime(), -1);
	            Vec_NoCRLs_V.push_back(tmp);

	            EV << "*** Statistics -> All required CRL pieces are received by " << source->getFullName() << endl;

	            // check to see if all vehicles receive all the CRL pieces.
	            // we can safely stop the simulation.
	            if(Vec_NoCRLs_V.size() == (unsigned int)Var_NoV)
	            {
	                EV << endl;
	                EV << "*** Statistics -> All vehicles in the network received all the required CRL pieces and " << endl;
	                EV << "its now safe to end the simulation." << endl << endl;

	                TerminateSimulation();
	            }
	        }
		}
    }
	// the vehicle signals the number of CRLs received so far (Sumo-mode).
	else if((signalID == Signal_NoCRLs_V) && SumoMode)
	{
	    EV << "*** Statistics -> A new CRL is received by " << source->getFullName() << endl;

	    int counter = findInVector(Vec_NoCRLs_V, source->getFullName());

		// its a new entry, so we add it.
		if(counter == -1)
		{
			NodeEntry *tmp = new NodeEntry(source->getFullName(), nodeIndex, i, simTime(), -1);
			Vec_NoCRLs_V.push_back(tmp);
		}
		// if found, just update the existing fields (pointed by counter)
		else if(i <= Var_Thresh)
		{
			Vec_NoCRLs_V[counter]->count = i;   // number of pieces
			Vec_NoCRLs_V[counter]->time = simTime();
		}
	}
	// the vehicle signals that it received a duplicate CRL.
	else if(signalID == Signal_DupCRLs_V)
	{
		// duplicate CRL from RSU
		if(i == 0)
		{
			Var_NoDupCRLs_RSU++;
			EV << "*** Statistics -> Var_NoDupCRLs_RSU = " << Var_NoDupCRLs_RSU << endl;
		}
		// duplicate CRL from V
		else if(i == 1)
		{
			Var_NoDupCRLs_V++;
			EV << "*** Statistics -> Var_NoDupCRLs_V = " << Var_NoDupCRLs_V << endl;
		}
	}
	else if(signalID == Signal_Beacon_V_B)
	{
		Var_Beacon_V_B++;
		EV << "*** Statistics -> Var_Beacon_V_B = " << Var_Beacon_V_B << endl;
	}
	else if(signalID == Signal_Beacon_V_U)
	{
		Var_Beacon_V_U++;
		EV << "*** Statistics -> Var_Beacon_V_U = " << Var_Beacon_V_U << endl;
	}
	else if(signalID == Signal_CRL_V2V)
	{
		Var_CRL_V2V++;
		EV << "*** Statistics -> Var_CRL_V2V = " << Var_CRL_V2V << endl;
	}
	else if(signalID == Signal_CRL_I2V)
	{
		Var_CRL_I2V++;
		EV << "*** Statistics -> Var_CRL_I2V = " << Var_CRL_I2V << endl;
	}
    else if(signalID == Signal_GetCRL_RSU)
    {
        EV << "*** Statistics -> A new RSU is encountered by " << source->getFullName() << endl;

        int counter = findInVector(Vec_GetCRL_RSU, source->getFullName());

        // its a new entry, so we add it.
        if(counter == -1)
        {
            NodeEntry *tmp = new NodeEntry(source->getFullName(), nodeIndex, 1, -1, -1);
            Vec_GetCRL_RSU.push_back(tmp);
        }
        // else, just update the existing fields (pointed by counter)
        else
        {
            // increase the count value by one
            Vec_GetCRL_RSU[counter]->count = Vec_GetCRL_RSU[counter]->count + 1;
        }
    }
	else if(signalID == Signal_RxErrors)
	{
	    EV << "*** Statistics -> " << source->getFullName() << " received a frame in error." << endl;

		if(strcmp(source->getName(), "RSU") == 0)
		{
			NodeEntry *tmp = new NodeEntry( source->getFullName(), nodeIndex, -1, simTime(), -1 );
		    Vec_RxErrors_RSU.push_back(tmp);
		}
		else if(strcmp(source->getName(), "V") == 0)
		{
			NodeEntry *tmp = new NodeEntry( source->getFullName(), nodeIndex, -1, simTime(), -1 );
		    Vec_RxErrors_V.push_back(tmp);
		}
	}
	else if(signalID == Signal_Msg_received_V)
	{
        EV << "*** Statistics -> " << source->getFullName() << " received a false message." << endl;

        NodeEntry *tmp = new NodeEntry(source->getFullName(), nodeIndex, -1, simTime(), -1);
        Vec_Received_Message.push_back(tmp);
	}
}


void Statistics::receiveSignal(cComponent *source, simsignal_t signalID, double i)
{
    Enter_Method_Silent();

    EV << "*** Statistics module received signal " << signalID;
    EV << " from module " << source->getFullName();
    EV << " with value " << i << endl;

    if(signalID == Signal_I2V_tho)
    {
        Var_I2V_tho = i;
        EV << "*** Statistics -> Var_I2V_tho = " << Var_I2V_tho << endl;
    }
    else if(signalID == Signal_V2V_tho)
    {
        Var_V2V_tho = i;
        EV << "*** Statistics -> Var_V2V_tho = " << Var_V2V_tho << endl;
    }
}


// Signal_In_Range is sent from BaseConnectionManager (non Sumo-mode or Sumo-mode)
// Signal_V_Deleted is sent from TraCIScenarioManager (only in Sumo-mode)
void Statistics::receiveSignal(cComponent *source, simsignal_t signalID, const char *str)
{
	Enter_Method_Silent();

	EV << "*** Statistics module received signal " << signalID;
	EV << " from module " << source->getFullName();
	EV << " with value " << str << endl;

    if (signalID == Signal_In_Range)
    {
        EV << "*** Statistics -> " << source->getFullName() << " connected to " << str << endl;

        // RSU is sending this signal
        if(strcmp(source->getName(), "RSU") == 0 && strstr(str, "V") != NULL)
        {
            Var_IN_RSU_RANGE++;

            // update the NewRSU_Encounter vector
            int counter = findInVector(Vec_NewRSU_Encounter, str);
            int nodeIndex = getNodeIndex(str);

            // its a new entry, so we add it.
            if(counter == -1)
            {
                NodeEntry *tmp = new NodeEntry(str, nodeIndex, 1, -1, -1);
                Vec_NewRSU_Encounter.push_back(tmp);
            }
            else
            {
                // increase the count value by one
                Vec_NewRSU_Encounter[counter]->count = Vec_NewRSU_Encounter[counter]->count + 1;
            }
        }
        // V is sending this signal
        else if(strcmp(source->getName(), "V") == 0 && strstr(str, "V") != NULL)
        {
            Var_IN_V_RANGE++;
        }
    }
    else if(SumoMode && signalID == Signal_V_Deleted)
	{
		EV << "Statistics -> Module " << str << " is deleted." << endl;

	    Var_NoMod_Deleted++;

	    // node str has been deleted, and we should update Vec_NoCRLs_V.
	    // we search in the Vector to find str
	    int counter = findInVector(Vec_NoCRLs_V, str);

	    // most of the time, we will find str, and
	    // we just update the ExitTime
	    if(counter != -1)
	    {
	        Vec_NoCRLs_V[counter]->ExitTime = simTime();
	    }
	    // if str is not found, it means that the vehicle has not received any CRLs.
	    // We should add it to the vector now.
	    else
	    {
	        int nodeIndex = getNodeIndex(str);
	        NodeEntry *tmp = new NodeEntry(str, nodeIndex, 0, 0, simTime());
	        Vec_NoCRLs_V.push_back(tmp);
	    }

	    // we also should make sure that vehicle str is
	    // in the Vec_NewRSU_Encounter and Vec_GetCRL_RSU
	    Update_Vectors(str);

		// check if all V move out of the network and it is safe to end the simulation.
		if(Var_NoMod_Deleted == Var_NoV)
		{
			EV << endl;
			EV << "*** Statistics -> All vehicles in the network moved out of network boundary and " << endl;
		    EV << "its now safe to end the simulation." << endl << endl;

		    TerminateSimulation();
		}
	}
}


void Statistics::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj)
{
    Enter_Method_Silent();

    EV << "*** Statistics module received signal " << signalID;
    EV << " from module " << source->getFullName() << endl;

    // signal from compromised node
    if(signalID == Signal_Msg_sent_V)
    {
        SimpleMsg *m = static_cast<SimpleMsg *>(obj);
        if (m == NULL) return;

        // we add the false message to the vector
        RxMsg *tmp = new RxMsg(source->getFullName(), simTime(), m->getCertLTfrom(), m->getCertLTto());
        Vec_Sent_Message.push_back(tmp);
    }
}


// returns the index of a node. for example gets V[10] as input and returns 10
int Statistics::getNodeIndex(const char *ModName)
{
    std::ostringstream oss;

    for(int h=0 ; ModName[h] != NULL ; h++)
    {
        if ( std::isdigit(ModName[h]) )
        {
            oss << ModName[h];
        }
    }

    int nodeID = std::atoi(oss.str().c_str());

    return nodeID;
}


int Statistics::findInVector(std::vector<NodeEntry *> Vec, const char *name)
{
    unsigned int counter;    // for counter
    bool found = false;

    for(counter=0; counter<Vec.size(); counter++)
    {
        if( strcmp(Vec[counter]->name, name) == 0 )
        {
            found = true;
            break;
        }
    }

    if(!found)
        return -1;
    else
        return counter;
}


// Update vectors in Sumo-mode only
void Statistics::Update_Vectors(const char *str)
{
    int nodeIndex = getNodeIndex(str);
    int counter;

    // #####################################################
    // update the NewRSU_Encounter vector for this vehicle

    counter = findInVector(Vec_NewRSU_Encounter, str);

    // this vehicle has not encountered any RSUs so far
    if(counter == -1)
    {
        NodeEntry *tmp = new NodeEntry(str, nodeIndex, 0, -1, -1);
        Vec_NewRSU_Encounter.push_back(tmp);
    }

    // #####################################################
    // update the GetCRL_RSU vector for this vehicle

    counter = findInVector(Vec_GetCRL_RSU, str);

    // this vehicle has not received any pieces
    if(counter == -1)
    {
        NodeEntry *tmp = new NodeEntry(str, nodeIndex, 0, -1, -1);
        Vec_GetCRL_RSU.push_back(tmp);
    }
}


std::vector<NodeEntry *> Statistics::SortByID(std::vector<NodeEntry *> vec)
{
	for(unsigned int i = 0; i<vec.size()-1; i++)
	{
		for(unsigned int j = i+1; j<vec.size(); j++)
		{
			if( vec[i]->nodeID > vec[j]->nodeID)
				std::swap(vec[i], vec[j]);
		}
	}

	return vec;
}


void Statistics::TerminateSimulation()
{
    // sort the vectors by node ID:
    // 1. to be displayed with alphabetic order in the print_Statistics()
    // 2. to be saved in file with alphabetic order (to be used correctly in Matlab)
    Vec_NoCRLs_V = SortByID(Vec_NoCRLs_V);
    Vec_NewRSU_Encounter = SortByID(Vec_NewRSU_Encounter);
    Vec_GetCRL_RSU = SortByID(Vec_GetCRL_RSU);
    Vec_Received_Message = SortByID(Vec_Received_Message);

    if (ev.isGUI())
    {
        // take snapshot for the last time
        cancelEvent(Timer1);
        printToFile_SnapShots();

        // print how many RSUs each vehicle has encountered to a file
        printToFile_RSUencounter();

        // show results on the screen
        print_Statistics();
    }
    // call this only when we are in command-line mode
    else
    {
        // print detailed statistics to a file.
        printToFile_Detailed();

        // print time of CRL reception for each vehicle.
        printToFile_CRLreception();

        // print message propagation timings to file
        printToFile_FalseMsgPropagation();
    }

    endSimulation();
}


void Statistics::print_Statistics()
{
    EV << endl;

    EV << "******************************************************" << endl;
	EV << "The simulation completed at " << simTime().dbl() << endl;
	EV << "******************************************************" << endl;

    EV << endl;

    EV << "SumoMode is " << (SumoMode ? "enable." : "not enable.") << endl;

	EV << endl;

    EV << "Number of vehicles in the network: " << Var_NoV << endl;
	EV << "Number of RSUs in the network: " << Var_NoRSUs << endl;

    EV << endl;

    EV << "Size of CRL: " << Var_CRLsize_CA << endl;
    EV << "ErasureCode is " << (ErasureCode ? "enable." : "not enable.") << endl;
    EV << "Each vehicle should receive " << Var_Thresh << " CRL pieces to re-construct the CRL." << endl;

    EV << endl;

    EV << "Number of beacons send from all RSUs: " << Var_Beacon_RSU << endl;
    EV << "Number of broadcast beacons send from all vehicles: " << Var_Beacon_V_B << endl;
    EV << "Number of unicast beacons send from all vehicles: " << Var_Beacon_V_U << endl;

    EV << endl;

    EV << "Number of total broadcasts in RSUs: " << Var_NoBroadcast_RSU << endl;
    EV << "I2V_tho = " << Var_I2V_tho << ", thus each RSU can send maximum of " << 10 << " CRL pieces in each broadcast." << endl;
    EV << "V2V_tho = " << Var_I2V_tho << ", thus each V can send maximum of " << 10 << " CRL pieces in each V2V communication." << endl;

    EV << endl;

    EV << "Number of all CRL pieces exchanged in I2V: " <<  Var_CRL_I2V << endl;
    EV << "    - new CRLs: " << Var_CRL_I2V - Var_NoDupCRLs_RSU << endl;
    EV << "    - dup CRLs: " << Var_NoDupCRLs_RSU << endl;
    EV << "Number of all CRL pieces exchanged in V2V: " << Var_CRL_V2V << endl;
    EV << "    - new CRLs: " << Var_CRL_V2V -  Var_NoDupCRLs_V << endl;
    EV << "    - dup CRLs: " << Var_NoDupCRLs_V << endl;

    EV << endl;

   	EV << "The vehicle nodes receive the CRL pieces as below: " << endl;

   	for(int k=0; k<Var_NoV; k++)
   	{
        EV << Vec_NoCRLs_V[k]->name << " received " << Vec_NoCRLs_V[k]->count;
        EV << " CRL pieces in " << Vec_NoCRLs_V[k]->time;

        if(SumoMode)
            EV << " and leaves the network at " << Vec_NoCRLs_V[k]->ExitTime;

        EV << endl;
   	}

    EV << endl;

    if(SumoMode)
    {
        EV << "The vehicle nodes encountered the RSUs as below: " << endl;

        for(int k=0; k<Var_NoV; k++)
        {
            EV << Vec_NewRSU_Encounter[k]->name << " encountered ";
            EV << Vec_NewRSU_Encounter[k]->count << " RSUs, and got pieces from ";
            EV << Vec_GetCRL_RSU[k]->count << " RSUs." << endl;
        }

        EV << endl;
    }

    EV << "Number of total I2V connections in data-link layer: " << Var_IN_RSU_RANGE << endl;
    EV << "Number of total V2V connections in data-link layer: " << Var_IN_V_RANGE / 2 << endl;

    EV << endl;

	int tot_Error = Vec_RxErrors_RSU.size() + Vec_RxErrors_V.size();
	EV << "Number of frames received with error: " << tot_Error << endl;

	EV << endl;

	if(Vec_RxErrors_V.size() > 0)
	{
	    EV << Vec_RxErrors_V.size() << " vehicles received erroneous frames (in order of occurrence): " << endl;

	    for(unsigned int k=0; k<Vec_RxErrors_V.size(); k++)
		    EV << Vec_RxErrors_V[k]->name << " received an erroneous frame in " << Vec_RxErrors_V[k]->time << endl;
	}

	EV << endl;

	if(Vec_RxErrors_RSU.size() > 0)
	{
	    EV << Vec_RxErrors_RSU.size() << " RSUs received erroneous frames (in order of occurrence): " << endl;

	    for(unsigned int k=0; k<Vec_RxErrors_RSU.size(); k++)
		    EV << Vec_RxErrors_RSU[k]->name << " received an erroneous frame in " << Vec_RxErrors_RSU[k]->time << endl;
	}
}


void Statistics::printToFile_RSUencounter()
{
    // choose file name
    char fileName [30];

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(_WIN64)
    sprintf( fileName, "results\\2.RSUencounter-%d nodes.txt", Var_NoV );
#else
    sprintf( fileName, "results//2.RSUencounter-%d nodes.txt", Var_NoV );
#endif

    // open the file in a+ mode
    FILE *pFile = fopen(fileName,"a+");

    if(pFile == NULL)
    {
        EV << "ERROR !!!" << endl;
    }

    fprintf (pFile, "Number of RSUs that each vehicle has encountered: \n\n");

    for(int i=0; i<Var_NoV; i++)
    {
        fprintf (pFile, "vehicle %d;       ", i);
        fprintf (pFile, "%d; \n", Vec_NewRSU_Encounter[i]->count);
    }

    // close the file
    fclose(pFile);
}


void Statistics::printToFile_FalseMsgPropagation()
{
    // get the current run number
    int currentRun = ev.getConfigEx()->getActiveRunNumber();

    // choose file name
    char fileName [30];

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(_WIN64)
    sprintf( fileName, "results\\msgPropagation\\%d.txt", currentRun );
#else
    sprintf( fileName, "results//msgPropagation//%d.txt", currentRun );
#endif

    // open the file in a+ mode
    FILE *pFile = fopen(fileName,"a+");

    if(pFile == NULL)
    {
        EV << "ERROR !!!" << endl;
    }

    std::vector<std::string> iterVar;

    if(!SumoMode)
    {
        // get all iteration variables for CommandLine config
        iterVar = ev.getConfigEx()->unrollConfig("BonnMotion_Cmdenv", false);
    }
    else
    {
        // get all iteration variables for CommandLine_Sumo config
        iterVar = ev.getConfigEx()->unrollConfig("Sumo_Cmdenv", false);
    }

    // get the current repetition number
    int repNum = std::atoi( ev.getConfigEx()->getVariable("repetition") );

    int totalRun;

    if(!SumoMode)
    {
        // get total number of run
        totalRun = ev.getConfigEx()->getNumRunsInConfig("BonnMotion_Cmdenv");
    }
    else
    {
        // get total number of run
        totalRun = ev.getConfigEx()->getNumRunsInConfig("Sumo_Cmdenv");
    }

    // get total number of repetitions
    int totalRep = std::atoi( ev.getConfig()->getConfigValue("repeat") );

    // write general information about the whole config
    fprintf (pFile, "Number of Scenarios;           %d;\n", totalRun / totalRep);
    fprintf (pFile, "Number of Repetitions;         %d;\n", totalRep);
    fprintf (pFile, "Number of Runs;                %d;\n", totalRun);
    fprintf (pFile, "Number of Vehicles;            %d;\n", Var_NoV);
    fprintf (pFile, "Number of RSUs;                %d;\n", Var_NoRSUs);
    fputs("\n", pFile);

    // write scenario information
    fprintf( pFile, "Parameters; %s;\n", iterVar[currentRun].c_str() );
    fputs("\n", pFile);

    // write line 2 (Run Number)
    fprintf (pFile, "Run Number;                %d;\n", currentRun);

    // write line 3 (Scenario Number)
    fprintf (pFile, "Scenario Number;           %d;\n", currentRun / totalRep);

    // write line 4 (Repetition Number)
    fprintf (pFile, "Repetition Number;         %d;\n", repNum);

    fprintf ( pFile, "\n");

    for(unsigned int i=0; i<Vec_Sent_Message.size(); i++)
    {
        fprintf ( pFile, "%s : sends false msg in;       %f;\n", Vec_Sent_Message[i]->name, Vec_Sent_Message[i]->time.dbl() );
        fprintf ( pFile, "%s : cert life-time from;      %f;\n", Vec_Sent_Message[i]->name, Vec_Sent_Message[i]->TSfrom  );
        fprintf ( pFile, "%s : cert life-time to;        %f;\n", Vec_Sent_Message[i]->name, Vec_Sent_Message[i]->TSto );
        fprintf ( pFile, "end of section;                  0;\n\n");
    }

    for(unsigned int i=0; i<Vec_Received_Message.size(); i++)
    {
        int id = Vec_Received_Message[i]->nodeID;

        fprintf ( pFile, "%s : receives false msg in;        %f;\n", Vec_Received_Message[i]->name, Vec_Received_Message[i]->time.dbl() );
        fprintf ( pFile, "%s : receives last CRL piece in;   %f;\n", Vec_Received_Message[i]->name, Vec_NoCRLs_V[id]->time.dbl() );
        fprintf ( pFile, "end of section;                      0;\n\n");
    }

    // close the file
    fclose(pFile);
}


void Statistics::printToFile_Detailed()
{
    // get the current run number
    int currentRun = ev.getConfigEx()->getActiveRunNumber();

	// choose file name
    char fileName [30];

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(_WIN64)
    sprintf( fileName, "results\\detailed\\%d.txt", currentRun );
#else
    sprintf( fileName, "results//detailed//%d.txt", currentRun );
#endif

    // open the file in a+ mode
    FILE *pFile = fopen(fileName,"a+");

    if(pFile == NULL)
    {
    	EV << "ERROR !!!" << endl;
    }

    std::vector<std::string> iterVar;

    if(!SumoMode)
    {
        // get all iteration variables for CommandLine config
        iterVar = ev.getConfigEx()->unrollConfig("BonnMotion_Cmdenv", false);
    }
    else
    {
        // get all iteration variables for CommandLine_Sumo config
        iterVar = ev.getConfigEx()->unrollConfig("Sumo_Cmdenv", false);
    }

    // get the current repetition number
    int repNum = std::atoi( ev.getConfigEx()->getVariable("repetition") );

    int totalRun;

    if(!SumoMode)
    {
	    // get total number of run
        totalRun = ev.getConfigEx()->getNumRunsInConfig("BonnMotion_Cmdenv");
    }
    else
    {
	    // get total number of run
        totalRun = ev.getConfigEx()->getNumRunsInConfig("Sumo_Cmdenv");
    }

    // get total number of repetitions
    int totalRep = std::atoi( ev.getConfig()->getConfigValue("repeat") );

    // write general information about the whole config
    fprintf (pFile, "Number of Scenarios;           %d;\n", totalRun / totalRep);
    fprintf (pFile, "Number of Repetitions;         %d;\n", totalRep);
    fprintf (pFile, "Number of Runs;                %d;\n", totalRun);
    fprintf (pFile, "Number of Vehicles;            %d;\n", Var_NoV);
    fprintf (pFile, "Number of RSUs;                %d;\n", Var_NoRSUs);
	fputs("\n", pFile);

    // write scenario information
	fprintf( pFile, "Parameters; %s;\n", iterVar[currentRun].c_str() );
    fputs("\n", pFile);

	// write line 2 (Run Number)
	fprintf (pFile, "Run Number;                %d;\n", currentRun);

	// write line 3 (Scenario Number)
	fprintf (pFile, "Scenario Number;           %d;\n", currentRun / totalRep);

	// write line 4 (Repetition Number)
	fprintf (pFile, "Repetition Number;         %d;\n", repNum);

    if(!SumoMode)
    {
	    // write line 5 (convergence time)
	    fprintf ( pFile, "Convergence Time;          %f;\n", ConvTime() );
    }
    else
    {
    	// convergence time in Sumo-mode is not applicable !!
    	fprintf ( pFile, "Convergence Time;          %d;\n", 0 );
    }

	// write line 6 (number of errors in vehicles)
	fprintf ( pFile, "Number of Errors;          %d;\n", Vec_RxErrors_V.size() );

	// write line 7
	fprintf ( pFile, "CRLs in I2V (Dup);         %d;\n", Var_NoDupCRLs_RSU );

	// write line 8 (New CRLs in I2V)
	fprintf ( pFile, "CRLs in I2V (New);         %d;\n", Var_CRL_I2V - Var_NoDupCRLs_RSU );

	// write line 9
	fprintf ( pFile, "CRLs in V2V (Dup);         %d;\n", Var_NoDupCRLs_V );

	// write line 10 (New CRLs in V2V)
	fprintf ( pFile, "CRLs in V2V (New);         %d;\n", Var_CRL_V2V -  Var_NoDupCRLs_V );

    // write line 11 (New CRLs in V2V)
    fprintf ( pFile, "RSUs broadcasts;           %d;\n", Var_NoBroadcast_RSU );

	// write line 12 (end section delimiter)
	fprintf ( pFile, "end of section;            0;\n");

	// write new line
	fputs("\n", pFile);

    // close the file
    fclose(pFile);
}


void Statistics::printToFile_CRLreception()
{
    // get the current run number
    int currentRun = ev.getConfigEx()->getActiveRunNumber();

    // choose file name
    char fileName [30];

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(_WIN64)
    sprintf( fileName, "results\\reception\\%d.txt", currentRun );
#else
    sprintf( fileName, "results//reception//%d.txt", currentRun );
#endif

    // open the file in a+ mode
    FILE *pFile = fopen(fileName,"a+");

    if(pFile == NULL)
    {
        EV << "ERROR !!!" << endl;
    }

    std::vector<std::string> iterVar;

    if(!SumoMode)
    {
        // get all iteration variables for CommandLine config
        iterVar = ev.getConfigEx()->unrollConfig("BonnMotion_Cmdenv", false);
    }
    else
    {
        // get all iteration variables for CommandLine_Sumo config
        iterVar = ev.getConfigEx()->unrollConfig("Sumo_Cmdenv", false);
    }

    // get the current repetition number
    int repNum = std::atoi( ev.getConfigEx()->getVariable("repetition") );

    int totalRun;

    if(!SumoMode)
    {
        // get total number of run
        totalRun = ev.getConfigEx()->getNumRunsInConfig("BonnMotion_Cmdenv");
    }
    else
    {
        // get total number of run
        totalRun = ev.getConfigEx()->getNumRunsInConfig("Sumo_Cmdenv");
    }

    // get total number of repetitions
    int totalRep = std::atoi( ev.getConfig()->getConfigValue("repeat") );

    // write general information about the whole config
    fprintf (pFile, "Number of Scenarios;           %d;\n", totalRun / totalRep);
    fprintf (pFile, "Number of Repetitions;         %d;\n", totalRep);
    fprintf (pFile, "Number of Runs;                %d;\n", totalRun);
    fprintf (pFile, "Number of Vehicles;            %d;\n", Var_NoV);
    fprintf (pFile, "Number of RSUs;                %d;\n", Var_NoRSUs);
    fputs("\n", pFile);

    // write scenario information
    fprintf( pFile, "Parameters; %s;\n", iterVar[currentRun].c_str() );
    fputs("\n", pFile);

    // write line 1 (Run Number)
    fprintf (pFile, "Run Number;                    %d;\n", currentRun);

    // write line 2 (Scenario Number)
    fprintf (pFile, "Scenario Number;               %d;\n", currentRun / totalRep);

    // write line 3 (Repetition Number)
    fprintf (pFile, "Repetition Number;             %d;\n", repNum);

    // write line 4 (CRL reception time)
    for(int k=0; k<Var_NoV; k++)
    {
            fprintf ( pFile, "%s : number of pieces;       %d;\n", Vec_NoCRLs_V[k]->name, Vec_NoCRLs_V[k]->count );
            fprintf ( pFile, "%s : last piece in;          %f;\n", Vec_NoCRLs_V[k]->name, Vec_NoCRLs_V[k]->time.dbl() );
    }

    // write line 5 (end section delimiter)
    fprintf ( pFile, "end of section;                0;\n");

    // write new line
    fputs("\n", pFile);

    // close the file
    fclose(pFile);
}


// calculate convergence time in non-Sumo mode.
double Statistics::ConvTime()
{
    // convergence time is obviously the time of last CRL piece reception by the vehicle.
    // we have sorted Vec_NoCRLs_V vector before! and we should find the entry with
    // biggest value in time!

    double conv = 0;

    for(unsigned int i=0; i<Vec_NoCRLs_V.size(); i++)
    {
        if(Vec_NoCRLs_V[i]->time.dbl() > conv)
            conv = Vec_NoCRLs_V[i]->time.dbl();
    }

    return conv;
}


void Statistics::finish()
{

}


Statistics::~Statistics()
{

}
