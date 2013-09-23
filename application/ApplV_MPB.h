#include <ApplV.h>


class ApplV_MPB : public ApplV
{
  public:
    ApplV_MPB();
    virtual ~ApplV_MPB();
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
    int counter;

	// Timer1, Timer2 and Timer3 are reserved in ApplV!
	cMessage *Timer4;
	cMessage *Timer5;

	void update(Beacon *);
	void checkCounter();
	void waitForCRLover();
	void sendCRLs();
};
