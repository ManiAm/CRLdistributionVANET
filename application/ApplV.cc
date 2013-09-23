#include <ApplV.h>

Define_Module(ApplV);

const simsignalwrap_t ApplV::mobilityStateChangedSignal = simsignalwrap_t(MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);
bool ApplV::VehicleTypeSet = false;
bool ApplV::runCompromised = false;


// constructor
ApplV::ApplV()
{


}


void ApplV::initialize(int stage)
{
	Appl::initialize(stage);

    if(stage == 0) 
	{
    	// get the value of parameters
	    speed = par("speed");
	    if(speed < 0)
		    error("value for speed is incorrect !!");

	    beacon_Interval = par("beacon_Interval").doubleValue();
	    if(beacon_Interval <= 0)
		    error("value for beacon_Interval is incorrect !!");

		V2V_tho = par("V2V_tho").doubleValue();
		if(V2V_tho < 0)     // V2V_tho can be 0.
		    error("value for V2V_tho is incorrect !!");

        // report to the statistics.
        simsignal_t Signal_V2V_tho = registerSignal("V2V_tho");
        nodePtr->emit(Signal_V2V_tho, V2V_tho);

		V2V_Wait_Beacon = par("V2V_Wait_Beacon").doubleValue();
		if(V2V_Wait_Beacon <= 0)
		    error("value for V2V_Wait_Beacon is incorrect !!");

		V2V_EnableMultiChannel = par("V2V_EnableMultiChannel").boolValue();

		V2V_Wait_CRL = par("V2V_Wait_CRL").doubleValue();
		if(V2V_Wait_CRL <= 0)
		    error("value for V2V_Wait_CRL is incorrect !!");

		MagicCarsRatio = par("MagicCarsRatio").doubleValue();
		if(MagicCarsRatio < 0 || MagicCarsRatio > 100)
		    error("value for MagicCarsRatio is incorrect !!");

		MagicReqTime = par("MagicReqTime").doubleValue();
		if(MagicReqTime < 0)
		    error("value for MagicReqTime is incorrect !!");

		CRLrecons = par("CRLrecons").boolValue();

		CompromisedEnable = par("CompromisedEnable").boolValue();

		CompromisedMsgSendTime = par("CompromisedMsgSendTime").doubleValue();
        if(CompromisedMsgSendTime < 0)
            error("value for CompromisedMsgSendTime is incorrect !!");

        if(CompromisedEnable)
            Timer_Verify  = new cMessage( "Timer_Wait_VerifyPS", Timer_Wait_Verify );

    	// register fromMAC signal and then subscribe to it locally.
    	// MAC layer uses this signal to report to application whether
        // a frame is sent successfully or a frame is received with error.
        // Depending on the applType, one of the ApplV, ApplV_Broadcast, ApplV_BBroadcast or ApplV_MPB can receive this signal.
    	Signal_fromMAC = registerSignal("fromMAC");
    	nodePtr->subscribe("fromMAC", this);

    	Signal_Magic_Req = registerSignal("Magic_Req");

        // set variables
    	CRL_Piece_No = 0;
    	CRLChannel = -1;
    	state = STATE_IDLE;
    	EncounteredRSU_old = "";

    	totFrames = 0;
    	forCounter = 0;

        // we assume, all vehicles are private at the start!
    	// we will select special vehicles later (in stage == 1).
    	vType = privateV;

    	// initially none of the vehicles are compromised
    	IsCompromised = false;

    	receiveCompromisedMsg = false;

        WATCH(CRL_Piece_No);
        WATCH(CRLChannel);
        WATCH(totalPieces);
        WATCH(state);
        WATCH(vType);

        // rangeStart, rangeEnd and unionRange are for ICE
        // are used in sendBeacon method
        rangeStart = 0;
        rangeEnd = totalPieces-1;

        // report this V to the statistics.
    	simsignal_t Signal_NoV = registerSignal("NoV");
    	nodePtr->emit(Signal_NoV, 1);

        // define and start Timer1 (send broadcast beacon)
        Timer1 = new cMessage( "Timer_Beacon_V", Timer_Beacon_V );
		scheduleAt(simTime() + dblrand() * beacon_Interval, Timer1);
    }
    // check if we are in second stage (stage == 1) of initialization!
    else if(stage == 1)
    {
    	if(!SumoMode)
    	{
            // in non-Sumo mode, in stage == 0 all vehicles in the network were initialized
    	    // and they are all assumed to be of type private !
    		// setMagicCar has access to all vehicles in the network and chooses some
    		// vehicles randomly and set them as magic cars. So we have no make sure that we
    		// run it once. For doing this I used a static boolean variable (VehicleTypeSet).
    		// Also we should check if value of MagicCarsRatio is not zero!
    	    if(MagicCarsRatio != 0 && !VehicleTypeSet)
    	    {
    		    setMagicCar();
    		    VehicleTypeSet = true;
    	    }
    	}
    	else
    	{
   	        // get a pointer to the traci.
        	traci = TraCIMobilityAccess().get(getParentModule());

        	// subscribe to mobilityStateChangedSignal.
            // nodePtr->subscribe(mobilityStateChangedSignal, this);

        	// Each vehicle runs setVehicleType method.
    	    setVehicleType();
    	}

    	// At this point, the vType of this vehicle is known.
    	// if this vehicle is magic, subscribe it to Magic_res signal.
    	if( isMagicCar() )
    	{
    	    simulation.getSystemModule()->subscribe("Magic_Res", this);

        	// for this magic car, we start a timer that when expires, a Magic_Req signal is sent to CA.
    		Timer2 = new cMessage( "Timer_Magic_Req", Timer_Magic_Req );
    		scheduleAt(simTime() + MagicReqTime, Timer2);
    	}

    	// select a random node in the whole network as compromised
        if(CompromisedEnable && !runCompromised)
        {
            setCompromised();
            runCompromised = true;
        }
    }
    // check if we are in third stage (stage == 2) of initialization.
    else if(stage == 2)
    {
    	if(ev.isGUI())
    	{
            // the r-tag (circle around vehicle) will be set in second stage (stage == 1) of
            // BaseConnectionManager.cc. We will remove the r-tag at third stage (stage == 2).
    	    EV << "*** " << nodePtr->getFullName() << ": removing the r-tag!" << endl;

    	    nodePtr->getDisplayString().removeTag("r");
    	    updateText();
    	}

    	// if this vehicle is compromised, then send the msg at TimeMsgSend
    	if(IsCompromised == true)
    	{
            Timer3 = new cMessage( "Timer_Sending_Compromised_Msg", Timer_Sending_Compromised_Msg );
            scheduleAt(simTime() + CompromisedMsgSendTime, Timer3);
    	}
    }
}


