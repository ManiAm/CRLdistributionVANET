#include <omnetpp.h>

#include <Beacon_m.h>
#include <CRL_Piece_m.h>

#include "FindModule.h"
#include "NetwControlInfo.h"

#include <Mac80211MultiChannel.h>

// ExtraClasses.h is also in here
#include <ApplV.h>


class Statistics : public BaseModule
{
  public:
	Statistics(); // constructor
    virtual ~Statistics();
    virtual void finish();
    virtual void initialize(int);
    virtual void handleMessage(cMessage *);
	virtual void receiveSignal(cComponent *, simsignal_t, long);
    virtual void receiveSignal(cComponent *, simsignal_t, double);
	virtual void receiveSignal(cComponent *, simsignal_t, const char *);
    virtual void receiveSignal(cComponent *, simsignal_t, cObject *);

  private:
    bool SumoMode;
    bool ErasureCode;

    cMessage *Timer1;

	simsignal_t Signal_V_Deleted;

	simsignal_t Signal_In_Range;

	simsignal_t Signal_CRLsize_CA;
    simsignal_t Signal_Thresh;

	simsignal_t Signal_NoRSUs;
	simsignal_t Signal_Beacon_RSU;
	simsignal_t Signal_Broadcast_RSU;
	simsignal_t Signal_I2V_tho;

	simsignal_t Signal_NoV;
    simsignal_t Signal_CRL_V2V;     // V reports that a pieces is received from V2V communication
    simsignal_t Signal_CRL_I2V;     // V reports that a piece is received from I2V communication
	simsignal_t Signal_NoCRLs_V;    // V reports that a "new piece" is received
	simsignal_t Signal_DupCRLs_V;   // V reports that a "duplicate piece" is received (separately for RSU, V)
	simsignal_t Signal_Beacon_V_B;
	simsignal_t Signal_Beacon_V_U;
	simsignal_t Signal_GetCRL_RSU;  // V reports that it is now receiving the pieces from a new RSU
    simsignal_t Signal_V2V_tho;
    simsignal_t Signal_Msg_received_V;
    simsignal_t Signal_Msg_sent_V;

	simsignal_t Signal_RxErrors;    // V or RSU uses this signal to report the frames which are received with error.

    static int Var_NoMod_Deleted;

    static int Var_IN_RSU_RANGE;
    static int Var_IN_V_RANGE;

  	static int Var_CRLsize_CA;
    static int Var_Thresh;

  	static int Var_NoRSUs;
    static int Var_Beacon_RSU;   // number of beacons send from RSUs
  	static int Var_NoBroadcast_RSU;    // number of broadcasts in all RSUs
  	static double Var_I2V_tho;

  	static int Var_NoV;
  	static int Var_CRL_I2V;      // number of I2V communication
  	static int Var_CRL_V2V;      // number of V2V communication
  	static int Var_NoDupCRLs_RSU;
  	static int Var_NoDupCRLs_V;
  	static int Var_Beacon_V_B;   // number of broadcast beacon frames send from V
  	static int Var_Beacon_V_U;   // number of unicast beacon frames send from V
    static double Var_V2V_tho;

  	static std::vector<NodeEntry *> Vec_NoCRLs_V;       // which V node received all CRLs
  	static std::vector<NodeEntry *> Vec_NewRSU_Encounter;
  	static std::vector<NodeEntry *> Vec_GetCRL_RSU;
  	static std::vector<NodeEntry *> Vec_RxErrors_V;     // if node V received a frame with error <node name, time>
  	static std::vector<NodeEntry *> Vec_RxErrors_RSU;   // if node RSU received a frame with error <node name, time>
    static std::vector<NodeEntry *> Vec_Received_Message;
    static std::vector<RxMsg *> Vec_Sent_Message;

    enum MessageType
    {
        // Self Messages
        Timer_Statistics_SnapShot,
    };

    // this is done during the simulation
    void printToFile_SnapShots();

    int getNodeIndex(const char*);
    int findInVector(std::vector<NodeEntry *>, const char *);
    void Update_Vectors(const char *);
  	std::vector<NodeEntry *> SortByID(std::vector<NodeEntry *>);

  	void TerminateSimulation();
  	void print_Statistics();
  	void printToFile_RSUencounter();
  	void printToFile_FalseMsgPropagation();
  	void printToFile_CRLreception();
    void printToFile_Detailed();

    double ConvTime();
};
