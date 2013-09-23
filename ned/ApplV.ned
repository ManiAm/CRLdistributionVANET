package ned;

simple ApplV extends Appl
{    
    parameters:
        @class(ApplV);
            
        double speed @unit(mps);        
        double beacon_Interval @unit(s);
        
        double V2V_tho @unit(s); 
        double V2V_Wait_Beacon @unit(s);
                
        bool V2V_EnableMultiChannel;
        double V2V_Wait_CRL @unit(s);  
        
        int MagicCarsRatio;
        double MagicReqTime;  
        
        bool CRLrecons;
        
        bool CompromisedEnable;
        double CompromisedMsgSendTime @unit(s);
        
    gates:       

}