void ApplV::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
        if( msg->getKind() == Timer_Beacon_V )
        {
        	// TODO: uncomment it
            // state = STATE_BROADCAST_BEACON_SENDING;
        	sendBeacon(-1, true);    // -1: broadcast beacon
        }
        else if( msg->getKind() == Timer_Magic_Req )
        {
        	EV << "*** " << nodePtr->getFullName() << " sends a Magic_Req signal to CA." << endl;

        	std::vector<CRL_Piece *> vec;   // create an empty vector !
            MagicCarsData *dummyData = new MagicCarsData("", vec);   // create a dummy data.

        	// we send the Magic_Req signal and wait for the Magic_Res signal from CA.
        	nodePtr->emit(Signal_Magic_Req, dummyData);
        }
        else if( msg->getKind() == Timer_Sending_Compromised_Msg )
        {
            sendMsg();
        }
        else if( msg->getKind() == Timer_Wait_Verify)
        {

        }
    }
	else if(msg->getArrivalGateId()==lowerLayerOut)
	{

    }
	else if(msg->getArrivalGateId()==lowerControlOut)
	{

    }
	else if(msg->getArrivalGateId()==lowerControlIn)
	{

    }
	else if(msg->getArrivalGateId()==lowerLayerIn)
	{
	    if(msg->getKind() == Msg_Beacon_V)
		{
		    recieveBeacon(msg);
		}
	    else if(msg->getKind() == Msg_Beacon_RSU)
	    {
            recieveBeacon(msg);
	    }
	    // Receive a CRL message from RSU
		else if(msg->getKind() == Msg_CRL_RSU)
		{
			simsignal_t Signal_CRL_I2V = registerSignal("CRL_I2V");
			nodePtr->emit(Signal_CRL_I2V, 1);

			// ###########################################################
		    // how many different RSUs this vehicle has encountered so far
			CRL_Piece *m = static_cast<CRL_Piece *>(msg);
		    if( strcmp(m->getName(), EncounteredRSU_old.c_str()) != 0 )
		    {
	            simsignal_t Signal_GetCRL_RSU = registerSignal("GetCRL_RSU");
	            nodePtr->emit(Signal_GetCRL_RSU, 1);

		        EncounteredRSU_old = m->getName();
		    }
		    // ###########################################################

			recieveCRL(msg);
		}
	    // Receive a CRL message from vehicle
		else if(msg->getKind() == Msg_CRL_V)
		{
			simsignal_t Signal_CRL_V2V = registerSignal("CRL_V2V");
		    nodePtr->emit(Signal_CRL_V2V, 1);

			recieveCRL(msg);
		}
	    // Receive a compromised msg from vehicle
		else if(msg->getKind() == Msg_Compromised_V)
		{
	        // scheduleAt(simTime() + 0.1, Timer_VerifyPS);

		    // check validity first.
		    bool result = IsValid(msg);

		    // if ok, then call receiveMsg(msg);
		    if(result)
		    {
		        receiveMsg(msg);
		    }
		}
    }
}


