#include <ApplCA.h>


Define_Module(ApplCA);


// constructor
ApplCA::ApplCA()
{


}


void ApplCA::initialize(int stage)
{
	Appl::initialize(stage);

    if(stage == 0) 
	{
		dataToRSU = findGate("dataToRSU");

		InitialWait = par("InitialWait");
		if(InitialWait < 0)
		    error("value for InitialWait is incorrect !!");

		CRLsize = par("CRLsize");
		if(CRLsize <= 0)
	        error("value for CRLsize is incorrect !!");
		
		EnableShuffle = par("EnableShuffle").boolValue();

		Pseudonym_lifeTime = par("Pseudonym_lifeTime").doubleValue();
        if(Pseudonym_lifeTime <= 0)
            error("value for Pseudonym_lifeTime is incorrect !!");

        if(!ErasureCode)
        {
            NoSegments = par("NoSegments");

            if(NoSegments <= 0)
                error("Value of NoSegments is incorrect! Check configuration file.");

            totalPieces = NoSegments;
        }
        else
        {
            N = par("N");
            M = par("M");

            if(N <= 0 || N >= 256)
                error("Value of N is not correct! Check configuration file.");

            if(M <= 0 || M > N)
                error("Value of M is not correct! Check configuration file.");

            totalPieces = N;

            // if ErasureCode is enabled, we calculate Matrix_A according to its dimension (N * M).
            // Calculation is only done once, and Matrix_A is common in all nodes.
            CalculateMatrixA();
        }

        // register Magic_Req signal and subscribe it globally.
    	// Magic cars emit this signal to request CRL from CA.
        Signal_Magic_Req = registerSignal("Magic_Req");
    	simulation.getSystemModule()->subscribe("Magic_Req", this);

        Timer1 = new cMessage( "Timer_Initial_Wait_CA", Timer_Initial_Wait_CA );
		scheduleAt(simTime() + InitialWait, Timer1);

        Timer2 = new cMessage( "Timer_Wait_Tx", Timer_Wait_Tx );
    }
}


void ApplCA::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
        if(msg->getKind()==Timer_Initial_Wait_CA)
		{
            createCRL();
		}
        else if(msg->getKind()==Timer_Wait_Tx)
        {
            sendBuffer();
        }
    } 
	else if(msg->getArrivalGateId()==lowerLayerOut)
	{

    } 
	else if(msg->getArrivalGateId()==lowerControlOut) 
	{

    } 
	else if(msg->getArrivalGateId()==lowerControlIn)
	{

    } 
	else if(msg->getArrivalGateId()==lowerLayerIn)
	{

    }
	else
	{
	    EV << "Unknown message! -> delete, kind: " << msg->getKind() << endl;
	    delete msg;	
	}    
}


void ApplCA::receiveSignal(cComponent* source, simsignal_t signalID, cObject *obj)
{
	Enter_Method_Silent();

	// we receive a Magic_Req signal from magic vehicle.
    if(signalID == Signal_Magic_Req)
    {
    	EV <<  "*** " << nodePtr->getFullName() << " receives a Magic_Req signal from " << source->getFullName() << endl;

    	if(PiecesCRL.size() > 0)
    	{
    	    EV <<  "*** " << nodePtr->getFullName() << " is sending the CRL ..." << endl;

            MagicCarsData *data = new MagicCarsData(source->getFullName(), EnableShuffle ? shuffle(PiecesCRL): PiecesCRL);

            // note that all magic cars in the network will receive this signal, but only
            // the magic car that sent the Magic_Req will store this
            simsignal_t Signal_Magic_Res = registerSignal("Magic_Res");
    	    nodePtr->emit(Signal_Magic_Res, data);
    	}
    	else
    	{
    		EV <<  "*** " << nodePtr->getFullName() << " does not have any CRL!" << endl;
    	}
    }
}


