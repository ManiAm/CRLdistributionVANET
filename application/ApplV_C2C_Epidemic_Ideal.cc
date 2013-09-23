#include <ApplV_C2C_Epidemic_Ideal.h>


Define_Module(ApplV_C2C_Epidemic_Ideal);

// constructor
ApplV_C2C_Epidemic_Ideal::ApplV_C2C_Epidemic_Ideal()
{

}
 

void ApplV_C2C_Epidemic_Ideal::initialize(int stage)
{
	ApplV::initialize(stage);

    if(stage == 0)
	{

    }
}


void ApplV_C2C_Epidemic_Ideal::handleMessage(cMessage *msg)
{
	ApplV::handleMessage(msg);
}


void ApplV_C2C_Epidemic_Ideal::receiveSignal(cComponent *source, simsignal_t signalID, const char *string)
{
	Enter_Method_Silent();

	ApplV::receiveSignal(source, signalID, string);
}


void ApplV_C2C_Epidemic_Ideal::Run_V2V_Algorithm(Beacon *m)
{
    EV << endl << "*** run the \"V2V C2C Epidemic (Ideal)\" algorithm: " << endl;

    int extractedID = getNodeIndex(m->getNodeName());

    cModule *module = simulation.getSystemModule()->getSubmodule("V", extractedID);
    cModule *applMod = module->getSubmodule("appl");

    ApplV *pointer = static_cast<ApplV *>(applMod);

    // PiecesCRLcollected
    // pointer->PiecesCRLcollected

    // update the peer
    for(unsigned int i=0; i<PiecesCRLcollected.size(); i++)
    {
        bool found = false;

        for(unsigned int j=0; j<pointer->PiecesCRLcollected.size(); j++)
        {
            if(PiecesCRLcollected[i]->getSeqNo() == pointer->PiecesCRLcollected[j]->getSeqNo())
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            simsignal_t Signal_CRL_V2V = registerSignal("CRL_V2V");
            module->emit(Signal_CRL_V2V, 1);

            pointer->recieveCRL(PiecesCRLcollected[i]);
        }
    }

    // update ourself
    for(unsigned int i=0; i<pointer->PiecesCRLcollected.size(); i++)
    {
        bool found = false;

        for(unsigned int j=0; j<PiecesCRLcollected.size(); j++)
        {
            if(pointer->PiecesCRLcollected[i]->getSeqNo() == PiecesCRLcollected[j]->getSeqNo())
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            simsignal_t Signal_CRL_V2V = registerSignal("CRL_V2V");
            nodePtr->emit(Signal_CRL_V2V, 1);

            recieveCRL(pointer->PiecesCRLcollected[i]);
        }
    }
}


int ApplV_C2C_Epidemic_Ideal::getNodeIndex(const char *ModName)
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


void ApplV_C2C_Epidemic_Ideal::finish()
{

}


ApplV_C2C_Epidemic_Ideal::~ApplV_C2C_Epidemic_Ideal()
{

}