void ApplV::receiveSignal(cComponent *source, simsignal_t signalID, const char *string)
{
	Enter_Method_Silent();

	if(signalID == Signal_fromMAC)
	{
	    if( strcmp(string, "TxOver") == 0 )
	    {
		    if (state == STATE_BROADCAST_BEACON_SENDING)
		    {
		        EV << "**** " << nodePtr->getFullName() << ": MAC layer signals that the broadcast beacon has sent." << endl;
		        state = STATE_BROADCAST_BEACON_SEND;
		    }
	    }
	    else if( strcmp(string, "RxError") == 0 )
	    {
		    EV << "**** " << nodePtr->getFullName() << ": MAC layer signals that the Msg is received with ERROR." << endl;
		    if ( ev.isGUI() ) nodePtr->bubble("Msg has error");
	    }
	}
}


void ApplV::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj)
{
	Enter_Method_Silent();

	// we receive a Magic_Res signal from CA.
	// Note that only the magic cars are subscribed to Magic_Res signal.
    if(signalID == registerSignal("Magic_Res"))
    {
    	MagicCarsData *MagicData = static_cast<MagicCarsData *>(obj);

    	// if we have not sent the Magic_Req signal, return, because another magic car requested it !
    	if( strcmp(MagicData->name, nodePtr->getFullName()) != 0 )
    		return;

        EV << "*** " << nodePtr->getFullName() << " received the CRL." << endl;

        // it is possible that the magic car, has already received pieces from
        // I2V or V2V. Delete them from heap memory.
        for(unsigned int i = 0; i < PiecesCRLcollected.size(); i++)
        {
            // only if the Msg does not come from CA magically!
            if(PiecesCRLcollected[i]->getKind() != Msg_CRL_CA)
            {
                delete PiecesCRLcollected[i];
            }
        }

        // clear the pieces received so far
        PiecesCRLcollected.clear();

        PiecesCRLcollected = MagicData->data;

    	CRL_Piece_No = MagicData->data.size();

        // update the rangeStart, rangeEnd variables (only used in ICE)
    	updateRange();

    	if(ev.isGUI())
    	{
    	    char buf[15];
    	    sprintf(buf, "[CA] pieces: %d", CRL_Piece_No);
    	    nodePtr->getDisplayString().setTagArg("t", 0, buf);

    	    // add a checkmark on the icon showing that this vehicles received all CRL pieces
            nodePtr->getDisplayString().updateWith("i2=status/checkmark,green");
    	}

    	// send signal to statistics to report number of CRL pieces.
    	simsignal_t Signal_NoCRLs_V = registerSignal("NoCRLs_V");
    	nodePtr->emit(Signal_NoCRLs_V, CRL_Piece_No);
    }
    // receive the signals from the traci manager (only in Sumo mode).
	else if (signalID == mobilityStateChangedSignal)
	{

	    /*
	    EV << "external ID: " << src->getExternalId() << endl;
	    EV << "road ID: " << src->getRoadId() << endl;
     // EV << "lane ID: " <<  << endl;
	    EV << "speed: " << src->getSpeed() << endl; */
	}
}


