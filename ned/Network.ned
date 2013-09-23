package ned;

import org.mixim.base.connectionManager.ConnectionManager;
import org.mixim.base.modules.BaseWorldUtility;
import org.mixim.modules.mobility.traci.TraCIScenarioManagerLaunchd;

network Network
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)

        double numCA;
        double numRSU;
        double numV;   // total number of vehicles in the network        

        string applType;
        V[*].applType = applType;
        bool ErasureCode;
        bool SumoMode;

        @display("bgb=$playgroundSizeX,$playgroundSizeY,white;bgp=0,0");

    types:
        channel C extends ned.DatarateChannel
        {
            datarate = 100Mbps;
            delay = 10us;
            @display("ls=red");
        }

    submodules:
        connectionManager: ConnectionManager {
            parameters:
                @display("p=114,24;b=42,42,rect,green;i=abstract/multicast");
        }
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                playgroundSizeZ = playgroundSizeZ;
                @display("p=30,24;i=misc/globe");
        }
        traciManager: TraCI {
            parameters:
                @display("p=30,94;i=abstract/people");
        }
        statistics: Statistics {
            parameters:
                @display("p=222,25;i=block/timer");
        }
        CA[numCA]: NodeCA {
            parameters:
                @display("p=159,87;i=old/pc");
        }
        RSU[numRSU]: NodeRSU {
            parameters:
                @display("p=88,154;i=device/antennatower");
        }
        V[numV]: NodeV {
            parameters:
                @display("p=256,154;i=old/ball");
        }

    connections allowunconnected:
        // one key point: initialization of NED files begin from top-level downward.
        // So BaseNetwork.ned is the first NED file which is initialized.
        // In BaseNetwork.ned, the number of indexes in "dataToRSU vector" which is declared
        // in NodeCA.ned will be determined. we used ++ operator in the form of CA[0].dataToRSU++ to do this.
        // Note that we can not use CA[0].dataToRSU[i] !!!
        // since the size of dataToRSU is known, NodeCA.ned has no problem in the line : for i=0..sizeof(dataToRSU)-1         
        for i=0..(numRSU-1) {
            CA[0].dataToRSU++ --> C --> RSU[i].dataFromCA;
           // RSU[0].dataToCA --> CA[0].dataFromRSU;
        }
}

