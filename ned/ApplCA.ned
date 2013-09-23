package ned;

simple ApplCA extends Appl
{
    parameters:
        @class(ApplCA);
        
        int InitialWait;
        int CRLsize;
        bool EnableShuffle;
        
        int NoSegments;        

        int M;
        int N; 
        
        double Pseudonym_lifeTime @unit(s);

    gates:        
      //  input dataFromRSU;
        output dataToRSU[];
}
