#include "RSUmobility.h"
#include "mobility/traci/TraCIMobility.h"

Define_Module(RSUmobility);


void RSUmobility::initialize(int stage)
{
    ConstSpeedMobility::initialize(stage);

    // we are sure that the traciManager module is initialized in stage 0 and 1
    // now we can move this RSU to the right place
    if(stage == 2)
    {
        bool SumoMode =  simulation.getSystemModule()->par("SumoMode").boolValue();

        if(SumoMode)
        {
            cModule *nodePtr = FindModule<>::findHost(this);
            if(nodePtr == NULL)
                error("can not get a pointer to the module.");

            // ##########################################################################
            // connect to SUMO and get the position of RSUs

            cModule *mod = simulation.getSystemModule()->getSubmodule("traciManager");
            TraCIScenarioManager *traciPtr = check_and_cast<TraCIScenarioManager *>(mod);

            // get the omnet++ coordinates of this RSU from RSUs.poly.xml
            std::list<Coord> listCoord = traciPtr->commandGetPolygonShape(nodePtr->getFullName());

            Coord newPos = listCoord.front();

            // #########################################################################

            // update the position in runtime! (using methods in BaseMobility)
            move.setStart(newPos);
            BaseMobility::updatePosition();

            EV << "**** " << nodePtr->getFullName() << " is moved to position: ";
            EV << newPos.x << ", " << newPos.y << endl << endl;
        }
    }
}
