//
// Generated file, do not edit! Created by opp_msgc 4.3 from msg/Beacon.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "Beacon_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(Beacon);

Beacon::Beacon(const char *name, int kind) : cPacket(name,kind)
{
    this->destAddr_var = -1;
    this->srcAddr_var = -1;
    this->NodeName_var = 0;
    this->positionX_var = 0;
    this->positionY_var = 0;
    this->speed_var = 0;
    this->NeedCRL_var = 0;
    this->CRL_Piece_No_var = 0;
    this->rangeS_var = 0;
    this->rangeE_var = 0;
    this->CRLchannel_var = 0;
}

Beacon::Beacon(const Beacon& other) : cPacket(other)
{
    copy(other);
}

Beacon::~Beacon()
{
}

Beacon& Beacon::operator=(const Beacon& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void Beacon::copy(const Beacon& other)
{
    this->destAddr_var = other.destAddr_var;
    this->srcAddr_var = other.srcAddr_var;
    this->NodeName_var = other.NodeName_var;
    this->positionX_var = other.positionX_var;
    this->positionY_var = other.positionY_var;
    this->speed_var = other.speed_var;
    this->NeedCRL_var = other.NeedCRL_var;
    this->CRL_Piece_No_var = other.CRL_Piece_No_var;
    this->rangeS_var = other.rangeS_var;
    this->rangeE_var = other.rangeE_var;
    this->CRLchannel_var = other.CRLchannel_var;
}

void Beacon::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->destAddr_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->NodeName_var);
    doPacking(b,this->positionX_var);
    doPacking(b,this->positionY_var);
    doPacking(b,this->speed_var);
    doPacking(b,this->NeedCRL_var);
    doPacking(b,this->CRL_Piece_No_var);
    doPacking(b,this->rangeS_var);
    doPacking(b,this->rangeE_var);
    doPacking(b,this->CRLchannel_var);
}

void Beacon::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->destAddr_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->NodeName_var);
    doUnpacking(b,this->positionX_var);
    doUnpacking(b,this->positionY_var);
    doUnpacking(b,this->speed_var);
    doUnpacking(b,this->NeedCRL_var);
    doUnpacking(b,this->CRL_Piece_No_var);
    doUnpacking(b,this->rangeS_var);
    doUnpacking(b,this->rangeE_var);
    doUnpacking(b,this->CRLchannel_var);
}

int Beacon::getDestAddr() const
{
    return destAddr_var;
}

void Beacon::setDestAddr(int destAddr)
{
    this->destAddr_var = destAddr;
}

int Beacon::getSrcAddr() const
{
    return srcAddr_var;
}

void Beacon::setSrcAddr(int srcAddr)
{
    this->srcAddr_var = srcAddr;
}

const char * Beacon::getNodeName() const
{
    return NodeName_var.c_str();
}

void Beacon::setNodeName(const char * NodeName)
{
    this->NodeName_var = NodeName;
}

double Beacon::getPositionX() const
{
    return positionX_var;
}

void Beacon::setPositionX(double positionX)
{
    this->positionX_var = positionX;
}

double Beacon::getPositionY() const
{
    return positionY_var;
}

void Beacon::setPositionY(double positionY)
{
    this->positionY_var = positionY;
}

double Beacon::getSpeed() const
{
    return speed_var;
}

void Beacon::setSpeed(double speed)
{
    this->speed_var = speed;
}

bool Beacon::getNeedCRL() const
{
    return NeedCRL_var;
}

void Beacon::setNeedCRL(bool NeedCRL)
{
    this->NeedCRL_var = NeedCRL;
}

int Beacon::getCRL_Piece_No() const
{
    return CRL_Piece_No_var;
}

void Beacon::setCRL_Piece_No(int CRL_Piece_No)
{
    this->CRL_Piece_No_var = CRL_Piece_No;
}

int Beacon::getRangeS() const
{
    return rangeS_var;
}

void Beacon::setRangeS(int rangeS)
{
    this->rangeS_var = rangeS;
}

int Beacon::getRangeE() const
{
    return rangeE_var;
}

void Beacon::setRangeE(int rangeE)
{
    this->rangeE_var = rangeE;
}

int Beacon::getCRLchannel() const
{
    return CRLchannel_var;
}

void Beacon::setCRLchannel(int CRLchannel)
{
    this->CRLchannel_var = CRLchannel;
}

class BeaconDescriptor : public cClassDescriptor
{
  public:
    BeaconDescriptor();
    virtual ~BeaconDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(BeaconDescriptor);

BeaconDescriptor::BeaconDescriptor() : cClassDescriptor("Beacon", "cPacket")
{
}

BeaconDescriptor::~BeaconDescriptor()
{
}

bool BeaconDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Beacon *>(obj)!=NULL;
}

const char *BeaconDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int BeaconDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 11+basedesc->getFieldCount(object) : 11;
}

unsigned int BeaconDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<11) ? fieldTypeFlags[field] : 0;
}

const char *BeaconDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destAddr",
        "srcAddr",
        "NodeName",
        "positionX",
        "positionY",
        "speed",
        "NeedCRL",
        "CRL_Piece_No",
        "rangeS",
        "rangeE",
        "CRLchannel",
    };
    return (field>=0 && field<11) ? fieldNames[field] : NULL;
}

int BeaconDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddr")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+1;
    if (fieldName[0]=='N' && strcmp(fieldName, "NodeName")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "positionX")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "positionY")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "speed")==0) return base+5;
    if (fieldName[0]=='N' && strcmp(fieldName, "NeedCRL")==0) return base+6;
    if (fieldName[0]=='C' && strcmp(fieldName, "CRL_Piece_No")==0) return base+7;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangeS")==0) return base+8;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangeE")==0) return base+9;
    if (fieldName[0]=='C' && strcmp(fieldName, "CRLchannel")==0) return base+10;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *BeaconDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "string",
        "double",
        "double",
        "double",
        "bool",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<11) ? fieldTypeStrings[field] : NULL;
}

const char *BeaconDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int BeaconDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string BeaconDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getDestAddr());
        case 1: return long2string(pp->getSrcAddr());
        case 2: return oppstring2string(pp->getNodeName());
        case 3: return double2string(pp->getPositionX());
        case 4: return double2string(pp->getPositionY());
        case 5: return double2string(pp->getSpeed());
        case 6: return bool2string(pp->getNeedCRL());
        case 7: return long2string(pp->getCRL_Piece_No());
        case 8: return long2string(pp->getRangeS());
        case 9: return long2string(pp->getRangeE());
        case 10: return long2string(pp->getCRLchannel());
        default: return "";
    }
}

bool BeaconDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestAddr(string2long(value)); return true;
        case 1: pp->setSrcAddr(string2long(value)); return true;
        case 2: pp->setNodeName((value)); return true;
        case 3: pp->setPositionX(string2double(value)); return true;
        case 4: pp->setPositionY(string2double(value)); return true;
        case 5: pp->setSpeed(string2double(value)); return true;
        case 6: pp->setNeedCRL(string2bool(value)); return true;
        case 7: pp->setCRL_Piece_No(string2long(value)); return true;
        case 8: pp->setRangeS(string2long(value)); return true;
        case 9: pp->setRangeE(string2long(value)); return true;
        case 10: pp->setCRLchannel(string2long(value)); return true;
        default: return false;
    }
}

const char *BeaconDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<11) ? fieldStructNames[field] : NULL;
}

void *BeaconDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