// Calculate Matrix A when Erasure Code is enabled.
// This matrix is N by M.
void ApplCA::CalculateMatrixA()
{
    EV << endl << "*** calculating Matrix_A -> M is " << M << ", and N is " << N << endl;

    Matrix_A.resize(N,M);

    int rnd;

    for(int i = 0; i< N; i++)
    {
        for (int j = 0; j < M ; j++)
        {
            if(j==0)  Matrix_A(i,j) = 1;
            else if(j==1)
            {
                // choose a random number in the range [0,255]
                rnd = intrand(256);
                Matrix_A(i,j) = rnd;
            }
            else
            {
                Matrix_A(i,j) = gpow(rnd,j);
            }
        }
    }

    EV << endl << Matrix_A << endl << endl;
}


void ApplCA::createCRL()
{
    // CRL consists of number of certificates
    std::vector<Certificate *> CRL;

    // CRLsize is a parameter
    for(int i = 0; i< CRLsize; i++)
    {
        char buf[25];
        sprintf(buf, "Node%d_Certificate", i+1);

        Certificate *cert = new Certificate();

        cert->CerName = buf;
        cert->CAname = "CA1";
        cert->CAid = 12;
        cert->NodeName = "V[x]";
        cert->NodeID = i+1;

        CRL.push_back(cert);
    }

    EV << "*** " << nodePtr->getFullName() << " created a CRL with size " << CRLsize << endl;

	// send signal to statistics to report the CRL size.
	simsignal_t Signal_CRLsize_CA = registerSignal("CRLsize_CA");
    nodePtr->emit(Signal_CRLsize_CA, CRLsize);

    // do some processing on CRL vector!
    processCRL(CRL);
}


void ApplCA::processCRL(std::vector<Certificate *> CRL)
{
    // Step 1: change CRL message into bytes and save it in CRLbytes stream.
    // CRLbytes is defined globally to be reachable in Noerasure() and erasure() methods.
    {
        boost::archive::text_oarchive oa(CRLbytes);
        oa << CRL;
    }


    // todo:
    char fileName [30];
    sprintf( fileName, "matrix.txt");
    // open the file in a+ mode
    FILE *pFile = fopen(fileName,"a+");



    unsigned long len = CRLbytes.str().size();

    EV << endl << "CRL is treated as raw data (sequence of " << len << " bytes):" << endl;

    for(unsigned long i=0; i<len; i++)
    {
        EV << (unsigned int)CRLbytes.str().at(i) << " " ;
        fprintf (pFile, "%d ", (unsigned int)CRLbytes.str().at(i) );
    }

    fprintf (pFile, "\n");
    fclose(pFile);

    EV << endl << endl;

    // Step 2: apply the erasure code if its enable.
    std::vector<std::string> EncodedCRL;

    if (!ErasureCode)
    {
        EncodedCRL = NOerasure();

        // send signal to statistics to report Thresh when ErasureCode=false.
        simsignal_t Signal_Thresh = registerSignal("Thresh");
        nodePtr->emit(Signal_Thresh, NoSegments);
    }
    else
    {
        EncodedCRL = erasure();

        // send signal to statistics to report Thresh when ErasureCode=true.
        simsignal_t Signal_Thresh = registerSignal("Thresh");
        nodePtr->emit(Signal_Thresh, M);
    }

	// Step 3: adding header to each of the entries in EncodedCRL.
	// PiecesCRL is defined in header file to be accessible from the whole class.
	PiecesCRL = addHeader(EncodedCRL);

    // now we have all CRL pieces in PiecesCRL vector
	fillBuffer(PiecesCRL);

	// we filled the buffer, and now send it
	sendBuffer();
}


std::vector<std::string> ApplCA::NOerasure()
{
    EV << "*** " << nodePtr->getFullName() << ": erasure code is disabled!" << endl;
    EV << "Dividing CRL message into "<< NoSegments << " segments." << endl;

    unsigned long len = CRLbytes.str().size();
    int n;   // number of bytes in each pieces

    if(len % NoSegments == 0)
        n = len / NoSegments;
    else
        n = (len / NoSegments) + 1;

    EV << "This means that each segment has maximum of " << n << " bytes." << endl;

    // now we save the bytes of each piece to the tmp vector.
    std::vector<std::string> tmp;
    std::ostringstream oss;

    for(unsigned long i=0; i<len; i++)
    {
        EV << (unsigned int)CRLbytes.str().at(i) << " ";
        oss << CRLbytes.str().at(i);

        if(i != 0 && (i+1)%n == 0)
        {
            EV << "  -   ";
            tmp.push_back(oss.str());
            oss.str("");
        }
    }

    if(len % NoSegments != 0)
        tmp.push_back(oss.str());

    EV << endl << endl;

    return tmp;
}


