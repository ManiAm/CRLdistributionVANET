#include <Appl.h>
#include "mobility/traci/TraCIMobility.h"


class ApplV : public Appl
{
  public:
    ApplV();
    virtual ~ApplV();
    virtual void finish();
    virtual void initialize(int);
    virtual int numInitStages() const
    {
    	return 3;
    }
	virtual void handleMessage(cMessage *);
	virtual void receiveSignal(cComponent *, simsignal_t, const char*);
	virtual void receiveSignal(cComponent *, simsignal_t, cObject*);

	// defined public to be used by ApplV_C2C_Epidemic_Ideal
    void recieveCRL(cMessage *);
    std::vector<CRL_Piece *> PiecesCRLcollected;

  protected:
	// parameters
	double beacon_Interval;
    double V2V_tho;
	double V2V_Wait_Beacon;
	bool V2V_EnableMultiChannel;
	double V2V_Wait_CRL;

    // variables
	int CRL_Piece_No;
	int CRLChannel;
    std::string EncounteredRSU_old;

	// for MPB method
	bool AnyPieceReceived;

	// for ICE
	int rangeStart;
	int rangeEnd;

	cMessage *Timer1;   // for sending beacon messages
	cMessage *Timer2;   // for sending Magic_Req signal
	cMessage *Timer3;   // for sending msg in compromised node
	cMessage *Timer_Verify;

     // is used in sendCRLs method
	int totFrames;
	unsigned int forCounter;

    enum ApplV_States
	{
		// states of ApplV
		STATE_IDLE,  // 0
		STATE_BROADCAST_BEACON_SENDING,
		STATE_BROADCAST_BEACON_SEND,

		// states for V2V algorithms
		STATE_UNICAST_BEACON_SENDING,
		STATE_UNICAST_BEACON_SEND,
	    STATE_WAIT_FOR_UNICAST_BEACON,
		STATE_WAITING_CRL,
		STATE_WAITING_CRL_2,
		STATE_WAIT_FOR_MORE_BEACONS,
		STATE_SENDING_CRLS,
		STATE_CRL_LIST_SEND
    };

    ApplV_States state;  // state of the vehicle node

    TraCIMobility* traci;
    static const simsignalwrap_t mobilityStateChangedSignal;

    simsignal_t Signal_fromMAC;
    simsignal_t Signal_Magic_Req;

    void sendBeacon(int, bool);
    bool isMagicCar();
	virtual void Run_V2V_Algorithm(Beacon *) { };
	virtual void updateRange() { };  // implemented only in ICE

  private:
	// parameters
	double speed;
	double MagicCarsRatio;
	double MagicReqTime;
	bool CRLrecons;
	bool CompromisedEnable;
	double CompromisedMsgSendTime;

    enum VehicleType
	{
        privateV,
        public_transport,
        public_emergency,
        public_authority,
        public_army,
        vip,
        ignoring,
        passenger,
        hov,
        taxi,
        bus,
        delivery,
        transport,
        lightrail,
        cityrail,
        rail_slow,
        rail_fast,
        motorcycle,
        bicycle,
        pedestrian,
    };

    VehicleType vType;  // type of vehicle
    static bool VehicleTypeSet;

    bool IsCompromised;
    static bool runCompromised;
    bool receiveCompromisedMsg;

	void recieveBeacon(cMessage *);

	void sendMsg();
    bool IsValid(cMessage *);
	void receiveMsg(cMessage *);

	void updateText();

	void setCompromised();

	void setMagicCar();
	void setVehicleType();

	void CRLreconstruction();
	void CRLreconstruction_NoErasure(); // when erasure code is not used
	void CRLreconstruction_Erasure();  // when erasure code is used
	double round(double);
};
