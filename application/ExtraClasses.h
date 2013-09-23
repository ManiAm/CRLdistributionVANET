#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>


class Certificate
{
public:
    std::string CerName;

    std::string CAname;
    int CAid;

    std::string NodeName;
    int NodeID;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & CerName;
        ar & CAname;
        ar & CAid;
        ar & NodeName;
        ar & NodeID;
    }
};


class NodeEntry
{
  public:
	char name[10];
	int nodeID;  // is used to sort the vector (used as a key)
    int count;   // it can be NoCRLs or NoNewRSUs
	simtime_t time;
    simtime_t ExitTime;

	NodeEntry(const char *str, int id, int n, simtime_t t, simtime_t t2)
	{
		strcpy(this->name, str);
		this->nodeID = id;
        this->count = n;
		this->time = t;
        this->ExitTime = t2;
	}
};


// used to send data from CA to magic cars
class MagicCarsData : public cObject, noncopyable
{
  public:
	char name[10];
	std::vector<CRL_Piece *> data;


	MagicCarsData(const char *str, std::vector<CRL_Piece *> d)
	{
		strcpy(this->name, str);
		this->data = d;
	}
};


// used  in false msg propagation scenario
class RxMsg : public cObject, noncopyable
{
  public:
    char name[10];
    simtime_t time;
    double TSfrom;
    double TSto;

    RxMsg(const char *str,  simtime_t t, double from, double to)
    {
        strcpy(this->name, str);
        this->time = t;
        this->TSfrom = from;
        this->TSto = to;
    }
};

