#include <ApplRSU.h>


Define_Module(ApplRSU);

// constructor
ApplRSU::ApplRSU()
{

}


void ApplRSU::initialize(int stage)
{
	Appl::initialize(stage);

    if(stage == 0) 
	{
    	dataFromCA = findGate("dataFromCA");

    	forCounter = 0;
    	AnyoneNeedCRL = false;
    	state = STATE_IDLE;

        broadcastMask = new int[totalPieces];
        for (int i=0; i<totalPieces; i++)
            broadcastMask[i] = 0;    // Initialize all elements to zero.

		// get the value of parameters
        CRL_Interval = par("CRL_Interval");
        beacon_Interval = par("beacon_Interval").doubleValue();
        I2V_tho = par("I2V_tho");
        I2V_EnableMultiChannel = par("I2V_EnableMultiChannel").boolValue();

        if(CRL_Interval <= 0)
		    error("value for RSU_ONLY is incorrect !!");

        if(beacon_Interval <= 0)
            error("value for beacon_Interval is incorrect !!");

        if(I2V_tho < 0)
		    error("value for I2V_tho is incorrect !!");

        // report to the statistics.
        simsignal_t Signal_I2V_tho = registerSignal("I2V_tho");
        nodePtr->emit(Signal_I2V_tho, I2V_tho);

    	// register fromMAC signal and then subscribe to it locally.
    	// MAC layer uses this signal to report to application whether
        // a frame is sent successfully or a frame is received with error.
    	Signal_fromMAC = registerSignal("fromMAC");
    	nodePtr->subscribe("fromMAC", this);

        // report this RSU to the statistics.
    	simsignal_t Signal_NoRSUs = registerSignal("NoRSUs");
   	    nodePtr->emit(Signal_NoRSUs, 1);

   	    // RSUs broadcast CRL pieces periodically
   	    if(strcmp (applType, "ApplV") == 0 || strcmp (applType, "ApplV_C2C_Epidemic") == 0)
   	    {
            Timer2 = new cMessage( "Timer_CRL_Interval_RSU", Timer_CRL_Interval_RSU );
            scheduleAt(simTime() + dblrand() * 10, Timer2);
   	    }
   	    // otherwise RSUs send beacon
   	    else
        {
            Timer1 = new cMessage( "Timer_Beacon_RSU", Timer_Beacon_RSU );
            scheduleAt(simTime() + dblrand() * beacon_Interval, Timer1);

            Timer3 = new cMessage( "Timer_Wait_Beacon_V", Timer_Wait_Beacon_V );
        }
    }
}


void ApplRSU::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
        if(msg->getKind()==Timer_CRL_Interval_RSU)
        {
            sendPeriodicCRL();
        }
        else if(msg->getKind()==Timer_Beacon_RSU)
        {
            sendBeacon();
        }
        else if(msg->getKind() == Timer_Wait_Beacon_V)
        {
            if(state == STATE_WAIT)
            {
                if( strcmp (applType, "ApplV_MPB") == 0 )
                {
                    if ( ev.isGUI() )
                    {
                        EV << "*** " << nodePtr->getFullName() << ": Waiting is over. Broadcasting pieces ..." << endl;
                        nodePtr->bubble("Broadcasting pieces");
                    }

                    sendCRLs();

                    // go back to state IDLE
                    state = STATE_IDLE;
                }
                else if( strcmp (applType, "ApplV_ICE") == 0 )
                {
                    if ( ev.isGUI() )
                    {
                        EV << "*** " << nodePtr->getFullName() << ": Waiting is over. Broadcasting pieces ..." << endl;
                        nodePtr->bubble("Broadcasting pieces");
                    }

                    sendCRLsMask();

                    AnyoneNeedCRL = false;

                    // reset the broadcastMask
                    for(int i=0; i<totalPieces;i++)
                        broadcastMask[i] = 0;

                    // go back to state IDLE
                    state = STATE_IDLE;
                }
                else if( strcmp (applType, "ApplV_ICEv2") == 0 )
                {
                    if ( ev.isGUI() )
                    {
                        EV << "*** " << nodePtr->getFullName() << ": Waiting is over. Broadcasting pieces ..." << endl;
                        nodePtr->bubble("Broadcasting pieces");
                    }

                    sendCRLsMaskv2();

                    AnyoneNeedCRL = false;

                    // reset the broadcastMask
                    for(int i=0; i<totalPieces;i++)
                        broadcastMask[i] = 0;

                    // go back to state IDLE
                    state = STATE_IDLE;
                }
            }
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
        // receive a beacon from vehicle
	    if(msg->getKind() == Msg_Beacon_V)
		{
		    recieveBeacon(msg);
		}
	    else delete msg;  // if receive a CRL from V, delete it
    }
	else if(msg->getArrivalGateId()==dataFromCA)
	{	
	    recieveCRL(msg);	
	}
	else
	{
	    EV << "Unknown message! -> delete, kind: " << msg->getKind() << endl;
	    delete msg;	
	}    
}