std::vector<std::string> ApplCA::erasure()
{
    EV << "*** " << nodePtr->getFullName() << " is applying erasure code on CRL." << endl << endl;

    EV << "Step 1: M is " << M << ", so we split CRL bytes into " << M << "-byte segments." << endl;

    unsigned long len = CRLbytes.str().size();

    for(unsigned long i=0; i<len; i++)
    {
        EV << (unsigned int)CRLbytes.str().at(i) << " ";
        if(i != 0 && (i+1)%M == 0) EV << "  -   ";
    }

    EV << endl << endl;

    int L;

    if(len % M == 0)
    {
        EV << "We have " << len << " bytes which is multiple of M (padding is not needed)." << endl;
        L = len / M;
        pad = 0;
        EV << "Number of parts (L) = " << L << endl;
    }
    else
    {
        EV << "We have " << len << " bytes which is not multiple of M (padding will be added in the last part)." << endl;
        L = (len / M) + 1;
        pad = M - (len % M);
        EV << "Number of parts (L) = " << L << endl;
        EV << "Number of padding (pad) = " << pad << endl;
    }

    EV << endl;

    EV << "Now we construct Matrix B and put each segment vertically:" << endl;

    Matrix<float, Dynamic, Dynamic> Matrix_B;
    Matrix_B.resize(M,L);
    Matrix_B = ArrayXXf::Zero(M,L);

    int row = 0, col = 0;

    for(unsigned long i=0; i<len; i++)
    {
        row = i % M;
        if(i != 0 && i%M == 0) col++;
        Matrix_B(row,col) = (unsigned int)CRLbytes.str().at(i);
    }

    EV << Matrix_B;

    EV << endl << endl;

    EV << "Step 2: We calculate Matrix W = Matrix A * Matrix B:" << endl << endl;
    // Matrix A was calculated before (All nodes use the same Matrix A)

    Matrix<float, Dynamic, Dynamic> Matrix_W;
    Matrix_W.resize(N,L);
    Matrix_W = ArrayXXf::Zero(N,L);

    Matrix_W = Matrix_A * Matrix_B;

    /* todo: uncomment this :)
    // matrix multiplication in GF(256)
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < L; col++)
        {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < M; inner++)
            {
                uint8_t tmp = gmul( (uint8_t)Matrix_A(row,inner), (uint8_t)Matrix_B(inner,col) );
                Matrix_W(row,col) = gadd ( (uint8_t)Matrix_W(row,col), tmp );
            }
        }
    }
    */

    EV << "Matrix W would be:" << endl;

    EV << Matrix_W << endl << endl;

    EV << "Each row of Matrix W is a CRL piece. As can be seen, the CRL is transformed into N = " << N << " pieces." << endl;
    EV << "CA sends each of the pieces separately to the RSUs." << endl;
    EV << "Any M = " << M << " out of N = " << N << " CRL pieces is enough for re-construction of the original CRL." << endl;

    EV << endl;

    std::vector<std::string> tmp;
    std::ostringstream oss;

    for(int i=0; i<N; i++)
    {
        oss.str("");

        for(int j=0; j<L; j++)
        {
            oss << Matrix_W(i,j) << "#";  // # is the delimiter!
        }

        tmp.push_back(oss.str());
    }

    return tmp;
}


