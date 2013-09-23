#include <ApplV_ICE.h>


Define_Module(ApplV_ICE);

// constructor
ApplV_ICE::ApplV_ICE()
{

}


void ApplV_ICE::initialize(int stage)
{
    ApplV::initialize(stage);

    if(stage == 0)
    {
        counter = 0;

        broadcastMask = new int[totalPieces];
        for (int i=0; i<totalPieces; i++)
            broadcastMask[i] = 0;    // Initialize all elements to zero.

        Timer4 = new cMessage( "Timer_Wait_Beacon_V", Timer_Wait_Beacon_V );
        Timer5 = new cMessage( "Timer_Wait_CRL_Over_V", Timer_Wait_CRL_Over_V );
    }
}


void ApplV_ICE::handleMessage(cMessage *msg)
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


void ApplV_ICE::receiveSignal(cComponent *source, simsignal_t signalID, const char *string)
{
    Enter_Method_Silent();

    // ApplV::receiveSignal(source, signalID, string);

}


void ApplV_ICE::Run_V2V_Algorithm(Beacon *m)
{
    EV << endl << "*** run the \"V2V ICE\" algorithm: " << endl;

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


void ApplV_ICE::update(Beacon *m)
{
    // update the broadcastMask
    for(int i = m->getRangeS(); i <= m->getRangeE(); i++)
        broadcastMask[i] = 1;

    /*
    // update counter variable
    if( (unsigned int)m->getCRL_Piece_No() > PiecesCRLcollected.size() )
        counter++;
    */

    // update counter variable
    if( (m->getRangeE() - m->getRangeS()) < (rangeEnd - rangeStart) )
        counter++;
}


void ApplV_ICE::checkCounter()
{
    if(state != STATE_WAIT_FOR_MORE_BEACONS)
        return;

    EV << "wait for more beacons is over! ..." << endl;

    if(counter == 0)
    {
        state = STATE_SENDING_CRLS;

        // we broadcast all CRL pieces.
        sendCRLsMask();

        // reset the broadcastMask
        for(int i=0; i<totalPieces;i++)
            broadcastMask[i] = 0;

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


void ApplV_ICE::waitForCRLover()
{
    if(state != STATE_WAITING_CRL)
        return;

    EV << "listening to CRL broadcast is over ..." << endl;

    // if no pieces are received from V2V communication
    if(!AnyPieceReceived)
    {
        // we broadcast all CRL pieces.
        sendCRLsMask();
    }

    // reset the broadcastMask
    for(int i=0; i<totalPieces;i++)
        broadcastMask[i] = 0;

    counter = 0;
    state = STATE_IDLE;
}


// broadcast CRL pieces of the vehicle with respect to broadcastMask[i]
void ApplV_ICE::sendCRLsMask()
{
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

    // iterate on all pieces in PiecesCRLcollected
    // note that the pieces are not sorted by SeqNo
    for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
    {
        // if corresponding bit in Mask variable is zero, then skip sending this piece
        if(broadcastMask[PiecesCRLcollected[i]->getSeqNo()] == 0)
            continue;

        // create the packet for transmitting a certificate
        CRL_Piece *pkt = PiecesCRLcollected[i]->dup();

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
    }

    EV << endl << counter << " CRL pieces out of " << PiecesCRLcollected.size() << " are sent." << endl;
}


void ApplV_ICE::updateRange()
{
    EV << "Updating range. ";

    // if we receive all the pieces
    if(PiecesCRLcollected.size() == (unsigned int)totalPieces)
    {
        // in this case rangeStart surpassed rangeEnd, thus
        // rangeEnd - rangeStart is negative.
        rangeStart = totalPieces;
        rangeEnd = totalPieces-1;

        EV << "rangeStart = " << rangeStart << ", rangeEnd =" << rangeEnd << endl;

        return;
    }

    int i;

    // look for rangeStart (the lowest index pointing to the missing piece)
    for(i=0; i<totalPieces-1;i++)
    {
        bool found = false;

        for(unsigned int j=0; j<PiecesCRLcollected.size();j++)
        {
            if(i == PiecesCRLcollected[j]->getSeqNo())
            {
                found = true;
                break;
            }
        }

        if(!found)
            break;
    }

    rangeStart = i;

    // now look for rangeEnd (the highest index pointing to the missing piece)
    for(i=totalPieces-1; i>=0;i--)
    {
        bool found = false;

        for(unsigned int j=0; j<PiecesCRLcollected.size();j++)
        {
            if(i == PiecesCRLcollected[j]->getSeqNo())
            {
                found = true;
                break;
            }
        }

        if(!found)
            break;
    }

    rangeEnd = i;

    EV << "rangeStart = " << rangeStart << ", rangeEnd =" << rangeEnd << endl;
}


void ApplV_ICE::finish()
{


}


ApplV_ICE::~ApplV_ICE()
{
    cancelAndDelete(Timer4);
    cancelAndDelete(Timer5);

    delete broadcastMask;
}

