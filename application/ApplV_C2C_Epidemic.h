#include <ApplV.h>
#include <Piece_list_m.h>


class ApplV_C2C_Epidemic : public ApplV
{
  public:
    ApplV_C2C_Epidemic();
    virtual ~ApplV_C2C_Epidemic();
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
    // Timer1, Timer2 and Timer3 are reserved in ApplV!
    cMessage *Timer4;
    cMessage *Timer5;
    int serial;

    int *broadcastMask;   // an array of int

    void Send_CRL_List_Msg(int);
    void UpdateMask(Piece_list *);
    void sendCRLsMask();
};