void ApplV::sendBeacon(int RecID, bool periodic)
{
	// RecID is the receiver ID. it will be stored in the DestAddr field of the beacon.
	// RecID = -1 means application layer broadcast

    if(RecID == -1)
    {
        EV << "**** " << nodePtr->getFullName() << " is sending a broadcast beacon ..." << endl;

        simsignal_t Signal_Beacon_V_B = registerSignal("Beacon_V_B");
        nodePtr->emit(Signal_Beacon_V_B, 1);
    }
    else
    {
        EV << nodePtr->getFullName() << " is sending a unicast beacon ..." << endl;

        simsignal_t Signal_Beacon_V_U = registerSignal("Beacon_V_U");
        nodePtr->emit(Signal_Beacon_V_U, 1);
    }

    if ( ev.isGUI() ) nodePtr->bubble("Sending beacon");

    Beacon *pkt = new Beacon(nodePtr->getFullName(), Msg_Beacon_V);

	pkt->setSrcAddr(nodeID);
	pkt->setDestAddr(RecID);

	pkt->setNodeName(nodePtr->getFullName());

    // get and then set the current position of the node
	int xPos = std::atoi( nodePtr->getDisplayString().getTagArg("p",0) );  // x-coordinate of node
    int yPos = std::atoi( nodePtr->getDisplayString().getTagArg("p",1) );  // y-coordinate of node
	pkt->setPositionX( xPos );
	pkt->setPositionY( yPos );

	pkt->setSpeed(speed);

    // ####################################################
    // set the NeedCRL flag (used in C2C_Epidemic and ICE)
    // ####################################################

    if( isMagicCar() || (PiecesCRLcollected.size() == (unsigned int)totalPieces) )
        pkt->setNeedCRL(false);
    else
        pkt->setNeedCRL(true);

    // ############################################
    // set the CRL_Piece_No (used in MPB and ICEv2)
    // ############################################

    pkt->setCRL_Piece_No(CRL_Piece_No);

    // ###################################################
    // set the rangeStart and rangeEnd (used only by ICE)
    // ###################################################

    EV << "**** " << nodePtr->getFullName() << " has these pieces: ";

    for(unsigned int i=0; i<PiecesCRLcollected.size();i++)
        EV << PiecesCRLcollected[i]->getSeqNo() << ", ";

    EV << endl;

    // set the start and end index of range
    pkt->setRangeS(rangeStart);
    pkt->setRangeE(rangeEnd);

    EV << "**** " << nodePtr->getFullName() << " range start: " << rangeStart;
    EV << ", range end: " << rangeEnd << endl;

	// ########################################################
    // set the CRL channel and 'NeedCRL' (used only by NewMPB)
    // ########################################################

	// only if multiChannel is enabled and a V2V algorithm is running
    if( V2V_EnableMultiChannel && strcmp (applType, "ApplV") != 0 )
    {
        // a random channel is selected from 2 to 14
        CRLChannel = 2 + intrand(13);
    }
    else CRLChannel = -1;

    pkt->setCRLchannel(CRLChannel);

    // ###################################################
    // ###################################################

    pkt->setBitLength(1000);

    // controlinfo for network layer (L3BROADCAST = -1)
    pkt->setControlInfo( new NetwControlInfo(-1) );

    // ####################################
    // print some info in the event window
    // ####################################

	// changing the channel to 1 (control channel) if it is not 1
    if(nicClassPtr->getChannel() != 1)
    {
        EV << "**** " << nodePtr->getFullName() << " is switching channel from " << nicClassPtr->getChannel() << " to 1." << endl;
        nicClassPtr->switchChannel(1);
    }

	send(pkt, lowerLayerOut);

	if(periodic) scheduleAt(simTime() + beacon_Interval, Timer1);
}


// receive a beacon from an RSU or a vehicle
void ApplV::recieveBeacon(cMessage *msg)
{
    Beacon *m = static_cast<Beacon *>(msg);
	if (m == NULL) return;

	if ( ev.isGUI() )
	{
	    // print on output, which type of beacon we have received.
        if(m->getDestAddr() == -1)
	    {
            EV << "*** " << nodePtr->getFullName() << " received a broadcast beacon from " << m->getNodeName() << endl;
            nodePtr->bubble("received a broadcast beacon");
	    }
	    else if(m->getDestAddr() == nodeID)
	    {
		    EV << "*** " << nodePtr->getFullName() << " received a unicast beacon from " << m->getNodeName() << endl;
		    nodePtr->bubble("received a unicast beacon");
	    }
	    else
	    {
		    EV << "*** " << nodePtr->getFullName() << " received a unicast beacon (not-mine) from " << m->getNodeName() << endl;
		    nodePtr->bubble("received a unicast beacon (not-mine)");
	    }

	    EV << "*** Extracting beacon information: " << endl;

        EV << "source Add: " << m->getSrcAddr() << endl;
        EV << "destination Add: " << m->getDestAddr() << endl;
        EV << "node name: " << m->getNodeName() << endl;
        EV << "position (x,y): " << m->getPositionX() << ", " << m->getPositionY() << endl;
        EV << "speed: " << m->getSpeed() << endl;
        EV << "number of pieces: " << m->getCRL_Piece_No() << endl;
        EV << "CRL channel: " << m->getCRLchannel()  << endl;
        EV << "Need CRL: " << m->getNeedCRL() << endl;
        EV << "Range Start: " << m->getRangeS() << endl;
        EV << "Range End: " << m->getRangeE() << endl;
	}

    // if we are running a V2V algorithm.
    // each algorithm has its own Run_V2V_Algorithm method.
    if( strcmp (applType, "ApplV") != 0 )
    {
        Run_V2V_Algorithm(m);
    }

    delete msg;
}


