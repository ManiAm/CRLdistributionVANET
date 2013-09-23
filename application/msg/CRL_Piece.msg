
//cplusplus {{
//#include <VC_CRL_Piece_m.h> 
//typedef std::string pType;
//}};

// class cArray extends void;
//class noncobject pType;

packet CRL_Piece
{
    int CRLversion;
    int Timestamp;
    int SeqNo;
    int CAid;
    int pad;
    // cArray Array_CRL;
    string payload;
  // pType payload;
}
