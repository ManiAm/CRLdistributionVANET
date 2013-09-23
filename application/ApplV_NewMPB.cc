#include <ApplV_NewMPB.h>


Define_Module(ApplV_NewMPB);

// constructor
ApplV_NewMPB::ApplV_NewMPB()
{

}


void ApplV_NewMPB::initialize(int stage)
{
	ApplV::initialize(stage);

    if(stage == 0)
	{
    	Isitme = true;
		maxCRL = 0;
	    maxChannel = 0;

    	PreChannel = 0;
    	numFrames = 0;

        Timer4 = new cMessage( "Timer_Wait_Beacon_V", Timer_Wait_Beacon_V );
        Timer5 = new cMessage( "Timer_Wait_CRL_Over_V", Timer_Wait_CRL_Over_V );
    }
}


void ApplV_NewMPB::handleMessage(cMessage *msg)
{
	ApplV::handleMessage(msg);

	if (msg->isSelfMessage())
	{
        if ( msg->getKind() == Timer_Wait_Beacon_V )
        {
        	whoShouldSendCRL();
        }
        else if( msg ->getKind() == Timer_Wait_CRL_Over_V )
        {
        	waitingCRLOver();
        }
    }
}


void ApplV_NewMPB::receiveSignal(cComponent *source, simsignal_t signalID, const char *string)
{
	Enter_Method_Silent();

	ApplV::receiveSignal(source, signalID, string);

	if(signalID == Signal_fromMAC)
	{
	    if( strcmp(string, "TxOver") == 0 )
	    {
		    if(state == STATE_UNICAST_BEACON_SENDING)
		    {
			    EV << "**** " << nodePtr->getFullName() << ": MAC layer signals that the unicast beacon has sent." << endl;
			    state = STATE_UNICAST_BEACON_SEND;

		        EV << "waiting for more beacons ..." << endl;
	            scheduleAt(simTime() + V2V_Wait_Beacon, Timer4);
		    }
		    else if(state == STATE_SENDING_CRLS)
		    {
		    	EV << "**** " << nodePtr->getFullName() << ": MAC layer signals that a CRL has been sent." << endl;

		    	numFrames++;

		    	if(numFrames == totFrames)
		    	{
		    		EV << "**** " << nodePtr->getFullName() << ": all CRLs have been sent." << endl;

                    numFrames = 0;
                    totFrames = 0;

                    waitingCRLOver();
		    	}
		    }
	    }
	}
}


void ApplV_NewMPB::Run_V2V_Algorithm(Beacon *m)
{
    if( m->getCRL_Piece_No() == 0 && PiecesCRLcollected.size() == 0 )
        return;

    EV << endl << "*** run the \"V2V Most Pieces Broadcast (New)\" algorithm: " << endl;

	// three types of beacons can be received.
	// 1. broadcast beacon
	// 2. unicast beacon: it is send to us
	// 3. unicast beacon (not-mine): it is send to another node, but we listen

	// type1: broadcast beacon received
	if(m->getDestAddr() == -1)
	{
		if(state == STATE_IDLE || state == STATE_BROADCAST_BEACON_SENDING || state == STATE_BROADCAST_BEACON_SEND)
		{
			whoHasMoreCRL(m);

	        cancelEvent(Timer1);   // cancel current periodic beaconing temporally

	        EV << "sending a unicast beacon in reply." << endl;

	        state = STATE_UNICAST_BEACON_SENDING;

	        sendBeacon(m->getSrcAddr(), false);   // disable future periodic beacon temporarily

	        // now we will wait till unicast beacon is sent completely.
	        // go to receiveSignal method.
		}
	}
	// type2: unicast beacon received
	else if(m->getDestAddr() == nodeID)
	{
    	if( state == STATE_BROADCAST_BEACON_SEND )
    	{
            whoHasMoreCRL(m);

		    cancelEvent(Timer1);   // cancel current periodic beaconing temporally

		    EV << "waiting for more beacons ..." << endl;
	        scheduleAt(simTime() + V2V_Wait_Beacon, Timer4);

		    state = STATE_WAIT_FOR_MORE_BEACONS;
    	}
    	else if( state == STATE_WAIT_FOR_MORE_BEACONS )
    	{
    		whoHasMoreCRL(m);
    	}
	}
	// type3: unicast beacon received (not-mine)
	else if( m->getDestAddr() != -1 && m->getDestAddr() != nodeID )
	{
		// we only respond to unicast beacons (not-mine) when we
		// have send the unicast beacon. otherwise we ignore it !!
		if(state == STATE_UNICAST_BEACON_SEND)
		{
		    whoHasMoreCRL(m);
		}
	}
}