// Receive a CRL from RSU or from another vehicle
void ApplV::recieveCRL(cMessage *msg)
{
    // if any pieces is received (new or duplicate) from RSU or nearby vehicle, we set this flag.
    // is used in MPB and ICE method.
    AnyPieceReceived = true;

    CRL_Piece *m = static_cast<CRL_Piece *>(msg);
	if (m == NULL) return;

    if ( ev.isGUI() )
    {
        EV << "*** " << nodePtr->getFullName() << " received a CRL piece:" << endl;

        EV << "Msg name: " << m->getName() << endl;
        EV << "CRL Version: " << m->getCRLversion() << endl;
        EV << "Timestamp: " << m->getTimestamp() << endl;
        EV << "Sequence Number: " << m->getSeqNo() << endl;
        EV << "CA ID: " << m->getCAid() << endl;
        EV << "Padding: " << m->getPad() << endl << endl;
    }

	// add it to PiecesCRLcollected, but first search it in PiecesCRLcollected to
	// see if we previously received it. We use SeqNo as the key
	bool found = false;

	for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
	{
		if( m->getSeqNo() == PiecesCRLcollected[i]->getSeqNo() )
		{
			found = true;
			break;
		}
	}

	if(found)
	{
        if ( ev.isGUI() )
        {
            nodePtr->bubble("duplicate copy !!");
            EV << " --> duplicate copy!!" << endl;
        }

        // send signal to statistics to report duplicate CRLs.
        if(msg->getKind() == Msg_CRL_RSU)
        {
            simsignal_t Signal_DupCRLs_V = registerSignal("DupCRLs_V");
            nodePtr->emit(Signal_DupCRLs_V, 0);
        }
        else if(msg->getKind() == Msg_CRL_V)
        {
            simsignal_t Signal_DupCRLs_V = registerSignal("DupCRLs_V");
            nodePtr->emit(Signal_DupCRLs_V, 1);
        }

        delete msg;  // delete the message
	}
	// no copy is found in PiecesCRLcollected
	else
	{
		if ( ev.isGUI() )
		{
		    nodePtr->bubble("received a new CRL piece");
		    EV << " --> save in array." << endl;
		}

		PiecesCRLcollected.push_back(m);  // store the CRL internally

		CRL_Piece_No++;  // Increment the number of CRLs received so far

        if(ev.isGUI())
        {
            EV << "*** now we have " << CRL_Piece_No << " CRL pieces." << endl;
            updateText();  // update the text in top of the V nodes.
        }

        // update the rangeStart, rangeEnd variables (only used in ICE)
		updateRange();

		// check to see if we can re-construct the CRL message, with this CRL piece.
		CRLreconstruction();

        // send signal to statistics to report number of CRLs received so far.
        simsignal_t Signal_NoCRLs_V = registerSignal("NoCRLs_V");
        nodePtr->emit(Signal_NoCRLs_V, CRL_Piece_No);
	}
}


// compromised node creates and broadcasts a msg
void ApplV::sendMsg()
{
    SimpleMsg *pkt = new SimpleMsg(nodePtr->getFullName(), Msg_Compromised_V);

    pkt->setPayload("accident happened in x y");
    pkt->setSignature("signature");
    pkt->setCertLTfrom(0);
    pkt->setCertLTto(Pseudonym_lifeTime);

    pkt->setBitLength(1000);

    // controlinfo for network layer (L3BROADCAST = -1)
    pkt->setControlInfo( new NetwControlInfo(-1) );

    // ####################################
    // print some info in the event window
    // ####################################

    // changing the channel to 1 (control channel) if it is not 1
    if(nicClassPtr->getChannel() != 1)
    {
        EV << "**** " << nodePtr->getFullName() << " is switching channel from " << nicClassPtr->getChannel() << " to 1." << endl;
        nicClassPtr->switchChannel(1);
    }

    send(pkt, lowerLayerOut);

    EV << "*** " << nodePtr->getFullName() << " sent a false message." << endl;

    // send signal to statistics
    simsignal_t Signal_Msg_sent_V = registerSignal("Msg_sent_V");
    nodePtr->emit(Signal_Msg_sent_V, pkt);
}


bool ApplV::IsValid(cMessage *msg)
{
    // step 1: Pseudonym verify: we check the lifetime of certificate here!

    SimpleMsg *m = static_cast<SimpleMsg *>(msg);
    if (m == NULL)
        return false;

    if( simTime().dbl() >= m->getCertLTfrom() && simTime().dbl() <= m->getCertLTto() )
    {
        return true;
    }
    else
    {
        EV << "*** " << nodePtr->getFullName() << " discards a msg:" << endl;

        EV << "Msg name: " << m->getName() << endl;
        EV << "Payload: " << m->getPayload() << endl;
        EV << "Signature: " << m->getSignature() << endl;
        EV << "Certificate Life-time from: " << m->getCertLTfrom() << endl;
        EV << "Certificate Life-time to: " << m->getCertLTto() << endl;
        EV << endl;

        return false;
    }

    // step 2: msg verify
}