void ApplRSU::receiveSignal(cComponent *source, simsignal_t signalID, const char *string)
{
	Enter_Method_Silent();

	if( signalID == Signal_fromMAC )
	{
	    if( strcmp(string, "TxOver") == 0 )
	    {
		    EV << "**** " << nodePtr->getFullName() << ": MAC layer signals that the Msg is transmitted." << endl;
	    }
	    else if( strcmp(string, "RxError") == 0 )
	    {
		    EV << "**** " << nodePtr->getFullName() << ": MAC layer signals that the Msg is received with ERROR." << endl;
		    if ( ev.isGUI() ) nodePtr->bubble("Msg has error");
	    }
	}
}


// Receive CRL from the CA
void ApplRSU::recieveCRL(cMessage *msg)
{
    CRL_Piece *m = static_cast<CRL_Piece *>(msg);
	if (m == NULL) return;

	if ( ev.isGUI() )
	{
	    nodePtr->bubble("received a piece from CA");
	
        EV << "*** " << nodePtr->getFullName() << " received a CRL piece from CA:" << endl;

        EV << "Msg name: " << m->getName() << endl;
        EV << "CRL Version: " << m->getCRLversion() << endl;
        EV << "Timestamp: " << m->getTimestamp() << endl;
        EV << "Sequence Number: " << m->getSeqNo() << endl;
        EV << "CA ID: " << m->getCAid() << endl;
        EV << "Padding: " << m->getPad() << endl << endl;
	}

    // save this CRL piece for future periodic CRL sending
    PiecesCRLfromCA.push_back( m );
}


void ApplRSU::sendPeriodicCRL()
{
	if ( ev.isGUI() )
	{
	    nodePtr->bubble("Sending periodic CRL");
	    EV << "**** " << nodePtr->getFullName() << " is sending periodic CRL." << endl;
	}

	sendCRLs();

    // reset to false
    // AnyoneNeedCRL = false;

    scheduleAt(simTime() + CRL_Interval, Timer2);
}


// broadcast CRL pieces of the RSU
void ApplRSU::sendCRLs()
{
    if ( ev.isGUI() )
    {
        EV << "**** " << nodePtr->getFullName();
        EV << ": sending pieces on channel " << nicClassPtr->getChannel() << " --> ";
    }

    double time = 0;
    int counter = 0;

    for(unsigned int i=0; i<PiecesCRLfromCA.size(); i++)
    {
        CRL_Piece *pkt = PiecesCRLfromCA[forCounter]->dup();

        pkt->setName(nodePtr->getFullName());
        pkt->setKind(Msg_CRL_RSU);
        pkt->setBitLength(18400);
        pkt->setControlInfo( new NetwControlInfo(-1) );

        // we have time constrain (tho).
        // we have to calculate how many CRL pieces can we send in tho.
        // the calculation considers size of frame and data rate.
        // I2V_tho == 0 means we don't have any restriction.
        if(I2V_tho != 0)
        {
            // transmission time for a single frame
            // (32 bits header for network layer and 192 bits for data-link layer)
            double TxTime = ( pkt->getBitLength() + 32 + 272 + 192) / bitrate;
            time = time + TxTime;
            if(time >= I2V_tho)
            {
                delete pkt;
                break;
            }
        }

        counter++;
        EV << pkt->getSeqNo() << ", ";

        // send the pkt
        send(pkt, lowerLayerOut);

        forCounter++;
        if( forCounter == PiecesCRLfromCA.size() ) forCounter = 0;
    }

    EV << endl << counter << " CRL pieces out of " << PiecesCRLfromCA.size() << " are sent." << endl;

    // if one or more pieces sent
    if(counter > 0)
    {
        simsignal_t Signal_Broadcast_RSU = registerSignal("Broadcast_RSU");
        nodePtr->emit(Signal_Broadcast_RSU, 1);
    }
}


