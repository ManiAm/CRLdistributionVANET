
packet Beacon
{
    int destAddr = -1;   // destination address
    int srcAddr = -1;    // source address
    string NodeName;
	double positionX;
	double positionY;
	double speed;
	
	bool NeedCRL;	
	int CRL_Piece_No;
	int rangeS;
	int rangeE;
    int CRLchannel;
}