void ApplV::receiveMsg(cMessage *msg)
{
    SimpleMsg *m = static_cast<SimpleMsg *>(msg);
    if (m == NULL) return;

    if(IsCompromised || receiveCompromisedMsg)
        return;

    EV << "*** " << nodePtr->getFullName() << " received a msg:" << endl;

    EV << "Msg name: " << m->getName() << endl;
    EV << "Payload: " << m->getPayload() << endl;
    EV << "Signature: " << m->getSignature() << endl;
    EV << "Certificate Life-time from: " << m->getCertLTfrom() << endl;
    EV << "Certificate Life-time to: " << m->getCertLTto() << endl;
    EV << endl;

    if ( ev.isGUI() )
    {
        nodePtr->bubble("msg received");
        nodePtr->getDisplayString().updateWith("i2=status/excl");
    }

    simsignal_t Signal_Msg_received_V = registerSignal("Msg_received_V");
    nodePtr->emit(Signal_Msg_received_V, 0);

    // re-broadcast this message
    send(msg, lowerLayerOut);

    receiveCompromisedMsg = true;
}


void ApplV::updateText()
{
	char buf[15];
	sprintf(buf, "pieces: %d", CRL_Piece_No);

	nodePtr->getDisplayString().setTagArg("t", 0, buf);
}


// set the MagicCars (in non-Sumo mode only).
void ApplV::setMagicCar()
{
	// get number of vehicles.
	int numV = nodePtr->getParentModule()->par("numV");

	// an array which contains the index of vehicles from 0 to numV - 1
	int vList[numV];

	// initialize vList with 0 to numV - 1
	for(int i=0; i<numV; i++)
	{
		vList[i] = i;
	}

	int rnd, tmp;

	// now we shuffle the vList array to have random arrangement
	// of vehicle indexes.
	for (int i = numV - 1; i >= 0; i--)
	{
	    rnd = intrand( numV );

	    if(i != rnd)
	    {
	    	tmp = vList[i];
	    	vList[i] = vList[rnd];
	    	vList[rnd] = tmp;
	    }
	}

	// calculate how many magic cars we can have.
    int n = (MagicCarsRatio / 100) * numV;

    EV << "*** " << MagicCarsRatio << " percent of vehicles are Magic -> " << n << " out of " << numV << endl;

    for(int i=0; i<n; i++)
    {
        // get a pointer to this vehicle module.
        cModule *vehiclePtr = simulation.getSystemModule()->getSubmodule("V", vList[i]);

        // get a pointer to the application sub-module
        cModule *appPtr = vehiclePtr->getSubmodule("appl");

        // get a pointer to the application class
        ApplV *classPtr = check_and_cast<ApplV *>(appPtr);

        classPtr->vType = public_army;

        // now change the icon
    	if(ev.isGUI())
            vehiclePtr->getDisplayString().updateWith("i=old/ball,red,100");

        EV << "*** " << vehiclePtr->getFullName() << " type is public_army." << endl;
    }
}


// set the vehicle type (in Sumo-mode only).
void ApplV::setVehicleType()
{
	// get the type of vehicle
	const char* externalID = traci->getExternalId().c_str();

    EV << "**** " << nodePtr->getFullName() << " type is " << externalID << endl;

	if (strstr(externalID, "public_army") != NULL)
	{
		vType = public_army;

    	if(ev.isGUI())
		    nodePtr->getDisplayString().updateWith("i=old/ball,red,100");
	}
    else if(strstr(externalID, "bus") != NULL)
	{
		vType = bus;

    	if(ev.isGUI())
		    nodePtr->getDisplayString().updateWith("i=old/ball,green,100");
	}
	// any other vehicle is treated as a private vehicle.
	else
	{
		vType = privateV;

    	if(ev.isGUI())
		    nodePtr->getDisplayString().updateWith("i=old/ball");
	}
}


// select a random node and set it as compromised
void ApplV::setCompromised()
{
    // get number of vehicles.
    int numV = nodePtr->getParentModule()->par("numV");

    int rnd = intrand( numV );

    // get a pointer to this vehicle module.
    cModule *vehiclePtr = simulation.getSystemModule()->getSubmodule("V", rnd);

    // get a pointer to the application sub-module
    cModule *appPtr = vehiclePtr->getSubmodule("appl");

    // get a pointer to the application class
    ApplV *classPtr = check_and_cast<ApplV *>(appPtr);

    classPtr->IsCompromised = true;

    // now change the icon
    if(ev.isGUI())
        vehiclePtr->getDisplayString().updateWith("i2=status/stop");

    EV << "*** " << vehiclePtr->getFullName() << " is compromised." << endl;
}


// if the current vehicle is a magic car.
bool ApplV::isMagicCar()
{
    if(vType == public_army || vType == bus)
        return true;
    else
        return false;
}


