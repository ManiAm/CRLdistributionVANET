#include <ApplV_BBroadcast.h>


Define_Module(ApplV_BBroadcast);

// constructor
ApplV_BBroadcast::ApplV_BBroadcast()
{


}


void ApplV_BBroadcast::initialize(int stage)
{
	ApplV::initialize(stage);

    if(stage == 0)
	{
    	PreChannel = 0;
    	numFrames = 0;

    	Timer4 = new cMessage( "Timer_Wait_CRL_Over_V", Timer_Wait_CRL_Over_V );
    	Timer5 = new cMessage( "Timer_Wait_CRL_Over_V_2", Timer_Wait_CRL_Over_V_2 );
    }
}


void ApplV_BBroadcast::handleMessage(cMessage *msg)
{
	ApplV::handleMessage(msg);

	if (msg->isSelfMessage())
	{
        if( msg->getKind() == Timer_Wait_CRL_Over_V && state == STATE_WAITING_CRL)
        {
        	EV << "**** " << nodePtr->getFullName() << ": waiting for CRLs is over." << endl;

    		// change the channel back to normal
			EV << "**** " << nodePtr->getFullName() << ": switching back to channel " << PreChannel << endl;
	        nicClassPtr->switchChannel(PreChannel);

		    state = STATE_IDLE;
		    scheduleAt(simTime() + beacon_Interval, Timer1);
        }
        else if( msg->getKind() == Timer_Wait_CRL_Over_V_2 && state == STATE_WAITING_CRL_2)
        {
        	EV << "**** " << nodePtr->getFullName() << ": waiting for CRLs is over." << endl;

		    if(PiecesCRLcollected.size() > 0)
		    {
			    // save the current channel temporarily
			    PreChannel = nicClassPtr->getChannel();

				EV << "**** " << nodePtr->getFullName() << ": switching to channel " << DesCh << endl;
			    nicClassPtr->switchChannel(DesCh);

		        // send the CRLs
		        sendCRLs();

			    state = STATE_SENDING_CRLS;
		    }
		    // we have no CRLs
		    else
		    {
	    		EV << "**** " << nodePtr->getFullName() << ": I have no CRL !!" << endl;

	    		// change the channel back to normal
				EV << "**** " << nodePtr->getFullName() << ": switching back to channel " << PreChannel << endl;
		        nicClassPtr->switchChannel(PreChannel);

                numFrames = 0;
                totFrames = 0;

    		    scheduleAt(simTime() + beacon_Interval, Timer1);

    		    state = STATE_IDLE;
		    }
        }
    }
}


void ApplV_BBroadcast::receiveSignal(cComponent *source, simsignal_t signalID, const char *string)
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

			    // we will wait till all CRLs be sent completely.
			    if(PiecesCRLcollected.size() > 0)
			    {
				    // save the current channel temporarily
				    PreChannel = nicClassPtr->getChannel();

					EV << "**** " << nodePtr->getFullName() << ": switching to channel " << CRLChannel << endl;
				    nicClassPtr->switchChannel(CRLChannel);

                    sendCRLs();

    			    state = STATE_UNICAST_BEACON_SEND;
			    }
			    // we have no CRLs
			    else
			    {
		    		EV << "**** " << nodePtr->getFullName() << ": I have no CRL !!" << endl;

			        // save the current channel temporarily
		            PreChannel = nicClassPtr->getChannel();

					EV << "**** " << nodePtr->getFullName() << ": switching to channel " << SrcCh << endl;
			        nicClassPtr->switchChannel(SrcCh);

		            EV << "waiting for CRLs ..." << endl;

		            // stay in this channel for Wait_CRL.
		            scheduleAt(simTime() + V2V_Wait_CRL, Timer4);

		            state = STATE_WAITING_CRL;
			    }
		    }
		    else if(state == STATE_UNICAST_BEACON_SEND)
		    {
		    	EV << "**** " << nodePtr->getFullName() << ": MAC layer signals that a CRL has been sent." << endl;

		    	numFrames++;

		    	if(numFrames == totFrames)
		    	{
		    		EV << "**** " << nodePtr->getFullName() << ": all CRLs have been sent." << endl;

                    numFrames = 0;
                    totFrames = 0;

			        // save the current channel temporarily
		            PreChannel = nicClassPtr->getChannel();

					EV << "**** " << nodePtr->getFullName() << ": switching to channel " << SrcCh << endl;
			        nicClassPtr->switchChannel(SrcCh);

		            EV << "waiting for CRLs ..." << endl;

		            // stay in this channel for Wait_CRL.
		            scheduleAt(simTime() + V2V_Wait_CRL, Timer4);

		            state = STATE_WAITING_CRL;

		            // now we will wait for the timer4 to expire.
		            // check handleMessage method.
		    	}
		    }
		    else if(state == STATE_WAITING_CRL)
		    {
		    	EV << "**** " << nodePtr->getFullName() << ": MAC layer signals that a CRL has been sent." << endl;

		    	numFrames++;

		    	if(numFrames == totFrames)
		    	{
		    		EV << "**** " << nodePtr->getFullName() << ": all CRLs have been sent." << endl;

		    		// change the channel back to normal
					EV << "**** " << nodePtr->getFullName() << ": switching back to channel " << PreChannel << endl;
			        nicClassPtr->switchChannel(PreChannel);

                    numFrames = 0;
                    totFrames = 0;

        		    scheduleAt(simTime() + beacon_Interval, Timer1);

        		    state = STATE_IDLE;
		    	}
		    }
	    }
	}
}


