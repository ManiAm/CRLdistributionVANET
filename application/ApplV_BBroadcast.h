#include <ApplV.h>


class ApplV_BBroadcast : public ApplV
{
  public:
    ApplV_BBroadcast();
    virtual ~ApplV_BBroadcast();
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
    void Run_V2V_Algorithm_MultiChannel(Beacon *);

  private:
	// Timer1, Timer2 and Timer3 are reserved !
    cMessage *Timer4;
    cMessage *Timer5;

    int SrcCh;
    int DesCh;
	int PreChannel;    // save the channel before changing it. to restore it later
	int numFrames;

	void sendCRLs();
};
