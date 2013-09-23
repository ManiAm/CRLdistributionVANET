package ned;

import org.mixim.base.modules.*;
import org.mixim.modules.nic.Nic80211MultiChannel;

module NodeCA
{
    parameters:
        string mobType;  // type of the mobility module
        @display("bgb=,,white,,");
    gates:
        input radioIn;  // gate for sendDirect
        output dataToRSU[];
    //  input dataFromRSU;

    submodules:
        arp: BaseArp {
            parameters:
                @display("p=130,108;b=24,24,rect,blue;i=block/process");
        }
        mobility: <mobType> like IMobility {
            parameters:
                @display("p=130,172;i=block/cogwheel");
        }
        appl: ApplCA {
            parameters:
                @display("p=60,50;i=app");
        }
    //    net: <netwType> like IBaseNetwLayer {
      //      parameters:
        //        @display("p=60,108;i=block/layer");
     //   }
   //     nic: Nic80211 {
     //       parameters:
       //         @display("p=60,166;i=block/ifcard");
     //   }
    connections allowunconnected:
 //       nic.upperGateOut --> net.lowerGateIn;
 //       nic.upperGateIn <-- net.lowerGateOut;
 //       nic.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> net.lowerControlIn;
 //       nic.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- net.lowerControlOut;

   //     net.upperGateOut --> appl.lowerGateIn;
    //    net.upperGateIn <-- appl.lowerGateOut;
    //    net.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> appl.lowerControlIn;
    //    net.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- appl.lowerControlOut;

   //     radioIn --> nic.radioIn;
        
        for i=0..sizeof(dataToRSU)-1 
        {
            appl.dataToRSU++ --> dataToRSU[i];
          //  dataFromRSU --> appl.dataFromRSU;
        }
}

