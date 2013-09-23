package ned;

simple ApplRSU extends Appl
{
    parameters:
    @class(ApplRSU);
    
    double CRL_Interval @unit(s);
    double beacon_Interval @unit(s);
    bool I2V_EnableMultiChannel;
    double I2V_tho @unit(s);

    gates:  
        input dataFromCA;
     //   output dataToCA;

}