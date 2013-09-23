#include <ApplV_MPB.h>


Define_Module(ApplV_MPB);

// constructor
ApplV_MPB::ApplV_MPB()
{

}


void ApplV_MPB::initialize(int stage)
{
	ApplV::initialize(stage);

    if(stage == 0)
	{
        counter = 0;

        Timer4 = new cMessage( "Timer_Wait_Beacon_V", Timer_Wait_Beacon_V );
        Timer5 = new cMessage( "Timer_Wait_CRL_Over_V", Timer_Wait_CRL_Over_V );
    }
}


void ApplV_MPB::handleMessage(cMessage *msg)
{
	ApplV::handleMessage(msg);

	if (msg->isSelfMessage())
	{
        if ( msg->getKind() == Timer_Wait_Beacon_V )
            checkCounter();
        else if( msg->getKind() == Timer_Wait_CRL_Over_V )
            waitForCRLover();
    }
}


void ApplV_MPB::receiveSignal(cComponent *source, simsignal_t signalID, const char *string)
{
	Enter_Method_Silent();

	// ApplV::receiveSignal(source, signalID, string);

}


void ApplV_MPB::Run_V2V_Algorithm(Beacon *m)
{
    EV << endl << "*** run the \"V2V MPB\" algorithm: " << endl;

    // we received broadcast beacon m

    if(state == STATE_IDLE)
    {
        update(m);

        EV << "wait for more beacons ..." << endl;

        state = STATE_WAIT_FOR_MORE_BEACONS;
        AnyPieceReceived = false;

        // wait for 1s to receive beacons from nearby vehicles.
        // we choose 1s, because it is the maximum value for Beacon_Interval.
        scheduleAt(simTime() + 1, Timer4);
    }
    else if(state == STATE_WAIT_FOR_MORE_BEACONS)
        update(m);
}


void ApplV_MPB::update(Beacon *m)
{
    // update counter variable
    if( (unsigned int)m->getCRL_Piece_No() > PiecesCRLcollected.size() )
        counter++;
}


void ApplV_MPB::checkCounter()
{
    if(state != STATE_WAIT_FOR_MORE_BEACONS)
        return;

    EV << "wait for more beacons is over! ..." << endl;

    if(counter == 0)
    {
        state = STATE_SENDING_CRLS;

        // we broadcast all CRL pieces.
        sendCRLs();

        counter = 0;
        state = STATE_IDLE;
    }
    else if(counter > 0)
    {
        EV << "listening to CRL broadcast ..." << endl;

        state = STATE_WAITING_CRL;

        // wait for 0.05 to see any pieces will be received or not?
        scheduleAt(simTime() + 0.05, Timer5);
    }
}


void ApplV_MPB::waitForCRLover()
{
    if(state != STATE_WAITING_CRL)
        return;

    EV << "listening to CRL broadcast is over ..." << endl;

    // if no pieces are received
    if(!AnyPieceReceived)
    {
        // we broadcast all CRL pieces.
        sendCRLs();
    }

    counter = 0;
    state = STATE_IDLE;
}


// broadcast CRL pieces of the vehicle
void ApplV_MPB::sendCRLs()
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


void ApplV_MPB::finish()
{


}


ApplV_MPB::~ApplV_MPB()
{
	cancelAndDelete(Timer4);
	cancelAndDelete(Timer5);
}

