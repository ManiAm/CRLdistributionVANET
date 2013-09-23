#include <ApplV.h>


class ApplV_C2C_Epidemic_Ideal : public ApplV
{
  public:
    ApplV_C2C_Epidemic_Ideal();
    virtual ~ApplV_C2C_Epidemic_Ideal();
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
	int getNodeIndex(const char *);
};
