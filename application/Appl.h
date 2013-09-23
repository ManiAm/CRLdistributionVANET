#include <omnetpp.h>

#include <msg/Beacon_m.h>
#include <msg/CRL_Piece_m.h>
#include <msg/SimpleMsg_m.h>

#include "FindModule.h"
#include "NetwControlInfo.h"

#include <Mac80211MultiChannel.h>

#include <ExtraClasses.h>

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

#include <Eigen/Dense>

using namespace Eigen;


class Appl : public BaseModule
{
  public:
	Appl(); // constructor
    virtual ~Appl();
    virtual void finish();
    virtual void initialize(int);

  protected:
    int lowerLayerIn;
    int lowerLayerOut;
    int lowerControlIn;
    int lowerControlOut;

	cModule *nodePtr;   // pointer to the Node
	int nodeID;   // id of the Node

	cModule *macPtr;    // pointer to MAC module
    Mac80211MultiChannel *nicClassPtr;   // pointer to the MAC layer
    double bitrate;

    const char *applType;
    bool ErasureCode;
    bool SumoMode;

    // these static variables are all initialized by CA. and
    // are common in all elements of the network.
    static int NoSegments;   // only when erasure code is disable
    static int M;
    static int N;
    static Matrix<float, Dynamic, Dynamic> Matrix_A;
    static int totalPieces;  // equal to NoSegments or N, depending on whether ErasureCode is enabled or not.

    static double Pseudonym_lifeTime;

    enum MessageType
	{
		// Self Messages
		Timer_Initial_Wait_CA,
		Timer_Wait_Tx,
       	Timer_CRL_Interval_RSU,
        Timer_Beacon_RSU,
       	Timer_Beacon_V,
    	Timer_Wait_Beacon_V,
    	Timer_Wait_CRL_Over_V,
    	Timer_Wait_CRL_Over_V_2,
    	Timer_Magic_Req,
    	Timer_Statistics_SnapShot,
    	Timer_Wait_PList,
    	Timer_Sending_Compromised_Msg,
    	Timer_Wait_Verify,

    	// Messages
        Msg_CRL_CA,
        Msg_CRL_RSU,
        Msg_CRL_V,
        Msg_Compromised_V,
        Msg_Pieces_list,
        Msg_Beacon_V,
        Msg_Beacon_RSU,
    };

    uint8_t gadd(uint8_t a, uint8_t b);
    uint8_t gsub(uint8_t a, uint8_t b);
    uint8_t gmul(uint8_t a, uint8_t b);
    uint8_t gpow(uint8_t a, uint8_t e);

  private:

};