void ApplV_NewMPB::whoHasMoreCRL(Beacon *m)
{
	EV << "number of CRLs for me: " << PiecesCRLcollected.size() << endl;
	EV << "number of CRLs in partner: " << m->getCRL_Piece_No() << endl;

	if(maxCRL == 0)
	{
		maxCRL = PiecesCRLcollected.size();
	}

	// note: if both nodes, have the same number of CRLs, they both will
	// send their CRLs. If they both listen, we will have problem.
	if( m->getCRL_Piece_No() > maxCRL )
	{
		EV << "my partner has more CRL pieces --> ";

		maxCRL = m->getCRL_Piece_No();
	    maxChannel = m->getCRLchannel();
	    Isitme = false;

		EV << "update maxCRL to " << maxCRL << " and maxXhannel to " << maxChannel << endl;
	}
}


void ApplV_NewMPB::whoShouldSendCRL()
{
	EV << "**** " << nodePtr->getFullName() << ": whoShouldSendCRL ?" << endl;

    // I have less CRLs, so I should be silent and listen
	if(!Isitme)
	{
	    // change the channel only if V2V_EnableMultiChannel in enabled
	    if(V2V_EnableMultiChannel)
	    {
	        // save the current channel temporarily
	        PreChannel = nicClassPtr->getChannel();

			EV << "**** " << nodePtr->getFullName() << ": switching to channel " << maxChannel << endl;
	        nicClassPtr->switchChannel(maxChannel);
	    }

		EV << "**** " << nodePtr->getFullName() << ": is listening on channel " << nicClassPtr->getChannel() << " ..." << endl;
		state = STATE_WAITING_CRL;

		// waiting for some time and then call waitingCRLOver method
		scheduleAt(simTime() + V2V_Wait_CRL, Timer5);
	}
	// I have more CRLs and should send them to others
	else
	{
	    // change the channel only if V2V_EnableMultiChannel in enabled
	    if(V2V_EnableMultiChannel)
	    {
	        // save the current channel temporarily
	        PreChannel = nicClassPtr->getChannel();

			EV << "**** " << nodePtr->getFullName() << ": switching to channel " << CRLChannel << endl;
	        nicClassPtr->switchChannel(CRLChannel);
	    }

		// send the CRLs
		sendCRLs();

		state = STATE_SENDING_CRLS;

		// we will wait till all CRLs be sent completely.
		// check receiveSignal method.
	}
}


void ApplV_NewMPB::waitingCRLOver()
{
    // change the channel back to normal
    if(V2V_EnableMultiChannel)
    {
        EV << "switching back to channel " << PreChannel << endl;
        nicClassPtr->switchChannel(PreChannel);
    }

    // reset the variables
    maxCRL = 0;
    maxChannel = 0;
    Isitme = true;

    state = STATE_IDLE;
    scheduleAt(simTime() + beacon_Interval, Timer1);
}


// broadcast CRL pieces of the vehicle
void ApplV_NewMPB::sendCRLs()
{
    if ( ev.isGUI() )
    {
        nodePtr->bubble("Sending CRLs");
        EV << "**** " << nodePtr->getFullName();
        EV << ": sending pieces on channel " << nicClassPtr->getChannel() << " -> ";
    }

    double time = 0;
    int counter = 0;

    for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
    {
        // forCounter variable is defined as a private variable in ApplV.h.
        // by this, its value will remain in every function call.
        CRL_Piece *pkt = PiecesCRLcollected[forCounter]->dup();

        pkt->setName(nodePtr->getFullName());
        pkt->setKind(Msg_CRL_V);
        pkt->setBitLength(18400);
        pkt->setControlInfo( new NetwControlInfo(-1) );

        // we have time constrain (tho).
        // we have to calculate how many CRL pieces can we send in tho.
        // the calculation considers size of frame and data rate.
        // I2V_tho == 0 means we don't have any restriction.
        if(V2V_tho != 0)
        {
            // transmission time for a single frame
            // (32 bits header for network layer and 192 bits for data-link layer)
            double TxTime = ( pkt->getBitLength() + 32 + 272 + 192) / bitrate;
            time = time + TxTime;
            if(time >= V2V_tho)
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
        if( forCounter == PiecesCRLcollected.size() ) forCounter = 0;
    }

    EV << endl << counter << " CRL pieces out of " << PiecesCRLcollected.size() << " are sent." << endl;

    // total number of frames that were sent.
    // we are sure that CRL_Collected.size > 0 so i > 0 (this is not true in BBroadcast).
    totFrames = counter;
}


void ApplV_NewMPB::finish()
{


}


ApplV_NewMPB::~ApplV_NewMPB()
{
	cancelAndDelete(Timer4);
	cancelAndDelete(Timer5);
}

