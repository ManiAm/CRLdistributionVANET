#include <ApplV_C2C_Epidemic.h>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

Define_Module(ApplV_C2C_Epidemic);

// constructor
ApplV_C2C_Epidemic::ApplV_C2C_Epidemic()
{

}
 

void ApplV_C2C_Epidemic::initialize(int stage)
{
	ApplV::initialize(stage);

    if(stage == 0)
	{
        broadcastMask = new int[totalPieces];
        for (int i=0; i<totalPieces; i++)
            broadcastMask[i] = 1;    // Initialize all elements to 1

        Timer4 = new cMessage( "Timer_Wait_CRL_Over_V", Timer_Wait_CRL_Over_V );
        Timer5 = new cMessage( "Timer_Wait_PList", Timer_Wait_PList);
    }
}


void ApplV_C2C_Epidemic::handleMessage(cMessage *msg)
{
	ApplV::handleMessage(msg);

	if(msg->isSelfMessage())
	{
        if( msg->getKind() == Timer_Wait_CRL_Over_V )
        {
            EV << "*** " << nodePtr->getFullName() << ": waiting is over." << endl;

            // waiting time for receiving the broadcast pieces has expired.
            // we can broadcast our own pieces now.
            if(state == STATE_WAITING_CRL)
            {
                // send the incremental-CRL update to the other node
                sendCRLsMask();

                // reset the broadcastMask
                for(int i=0; i<totalPieces;i++)
                    broadcastMask[i] = 1;

                // reset the state back to idle
                state = STATE_IDLE;
            }
        }
        // we did not receive any corresponding PList msg
        else if( msg->getKind() == Timer_Wait_PList )
        {
            state = STATE_IDLE;
        }
	}
	else if(msg->getArrivalGateId() == lowerLayerIn)
	{
        if(msg->getKind() == Msg_Pieces_list)
        {
            EV << "*** " << nodePtr->getFullName() << " received a list of pieces." << endl;

            Piece_list *m = static_cast<Piece_list *>(msg);
            if(m == NULL)
                return;

            if(state == STATE_CRL_LIST_SEND)
            {
                // first check if the serial matches or not
                if(serial != m->getSerial())
                    return;

                EV << "*** " << nodePtr->getFullName() << " received the corresponding PList msg." << endl;
                cancelEvent(Timer5);

                // Update the broadcastMask array
                UpdateMask(m);

                // send the incremental-CRL update to the other node
                sendCRLsMask();

                // reset the broadcastMask
                for(int i=0; i<totalPieces;i++)
                    broadcastMask[i] = 1;

                // reset the state back to idle
                state = STATE_IDLE;
            }
            else if(state == STATE_IDLE)
            {
                // Update the broadcastMask array
                UpdateMask(m);

                // send my own list
                Send_CRL_List_Msg( m->getSerial());

                state = STATE_WAITING_CRL;

                // wait for a while to receive the broadcast pieces,
                // and then we will broadcast our pieces
                double WaitingTime = ((18400 + 32 + 272 + 192) / bitrate) * totalPieces;
                scheduleAt(simTime() + WaitingTime, Timer4);

                EV << "*** " << nodePtr->getFullName() << " is waiting for " << WaitingTime << " ..." << endl;
            }

            delete m;
        }
	}
}


void ApplV_C2C_Epidemic::receiveSignal(cComponent *source, simsignal_t signalID, const char *string)
{
	Enter_Method_Silent();

	ApplV::receiveSignal(source, signalID, string);
}


void ApplV_C2C_Epidemic::Run_V2V_Algorithm(Beacon *m)
{
    EV << endl << "*** run the \"V2V C2C Epidemic\" algorithm: " << endl;

    // we received a broadcast beacon
    if(state == STATE_IDLE)
    {
        bool WeNeedCRL;
        if( isMagicCar() || (PiecesCRLcollected.size() == (unsigned int)totalPieces) )
            WeNeedCRL = false;
        else
            WeNeedCRL = true;

        // check to see if the vehicle sending this beacon, or even we need any CRL.
        if( m->getNeedCRL() || WeNeedCRL)
        {
            // send my own list
            Send_CRL_List_Msg(-1);

            state = STATE_CRL_LIST_SEND;
            scheduleAt(simTime() + 0.1, Timer5);
        }
    }
}


void ApplV_C2C_Epidemic::Send_CRL_List_Msg(int rSerial)
{
    std::ostringstream oss;
    oss.str("");

    for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
    {
        oss << PiecesCRLcollected[i]->getSeqNo();
        oss << ", ";
    }

    Piece_list *pkt = new Piece_list(nodePtr->getFullName(), Msg_Pieces_list);

    if(rSerial == -1)
    {
        // generate a random number from 0 to 32766
        serial = intrand(32767);
    }
    else
        serial = rSerial;

    pkt->setSerial(serial);
    pkt->setList(oss.str().c_str());
    pkt->setBitLength(18400);
    pkt->setControlInfo( new NetwControlInfo(-1) );

    EV << "*** " << nodePtr->getFullName() << " is sending PList message: ";
    EV << "Serial: " << serial << ", List: " << oss.str() << endl;

    // send the pkt
    send(pkt, lowerLayerOut);
}


void ApplV_C2C_Epidemic::UpdateMask(Piece_list *m)
{
     // tokenize the received string
     std::string text = m->getList();
     char_separator<char> sep(", ");
     tokenizer< char_separator<char> > tokens(text, sep);

     for(tokenizer< char_separator<char> >::iterator beg=tokens.begin(); beg!=tokens.end();++beg)
     {
         std::string str = *beg;
         int PieceNumber = std::atoi(str.c_str());
         broadcastMask[PieceNumber] = 0;
     }
}


// broadcast CRL pieces of the vehicle with respect to broadcastMask[i] (used only in Epidemic and ICE).
void ApplV_C2C_Epidemic::sendCRLsMask()
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


void ApplV_C2C_Epidemic::finish()
{

}


ApplV_C2C_Epidemic::~ApplV_C2C_Epidemic()
{
    cancelAndDelete(Timer4);
    cancelAndDelete(Timer5);

    delete broadcastMask;
}