std::vector<CRL_Piece *> ApplCA::addHeader(std::vector<std::string> vec)
{
    EV << "*** " << nodePtr->getFullName() << " is adding header to each entry." << endl;

    // generate a random initial Sequence number
    // int ISeqNo = 1 + intrand(1000);
    int ISeqNo = 0;

    // a vector for storing the CRL pieces
    std::vector<CRL_Piece *> vecResult;

	for(unsigned int i=0; i< vec.size(); i++)
	{
        // create the packet for transmitting a certificate
        CRL_Piece *pkt = new CRL_Piece(nodePtr->getFullName(), Msg_CRL_CA);

        pkt->setCRLversion(1);
        pkt->setTimestamp(0);
        pkt->setSeqNo(ISeqNo++);
        pkt->setCAid(23);

        // when ErasureCode is enabled, we add number of padding in all pieces!
        // you do not know which subset of pieces will be received in a vehicle
        if(ErasureCode)
            pkt->setPad(pad);
        else
            pkt->setPad(-1);

        pkt->setPayload(vec[i].c_str());

        pkt->setBitLength(18400);   // size of each msg is 2300 Bytes.
                                      // note that 4 Bytes header will be added in network layer.

        // pkt is ready. we add it to the vector
        vecResult.push_back(pkt);
	}

    EV << "*** " << " CRL pieces are ready: ";

    for(unsigned int i=0; i<vecResult.size(); i++)
        EV << vecResult[i]->getSeqNo() << "  ";

    EV << endl << endl;

	return vecResult;
}


std::vector<CRL_Piece *> ApplCA::shuffle(std::vector<CRL_Piece *> vec)
{
	int n = 0;

	for (int i = vec.size() - 1; i >= 0; i--)
	{
	    n = intrand( vec.size() );

	    if(i != n)
	        std::swap(vec[i], vec[n]);
	}

	return vec;
}


// send the CRL pieces to all RSUs
void ApplCA::fillBuffer(std::vector<CRL_Piece *> PiecesCRL)
{
    if ( ev.isGUI() )  nodePtr->bubble("Sending CRL Msg");

    std::vector<bufferEntry *> TempBuffer;

    // get number of RSUs
    int RSUNo = nodePtr->getParentModule()->par("numRSU");

    for(int i=0; i<RSUNo; i++)
    {
        // shuffling is not enabled
        if(!EnableShuffle)
        {
            for(unsigned int j=0; j < PiecesCRL.size(); j++)
            {
                bufferEntry *entry = new bufferEntry();
                entry->piece = PiecesCRL[j];
                entry->RSU = i;
                entry->sent = false;

                TempBuffer.push_back(entry);
            }
        }
        // send a shuffled version of array to RSU i
        else
        {
            std::vector<CRL_Piece *> PiecesCRL_shuffled;
            PiecesCRL_shuffled = shuffle(PiecesCRL);

            for(unsigned int j=0; j < PiecesCRL_shuffled.size(); j++)
            {
                bufferEntry *entry = new bufferEntry();
                entry->piece = PiecesCRL_shuffled[j];
                entry->RSU = i;
                entry->sent = false;

                TempBuffer.push_back(entry);
            }
        }
    }

    int index;

    // re-arrange the order of buffer to increase efficiency
    for(int i=0; i<totalPieces; i++)
    {
        for(int j=0; j<RSUNo; j++)
        {
            index = (totalPieces * j) + i;

            bufferEntry *entry = new bufferEntry;
            entry->RSU = TempBuffer[index]->RSU;
            entry->piece = TempBuffer[index]->piece;
            entry->sent = TempBuffer[index]->sent;

            buffer.push_back(entry);
        }
    }
}


// send buffer to RSUs
void ApplCA::sendBuffer()
{
    for(unsigned int k=0; k < buffer.size(); k++)
    {
        bufferEntry *entry = buffer[k];

        // if this entry has not been sent
        if(!entry->sent)
        {
            // now check if the channel is free
            cChannel *txChannel = gate("dataToRSU", entry->RSU)->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();

            // channel free; send out the packet immediately
            if (txFinishTime <= simTime())
            {
                EV << "*** " << nodePtr->getFullName() << " is sending piece ";
                EV << entry->piece->getSeqNo() << " to RSU " << entry->RSU;
                EV << " at " << simTime() << endl;

                send(entry->piece->dup(), "dataToRSU", entry->RSU);

                entry->sent = true;
            }
            else
            {
                scheduleAt(txFinishTime, Timer2);

                // return from this method
                return;
            }
        }
    }
}


void ApplCA::finish()
{


}


ApplCA::~ApplCA()
{
    cancelAndDelete(Timer1);
    cancelAndDelete(Timer2);

    for(unsigned int i = 0; i < PiecesCRL.size(); i++)
    {
    	delete PiecesCRL[i];
    }
}
