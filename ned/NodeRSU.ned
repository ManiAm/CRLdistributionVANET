package ned;

import org.mixim.base.modules.*;
import org.mixim.modules.nic.Nic80211MultiChannel;
import org.mixim.modules.power.battery.BatteryStats;


module NodeRSU
{
    parameters:
        string mobType;  // type of the mobility module
        @display("bgb=,,white,,");
    gates:
        input radioIn;  // gate for sendDirect
        input dataFromCA;
    //  output dataToCA;
    submodules:
        arp: BaseArp {
            parameters:
                @display("p=130,108;b=24,24,rect,blue;i=block/process");
        }
        mobility: <mobType> like IMobility {
            parameters:
                @display("p=130,172;i=block/cogwheel");
        }
        appl: ApplRSU {
            parameters:
                @display("p=60,50;i=app");
        }
        net: BaseNetwLayer {
            parameters:
                @display("p=60,108;i=block/layer");
        }
        nic: Nic80211MultiChannel {
            parameters:
                @display("p=60,166;i=block/ifcard");
        }
    connections allowunconnected:
        nic.upperLayerOut --> net.lowerLayerIn;
        nic.upperLayerIn <-- net.lowerLayerOut;
        nic.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> net.lowerControlIn;
        nic.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- net.lowerControlOut;

        net.upperLayerOut --> appl.lowerLayerIn;
        net.upperLayerIn <-- appl.lowerLayerOut;
        net.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> appl.lowerControlIn;
        net.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- appl.lowerControlOut;

        radioIn --> nic.radioIn;

        //  appl.dataToCA --> dataToCA;
        dataFromCA --> appl.dataFromCA;
}

