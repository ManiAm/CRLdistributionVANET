#include "ConstSpeedMobility.h"

class RSUmobility : public ConstSpeedMobility
{

public:
  virtual void initialize(int);
  virtual int numInitStages() const
  {
      return 3;
  }

};

