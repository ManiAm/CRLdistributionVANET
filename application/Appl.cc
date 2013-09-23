#include <Appl.h>

Define_Module(Appl);

int Appl::NoSegments;
int Appl::M;
int Appl::N;
Matrix<float, Dynamic, Dynamic> Appl::Matrix_A;
int Appl::totalPieces;
double Appl::Pseudonym_lifeTime;

// constructor
Appl::Appl()
{

}


void Appl::initialize(int stage)
{
    if(stage == 0)
	{
        lowerLayerIn  = findGate("lowerLayerIn");
        lowerLayerOut = findGate("lowerLayerOut");
        lowerControlIn  = findGate("lowerControlIn");
        lowerControlOut = findGate("lowerControlOut");

		// get the ptr of the current module (NodeCA or NodeRSU or NodeV)
		nodePtr = FindModule<>::findHost(this);
		if(nodePtr == NULL)
		    error("can not get a pointer to the module.");

		// get node id
		nodeID = nodePtr->getId();
        EV << "*** node id of " << nodePtr->getFullName() << " is: " << nodeID << endl;

    	// get a pointer to MAC submodule
        cModule *nicPtr = nodePtr->getSubmodule ("nic");
        if(nicPtr != NULL)    // CA does not have any nic !! so we should check nicPtr.
        {
            macPtr = nicPtr->getSubmodule("mac");
            bitrate = macPtr->par("bitrate");   // get the bitrate parameter from the MAC layer.

            if(macPtr != NULL)
                nicClassPtr = check_and_cast<Mac80211MultiChannel *>(macPtr);
        }

        applType = simulation.getSystemModule()->par("applType").stringValue();
        ErasureCode = simulation.getSystemModule()->par("ErasureCode").boolValue();
    	SumoMode =  simulation.getSystemModule()->par("SumoMode").boolValue();
    }
}


/* Add two numbers in a GF(2^8) finite field */
uint8_t Appl::gadd(uint8_t a, uint8_t b)
{
    return a ^ b;
}


/* Subtract two numbers in a GF(2^8) finite field */
uint8_t Appl::gsub(uint8_t a, uint8_t b)
{
    return a ^ b;
}


/* Multiply two numbers in the GF(2^8) finite field defined
   by the polynomial x^8 + x^4 + x^3 + x^2 + 1, hexadecimal 0x11d.*/
uint8_t Appl::gmul(uint8_t a, uint8_t b)
{
    uint8_t p = 0;
    uint8_t counter;
    uint8_t hi_bit_set;

    for (counter = 0; counter < 8; counter++)
    {
        if (b & 1) p ^= a;
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) a ^= 0x11D;
        b >>= 1;
    }

    return p;
}


uint8_t Appl::gpow(uint8_t a, uint8_t e)
{
    uint8_t tmp = 1;

    for(int i=1; i<=e; i++)
        tmp = gmul(a,tmp);

    return tmp;
}


void Appl::finish()
{

}


Appl::~Appl()
{

}
