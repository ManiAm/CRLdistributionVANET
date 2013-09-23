#include <ApplV.h>


class ApplV_NewMPB : public ApplV
{
  public:
    ApplV_NewMPB();
    virtual ~ApplV_NewMPB();
    virtual void finish();
    virtual void initialize(int);
    virtual int numInitStages() const
    {
    	return 3;
    }
	virtual void handleMessage(cMessage *);
	virtual void receiveSignal(cComponent *, simsignal_t, const char *);


  protected:
    void Run_V2V_Algorithm(Beacon *);


  private:
	bool Isitme;
	int maxCRL;
	int maxChannel;

	int PreChannel;    // save the channel before changing it. to restore it later
	int numFrames;

	// Timer1, Timer2 and Timer3 are reserved !
	cMessage *Timer4;
	cMessage *Timer5;

    void whoHasMoreCRL(Beacon *);
    void whoShouldSendCRL();
    void waitingCRLOver();
    void sendCRLs();
};