void ApplV_BBroadcast::Run_V2V_Algorithm(Beacon *m)
{
    EV << endl << "*** run the \"V2V BBroadcast\" algorithm: " << endl;

	// if multi-channel is enabled.
	if (V2V_EnableMultiChannel)
	{
		//Run_V2V_Algorithm_MultiChannel(m);
		//return;
	}

    // broadcast beacon received
	if(m->getDestAddr() == -1)
    {
		// check if we are at correct state
		if(state == STATE_IDLE || state == STATE_BROADCAST_BEACON_SENDING || state == STATE_BROADCAST_BEACON_SEND)
		{
		    // cancel current periodic beaconing temporally
	        cancelEvent(Timer1);

	        // send CRLs in channel 1
	        sendCRLs();

	        // we don't wait for all CRLs to be send. Immediately send a unicast beacon.
	        // because CRLs and beacons are send in channel 1.
            EV << "sending a unicast beacon in reply." << endl;
            sendBeacon(m->getSrcAddr(), false);

            state = STATE_IDLE;
            scheduleAt(simTime() + beacon_Interval, Timer1);
		}
    }
	// unicast beacon received
	else if(m->getDestAddr() == nodeID)
	{
		// check if we are at correct state
		if(state == STATE_IDLE || state == STATE_BROADCAST_BEACON_SENDING || state == STATE_BROADCAST_BEACON_SEND)
		{
		    // cancel current periodic beaconing temporally
	        cancelEvent(Timer1);

	        // send CRLs in channel 1
	        sendCRLs();

            state = STATE_IDLE;
            scheduleAt(simTime() + beacon_Interval, Timer1);
		}
	}
}


void ApplV_BBroadcast::Run_V2V_Algorithm_MultiChannel(Beacon *m)
{
    // broadcast beacon received
	if(m->getDestAddr() == -1)
	{
		// check if we are at correct state
		if(state == STATE_IDLE || state == STATE_BROADCAST_BEACON_SENDING || state == STATE_BROADCAST_BEACON_SEND)
		{
		    // cancel current periodic beaconing temporally
	        cancelEvent(Timer1);

            EV << "sending a unicast beacon in reply." << endl;

            state = STATE_UNICAST_BEACON_SENDING;

	        // we send a unicast beacon to report our channel to the other node
            // disable future periodic beacon temporarily
            sendBeacon(m->getSrcAddr(), false);

            // save the channel of source for future use
            SrcCh = m->getCRLchannel();

            // now we will wait till unicast beacon is sent completely.
            // go to receiveSignal method.
		}
	}
	// unicast beacon received
	else if(m->getDestAddr() == nodeID)
	{
		// check if we are at correct state
		if(state == STATE_IDLE || state == STATE_BROADCAST_BEACON_SENDING || state == STATE_BROADCAST_BEACON_SEND)
		{
		    cancelEvent(Timer1);   // cancel current periodic beaconing temporally

            // save the current channel temporarily
            PreChannel = nicClassPtr->getChannel();

		    EV << "switching to channel " << m->getCRLchannel() << endl;
            nicClassPtr->switchChannel(m->getCRLchannel());

            EV << "waiting for CRLs ..." << endl;

            // stay in this channel for Wait_CRL.
            scheduleAt(simTime() + V2V_Wait_CRL, Timer5);

            state = STATE_WAITING_CRL_2;

            // save the channel of destination for future use
            DesCh = m->getCRLchannel();

            // now we will wait for the timer5 to expire.
            // check handleMessage method.
		}
	}
}

// broadcast CRL pieces of the vehicle
void ApplV_BBroadcast::sendCRLs()
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
        // create the packet for transmitting a certificate
        CRL_Piece *pkt = new CRL_Piece(nodePtr->getFullName(), Msg_CRL_V);

        // forCounter variable is defined as a private variable in ApplV.h.
        // by this, its value will remain in every function call.

        pkt->setCRLversion(1);
        pkt->setTimestamp(0);
        pkt->setSeqNo(PiecesCRLcollected[forCounter]->getSeqNo());
        pkt->setCAid(23);
        pkt->setBitLength(18400);
        pkt->setPayload( PiecesCRLcollected[forCounter]->getPayload() );
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


void ApplV_BBroadcast::finish()
{


}


ApplV_BBroadcast::~ApplV_BBroadcast()
{
	cancelAndDelete(Timer4);
	cancelAndDelete(Timer5);

}