// broadcast CRL pieces of the RSU with respect to broadcastMask[i] in ICE
void ApplRSU::sendCRLsMask()
{
    if(!AnyoneNeedCRL)
    {
        EV << "**** " << nodePtr->getFullName() << " broadcasting canceled!" << endl;
        return;
    }

    if ( ev.isGUI() )
    {
        nodePtr->bubble("Sending CRLs");
        EV << "**** " << nodePtr->getFullName();
        EV << ": sending pieces on channel " << nicClassPtr->getChannel() << endl;

        EV << "broadcastMask is: ";

        for(int h=0; h<totalPieces; h++)
        {
            EV << broadcastMask[h] << ", ";
        }

        EV << endl;

        EV << "broadcast CRL pieces are: ";
    }

    double time = 0;
    int counter = 0;

    // iterate on all pieces in PiecesCRLfromCA
    // note that the pieces are not sorted by SeqNo
    for(unsigned int i=0; i<PiecesCRLfromCA.size(); i++)
    {
        // if corresponding bit in Mask variable is zero, then skip sending this piece
        if(broadcastMask[PiecesCRLfromCA[i]->getSeqNo()] == 0)
            continue;

        // create the packet for transmitting a certificate
        CRL_Piece *pkt = PiecesCRLfromCA[i]->dup();

        pkt->setName(nodePtr->getFullName());
        pkt->setKind(Msg_CRL_RSU);
        pkt->setBitLength(18400);
        pkt->setControlInfo( new NetwControlInfo(-1) );

        // we have time constrain (tho).
        // we have to calculate how many CRL pieces can we send in tho.
        // the calculation considers size of frame and data rate.
        // I2V_tho == 0 means we don't have any restriction.
        if(I2V_tho != 0)
        {
            // transmission time for a single frame
            // (32 bits header for network layer and 192 bits for data-link layer)
            double TxTime = ( pkt->getBitLength() + 32 + 272 + 192) / bitrate;
            time = time + TxTime;
            if(time >= I2V_tho)
            {
                delete pkt;
                break;
            }
        }

        counter++;
        EV << pkt->getSeqNo() << ", ";

        // send the pkt
        send(pkt, lowerLayerOut);
    }

    EV << endl << counter << " CRL pieces out of " << PiecesCRLfromCA.size() << " are sent." << endl;

    // if one or more pieces sent
    if(counter > 0)
    {
        simsignal_t Signal_Broadcast_RSU = registerSignal("Broadcast_RSU");
        nodePtr->emit(Signal_Broadcast_RSU, 1);
    }
}


