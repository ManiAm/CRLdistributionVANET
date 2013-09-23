#include <omnetpp.h>
#include "mobility/traci/TraCIScenarioManagerLaunchd.h"

class TraCI : public TraCIScenarioManagerLaunchd
{
	public:
		virtual void initialize(int stage);
		virtual void finish();
};

