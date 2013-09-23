#include <ApplV.h>


class ApplV_ICEv2 : public ApplV
{
  public:
    ApplV_ICEv2();
    virtual ~ApplV_ICEv2();
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
    void updateRange();

  private:
    int counter;

    // Timer1, Timer2 and Timer3 are reserved in ApplV!
    cMessage *Timer4;
    cMessage *Timer5;

    int *broadcastMask;   // an array of int

    void update(Beacon *);
    void checkCounter();
    void waitForCRLover();
    void sendCRLsMask();
    int Maximum();
    int IsExist(int);
};
