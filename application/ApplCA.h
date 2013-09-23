#include <Appl.h>


class bufferEntry
{
  public:
    CRL_Piece *piece;
    int RSU;
    bool sent;
};


class ApplCA : public Appl
{
  public:
    ApplCA();
    virtual ~ApplCA();
    virtual void finish();
    virtual void initialize(int);
    virtual int numInitStages() const
    {
    	return 1;
    }
	virtual void handleMessage(cMessage *);
	virtual void receiveSignal(cComponent *, simsignal_t, cObject *);

  private:
	int dataToRSU;
	int InitialWait;
	int CRLsize;
	bool EnableShuffle;
    int pad;   // how many padding are added

	cMessage *Timer1;
    cMessage *Timer2;

	simsignal_t Signal_Magic_Req;

	std::ostringstream CRLbytes;
	std::vector<CRL_Piece *> PiecesCRL;
	std::vector<bufferEntry *> buffer;

    void CalculateMatrixA();

	void createCRL();

	void processCRL(std::vector<Certificate *>);
    std::vector<std::string> NOerasure();
    std::vector<std::string> erasure();
    std::vector<CRL_Piece *> addHeader(std::vector<std::string>);

    std::vector<CRL_Piece *> shuffle(std::vector<CRL_Piece *>);
    void fillBuffer(std::vector<CRL_Piece *>);
    void sendBuffer();
};