// broadcast CRL pieces of the RSU with respect to broadcastMask[i] in ICEv2
void ApplRSU::sendCRLsMaskv2()
{
    if(!AnyoneNeedCRL)
    {
        EV << "**** " << nodePtr->getFullName() << " broadcasting canceled!" << endl;
        return;
    }

    if ( ev.isGUI() )
    {
        nodePtr->bubble("Sending CRLs");
        EV << "**** " << nodePtr->getFullName();
        EV << ": sending pieces on channel " << nicClassPtr->getChannel() << endl;

        EV << "broadcastMask in RSU is: ";

        for(int h=0; h<totalPieces; h++)
        {
            EV << broadcastMask[h] << ", ";
        }

        EV << endl;

        EV << "broadcast CRL pieces are: ";
    }

    double time = 0;
    int counter = 0;

    // iterate on all elements of broadcastMask
    for(int i=0; i<totalPieces; i++)
    {
        // return the SeqNo of the piece with highest priority in broadcastMask
        int pieceIndex = Maximum();

        // if there is no maximum, go out of the loop
        if (pieceIndex == -1)
            break;

        // check to see if this piece is in the PiecesCRLcollected
        int result = IsExist(pieceIndex);

        if(result == -1)
            continue;

        // create the packet for transmitting a certificate
        CRL_Piece *pkt = PiecesCRLfromCA[result]->dup();

        pkt->setName(nodePtr->getFullName());
        pkt->setKind(Msg_CRL_RSU);
        pkt->setBitLength(18400);
        pkt->setControlInfo( new NetwControlInfo(-1) );

        // we have time constrain (tho).
        // we have to calculate how many CRL pieces can we send in tho.
        // the calculation considers size of frame and data rate.
        // I2V_tho == 0 means we don't have any restriction.
        if(I2V_tho != 0)
        {
            // transmission time for a single frame
            // (32 bits header for network layer and 192 bits for data-link layer)
            double TxTime = ( pkt->getBitLength() + 32 + 272 + 192) / bitrate;
            time = time + TxTime;
            if(time >= I2V_tho)
            {
                delete pkt;
                break;
            }
        }

        counter++;
        EV << pkt->getSeqNo() << ", ";

        // send the pkt
        send(pkt, lowerLayerOut);
    }

    EV << endl << counter << " CRL pieces out of " << PiecesCRLfromCA.size() << " are sent." << endl;

    // if one or more pieces sent
    if(counter > 0)
    {
        simsignal_t Signal_Broadcast_RSU = registerSignal("Broadcast_RSU");
        nodePtr->emit(Signal_Broadcast_RSU, 1);
    }

}


