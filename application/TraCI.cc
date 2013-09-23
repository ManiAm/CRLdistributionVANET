#include <TraCI.h>

Define_Module(TraCI);


void TraCI::initialize(int stage)
{
    bool SumoMode = simulation.getSystemModule()->par("SumoMode").boolValue();

    if (!SumoMode) return;

    TraCIScenarioManagerLaunchd::initialize(stage);
}


void TraCI::finish()
{
    bool SumoMode = simulation.getSystemModule()->par("SumoMode").boolValue();

    if (!SumoMode) return;

    TraCIScenarioManagerLaunchd::finish();
}