void ApplV::CRLreconstruction()
{
    // if ErasureCode is not enabled, we have to wait to receive all
    // CRL pieces to be able to reconstruct the original CRL message.
    if(!ErasureCode && CRL_Piece_No == NoSegments)
    {
        EV << endl;
        EV << "*** " << nodePtr->getFullName() << " received all " << NoSegments << " CRL pieces." << endl;
        EV << "Seq number of the received CRL pieces are: ";

        for(unsigned int i=0;i<PiecesCRLcollected.size();i++)
        {
            EV << PiecesCRLcollected[i]->getSeqNo() << "  ";
        }

        EV << endl;

        EV << "Original CRL message can be re-constructed." << endl << endl;

        if(ev.isGUI() && !IsCompromised)
            nodePtr->getDisplayString().updateWith("i2=status/checkmark,green");

        // do the real CRL reconstruction
        if(CRLrecons)
            CRLreconstruction_NoErasure();
    }
    // if ErasureCode is enabled then reception of M CRL pieces is enough
    // for original CRL reconstruction.
    else if(ErasureCode && CRL_Piece_No == M)
    {
        EV << endl;
        EV << "*** " << nodePtr->getFullName() << " received M = " << M << " out of N = " << N << " CRL pieces!" << endl;
        EV << "Seq number of the received CRL pieces are: ";

        for(unsigned int i=0;i<PiecesCRLcollected.size();i++)
        {
            EV << PiecesCRLcollected[i]->getSeqNo() << "  ";
        }

        EV << endl;

        EV << "Original CRL message can be re-constructed." << endl << endl;

        if(ev.isGUI() && !IsCompromised)
            nodePtr->getDisplayString().updateWith("i2=status/checkmark,green");

        // do the real CRL reconstruction.
        if(CRLrecons)
            CRLreconstruction_Erasure();
    }
}


void ApplV::CRLreconstruction_NoErasure()
{
    // we have CRL pieces in PiecesCRLcollected vector.
    // We should sort the pieces based on the SeqNo.
    std::vector<CRL_Piece *> vec (PiecesCRLcollected);

    for(unsigned int i = 0; i<vec.size()-1; i++)
    {
        for(unsigned int j = i+1; j<vec.size(); j++)
        {
            if( vec[i]->getSeqNo() > vec[j]->getSeqNo())
                std::swap(vec[i], vec[j]);
        }
    }

    // now we are able to re-construct the original CRL message.
    std::stringstream ostr;

    for(unsigned int i = 0; i<vec.size(); i++)
        ostr << vec[i]->getPayload();

    // use boost to de-serialize
    std::vector<Certificate *> recCRL;

    {
        boost::archive::text_iarchive ia(ostr);
        // read class state from archive
        ia >> recCRL;
    }

    EV << "Re-constructed CRL is:" << endl;

    for(unsigned int i = 0; i < recCRL.size(); i++)
    {
        EV << "Certificate name: " << recCRL[i]->CerName << ", ";
        EV << "CA name: " << recCRL[i]->CAname << ", ";
        EV << "CA ID: " << recCRL[i]->CAid << ", ";
        EV << "Node ID: " << recCRL[i]->NodeID << ", ";
        EV << "Node Name: " << recCRL[i]->NodeName << endl;
    }

    EV << endl;
}