// receive beacon from vehicles
void ApplRSU::recieveBeacon(cMessage *msg)
{
    Beacon *m = static_cast<Beacon *>(msg);
    if (m == NULL) return;

    if(ev.isGUI())
    {
        EV << "*** " << nodePtr->getFullName() << " received a beacon from " << m->getNodeName() << endl;
        nodePtr->bubble("received a beacon");

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

    // if the algorithm is MPB
    if( strcmp (applType, "ApplV_MPB") == 0 )
    {
        if(state == STATE_IDLE)
        {
            // RSU waits for 1s, to send a beacon to the vehicle
            // and by this the vehicle is notified about the piece_count of RSU
            scheduleAt(simTime() + 1, Timer3);

            EV << endl << "*** " << nodePtr->getFullName() << ": Waiting ..." << endl;
            state = STATE_WAIT;
        }
    }
    // if the algorithm is ICE
    else if( strcmp (applType, "ApplV_ICE") == 0 )
    {
        // update AnyoneNeedCRL
        if (!AnyoneNeedCRL)
        {
            if(m->getRangeE() - m->getRangeS() == -1)
                AnyoneNeedCRL = false;
            else
                AnyoneNeedCRL = true;
        }

        if(state == STATE_IDLE)
        {
            // update the broadcastMask
            for(int i = m->getRangeS(); i <= m->getRangeE(); i++)
                broadcastMask[i] = 1;

            // RSU waits for 1s, to send a beacon to the vehicle
            // and by this the vehicle is notified about the range of RSU
            scheduleAt(simTime() + 1, Timer3);

            EV << endl << "*** " << nodePtr->getFullName() << ": Waiting ..." << endl;
            state = STATE_WAIT;
        }
        else if(state == STATE_WAIT)
        {
            // update the broadcastMask
            for(int i = m->getRangeS(); i <= m->getRangeE(); i++)
                broadcastMask[i] = 1;
        }
    }
    // if the algorithm is ICEv2
    if( strcmp (applType, "ApplV_ICEv2") == 0 )
    {
        // update AnyoneNeedCRL
        if (!AnyoneNeedCRL)
        {
            if(m->getRangeE() - m->getRangeS() == -1)
                AnyoneNeedCRL = false;
            else
                AnyoneNeedCRL = true;
        }

        if(state == STATE_IDLE)
        {
            // update the broadcastMask
            for(int i = m->getRangeS(); i <= m->getRangeE(); i++)
                broadcastMask[i] = broadcastMask[i] + 1;

            // RSU waits for 1s, to send a beacon to the vehicle
            // and by this the vehicle is notified about the range of RSU
            scheduleAt(simTime() + 1, Timer3);

            EV << endl << "*** " << nodePtr->getFullName() << ": Waiting ..." << endl;
            state = STATE_WAIT;
        }
        else if(state == STATE_WAIT)
        {
            // update the broadcastMask
            for(int i = m->getRangeS(); i <= m->getRangeE(); i++)
                broadcastMask[i] = broadcastMask[i] + 1;
        }
    }

    delete msg;
}


// send beacon (only in MPB and ICE)
void ApplRSU::sendBeacon()
{
    EV << "**** " << nodePtr->getFullName() << " is sending a broadcast beacon." << endl;
    if ( ev.isGUI() ) nodePtr->bubble("Sending beacon");

    simsignal_t Signal_Beacon_RSU = registerSignal("Beacon_RSU");
    nodePtr->emit(Signal_Beacon_RSU, 1);

    Beacon *pkt = new Beacon(nodePtr->getFullName(), Msg_Beacon_RSU);

    pkt->setSrcAddr(nodeID);
    pkt->setDestAddr(-1);   // its a broadcast beacon

    pkt->setNodeName(nodePtr->getFullName());

    // get and then set the current position of the node
    int xPos = std::atoi( nodePtr->getDisplayString().getTagArg("p",0) );  // x-coordinate of node
    int yPos = std::atoi( nodePtr->getDisplayString().getTagArg("p",1) );  // y-coordinate of node
    pkt->setPositionX( xPos );
    pkt->setPositionY( yPos );

    pkt->setSpeed(0);

    pkt->setNeedCRL(false);
    pkt->setCRL_Piece_No(totalPieces);

    pkt->setRangeS(totalPieces);
    pkt->setRangeE(totalPieces-1);

    pkt->setCRLchannel(-1);

    pkt->setBitLength(1000);
    pkt->setControlInfo( new NetwControlInfo(-1) );

    send(pkt, lowerLayerOut);

    scheduleAt(simTime() + beacon_Interval, Timer1);
}


// return the index of maximum value
int ApplRSU::Maximum()
{
    int Max = 0;
    int index = -1;

    for(int i=0; i<totalPieces; i++)
    {
        if(broadcastMask[i] > Max)
        {
            Max = broadcastMask[i];
            index = i;
        }
    }

    // all elements of broadcastMask are 0
    if(Max == 0)
        return -1;
    else
    {
        broadcastMask[index] = 0;
        return index;
    }
}


int ApplRSU::IsExist(int index)
{
    bool found = false;
    unsigned int j = 0;

    for(j=0; j<PiecesCRLfromCA.size(); j++)
    {
        if(PiecesCRLfromCA[j]->getSeqNo() == index)
        {
            found = true;
            break;
        }
    }

    if(found)
        return j;
    else
        return -1;
}


void ApplRSU::finish()
{


}


ApplRSU::~ApplRSU()
{
    if(strcmp (applType, "ApplV_MPB") == 0 || strcmp (applType, "ApplV_ICE") == 0 || strcmp (applType, "ApplV_ICEv2") == 0)
    {
        cancelAndDelete(Timer1);
        cancelAndDelete(Timer3);
    }
    else
    {
        cancelAndDelete(Timer2);
    }

    for(unsigned int i=0; i<PiecesCRLfromCA.size(); i++)
    {
    	delete PiecesCRLfromCA[i];
    }
}

