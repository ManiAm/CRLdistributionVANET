#include <Appl.h>


class ApplRSU : public Appl
{
  public:
    ApplRSU();
    virtual ~ApplRSU();
    virtual void finish();
    virtual void initialize(int);
    virtual int numInitStages() const
    {
    	return 1;
    }
	virtual void handleMessage(cMessage *);
	virtual void receiveSignal(cComponent *, simsignal_t, const char *);

  private:
	// gates id
	int dataFromCA;

    // variables
	cMessage *Timer1;   // beacon
	cMessage *Timer2;   // CRL_Interval
	cMessage *Timer3;

	std::vector<CRL_Piece *> PiecesCRLfromCA;
	unsigned int forCounter;    // is used in sendPeriodicCRL method
	bool AnyoneNeedCRL;
    int *broadcastMask;  // for ICE

	// parameters
	double CRL_Interval;
	double beacon_Interval;
	double I2V_tho;
	bool I2V_EnableMultiChannel;

    simsignal_t Signal_fromMAC;

    enum ApplRSU_States
    {
        STATE_IDLE,  // 0
        STATE_WAIT,
        STATE_SENDING_CRLS,
        STATE_CRL_LIST_SEND
    };

    ApplRSU_States state;  // state of the RSU

    void recieveCRL(cMessage *);
    void sendPeriodicCRL();
    void sendCRLs();
    void sendCRLsMask();
    void sendCRLsMaskv2();
    void recieveBeacon(cMessage *);
	void sendBeacon();
    int Maximum();
    int IsExist(int);
};