void ApplV::CRLreconstruction_Erasure()
{
    // we have CRL pieces in PiecesCRLcollected vector.
    // we may have received any subset of CRL pieces.
    // also there is no need to sort the PiecesCRLcollected.

    // To calculate matrix B, we should form the equation A'.x = W' in which x is matrix B.
    // we formed matrix W' based on the received CRL pieces.
    // we also know matrix A and can easily form matrix A'.
    // Finally we can solve the above equation and find x (Matrix B).

    // we should calculate L first. For doing this we get the string from
    // one of the pieces and count the segments (using # delimiter)
    std::string str = PiecesCRLcollected[0]->getPayload();

    int L = 0;

    for(unsigned int i=0; i<str.size(); i++)
        if(str[i] == '#') L++;

    // Step 1: form Matrix W, based on the received CRL pieces

    Matrix<float, Dynamic, Dynamic> Matrix_W;
    Matrix_W.resize(N,L);
    Matrix_W = ArrayXXf::Zero(N,L);

    int col = 0;
    std::ostringstream oss;

    int iReceivedRow[N];
    for (int i = 0; i < N; i++)
    {
        iReceivedRow[i] = -1;
    }

    for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
    {
        iReceivedRow[i] = PiecesCRLcollected[i]->getSeqNo();

        std::string str = PiecesCRLcollected[i]->getPayload();

        for(unsigned int j=0; j<str.size(); j++)
        {
            if(str[j] == '#')
            {
                int f = std::atoi(oss.str().c_str());
                Matrix_W(iReceivedRow[i],col) = f;
                col++;
                oss.str("");  // clear the stream
            }
            else
                oss << str[j];
        }

        col = 0;
    }

    EV << "Now we form Matrix W, based on the received CRL pieces:" << endl;

    EV << Matrix_W << endl << endl;

    // Step 2: Modify matrix A a little bit to be consistent with matrix w

    Matrix<float, Dynamic, Dynamic> New_Matrix_A;
    New_Matrix_A = Matrix_A;

    for (int i = 0; i < N; i++)
    {
        bool found = false;

        for (int j = 0; j < N; j++)
        {
            if (i == iReceivedRow[j])
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            New_Matrix_A.row(i).setZero();
        }
    }

    EV << "Modified Matrix A is:" << endl;
    EV << New_Matrix_A << endl << endl;

    // Step 3: A.x = W -> we are looking for x (Matrix B)

    Matrix<float, Dynamic, Dynamic> Matrix_B;
    Matrix_B.resize(M,L);
    Matrix_B = ArrayXXf::Zero(M,L);

    Matrix_B = New_Matrix_A.colPivHouseholderQr().solve(Matrix_W);

    EV << "Matrix B is calculated as following:" << endl;

    EV << Matrix_B << endl << endl;

    // Step 4: colPivHouseholderQr function does not compute Matrix B very accurately.
    // At the next step, we should round each entry to get a perfect integer.

    for(int i=0; i<M; i++)
        for(int j=0; j<L; j++)
            Matrix_B(i,j) = round( (double)Matrix_B(i,j) );

    EV << "Matrix B after rounding:" << endl;

    EV << Matrix_B << endl << endl;

    // Now we read matrix B, column-wise.
    std::ostringstream matrixB_str;
    matrixB_str.str("");

    for(int j=0; j<L; j++)   // column
    {
        for(int i=0; i<M; i++)
            matrixB_str << (char)Matrix_B(i,j);
    }

    EV << "Reading Matrix B in column-wise fashion:" << endl;

    for(unsigned long i=0; i< matrixB_str.str().size(); i++)
    {
        EV << (unsigned int)matrixB_str.str().at(i) << " " ;
    }

    EV << endl << endl;


    // todo:
    char fileName [30];
    sprintf( fileName, "matrix.txt");
    // open the file in a+ mode
    FILE *pFile = fopen(fileName,"a+");


    // Remove the padding (if exists) from ostr
    int pad = PiecesCRLcollected[0]->getPad();

    unsigned long len = matrixB_str.str().size();
    std::stringstream ostr;
    ostr << matrixB_str.str().substr(0,len-pad);

    if(pad == 0)
    {
        EV << "No padding is present!" << endl;
    }
    else
    {
        EV << "After removing padding (" << pad << " zeros were removed)." << endl;

        for(unsigned long i=0; i< ostr.str().size(); i++)
        {
            EV << (unsigned int)ostr.str().at(i) << " " ;
            fprintf (pFile, "%d ", (unsigned int)ostr.str().at(i) );
        }

        EV << endl << endl;
    }

    fclose(pFile);


    /*
    // now we re-construct the original CRL message.
    std::vector<Certificate *> recCRL;

    {
        boost::archive::text_iarchive ia(ostr);
        ia >> recCRL;
    }

    EV << "Re-constructed CRL is:" << endl;

    for(unsigned int i = 0; i < recCRL.size(); i++)
    {
        EV << "Certificate name: " << recCRL[i]->CerName << ", ";
        EV << "CA name: " << recCRL[i]->CAname << ", ";
        EV << "CA ID: " << recCRL[i]->CAid << ", ";
        EV << "Node ID: " << recCRL[i]->NodeID << ", ";
        EV << "Node Name: " << recCRL[i]->NodeName << endl;
    }

    EV << endl;
    */
}


double ApplV::round(double r)
{
    double n = (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);

    return abs(n);
}


void ApplV::finish()
{


}


ApplV::~ApplV()
{
    cancelAndDelete(Timer1);

    for(unsigned int i = 0; i < PiecesCRLcollected.size(); i++)
    {
        // only if the Msg does not come from CA magically!
        if(PiecesCRLcollected[i]->getKind() != Msg_CRL_CA)
        {
    	    delete PiecesCRLcollected[i];
        }
    }

	// if this is a magic car, unsubscribe it from Magic_Res on deletion.
    // In Sumo-mode vehicles are deleted at the end, and this can lead to a crash !
    // If we don't do it in Sumo-mode, and a vehicle is deleted, it is still
    // in the subscription list of Magic_Res signal. If CA emits Magic_res signal, the
    // program crashes !!
    if( isMagicCar() )
    {
    	cancelAndDelete(Timer2);
	    simulation.getSystemModule()->unsubscribe("Magic_Res", this);
    }
}
