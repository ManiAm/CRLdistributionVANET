//
// Generated file, do not edit! Created by opp_msgc 4.3 from msg/SimpleMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SimpleMsg_m.h"

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




Register_Class(SimpleMsg);

SimpleMsg::SimpleMsg(const char *name, int kind) : cPacket(name,kind)
{
    this->payload_var = 0;
    this->signature_var = 0;
    this->certLTfrom_var = 0;
    this->certLTto_var = 0;
}

SimpleMsg::SimpleMsg(const SimpleMsg& other) : cPacket(other)
{
    copy(other);
}

SimpleMsg::~SimpleMsg()
{
}

SimpleMsg& SimpleMsg::operator=(const SimpleMsg& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void SimpleMsg::copy(const SimpleMsg& other)
{
    this->payload_var = other.payload_var;
    this->signature_var = other.signature_var;
    this->certLTfrom_var = other.certLTfrom_var;
    this->certLTto_var = other.certLTto_var;
}

void SimpleMsg::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->payload_var);
    doPacking(b,this->signature_var);
    doPacking(b,this->certLTfrom_var);
    doPacking(b,this->certLTto_var);
}

void SimpleMsg::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->payload_var);
    doUnpacking(b,this->signature_var);
    doUnpacking(b,this->certLTfrom_var);
    doUnpacking(b,this->certLTto_var);
}

const char * SimpleMsg::getPayload() const
{
    return payload_var.c_str();
}

void SimpleMsg::setPayload(const char * payload)
{
    this->payload_var = payload;
}

const char * SimpleMsg::getSignature() const
{
    return signature_var.c_str();
}

void SimpleMsg::setSignature(const char * signature)
{
    this->signature_var = signature;
}

double SimpleMsg::getCertLTfrom() const
{
    return certLTfrom_var;
}

void SimpleMsg::setCertLTfrom(double certLTfrom)
{
    this->certLTfrom_var = certLTfrom;
}

double SimpleMsg::getCertLTto() const
{
    return certLTto_var;
}

void SimpleMsg::setCertLTto(double certLTto)
{
    this->certLTto_var = certLTto;
}

class SimpleMsgDescriptor : public cClassDescriptor
{
  public:
    SimpleMsgDescriptor();
    virtual ~SimpleMsgDescriptor();

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

Register_ClassDescriptor(SimpleMsgDescriptor);

SimpleMsgDescriptor::SimpleMsgDescriptor() : cClassDescriptor("SimpleMsg", "cPacket")
{
}

SimpleMsgDescriptor::~SimpleMsgDescriptor()
{
}

bool SimpleMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SimpleMsg *>(obj)!=NULL;
}

const char *SimpleMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SimpleMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int SimpleMsgDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *SimpleMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "payload",
        "signature",
        "certLTfrom",
        "certLTto",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int SimpleMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "payload")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "signature")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "certLTfrom")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "certLTto")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SimpleMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "double",
        "double",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *SimpleMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SimpleMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SimpleMsg *pp = (SimpleMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SimpleMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleMsg *pp = (SimpleMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getPayload());
        case 1: return oppstring2string(pp->getSignature());
        case 2: return double2string(pp->getCertLTfrom());
        case 3: return double2string(pp->getCertLTto());
        default: return "";
    }
}

bool SimpleMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SimpleMsg *pp = (SimpleMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setPayload((value)); return true;
        case 1: pp->setSignature((value)); return true;
        case 2: pp->setCertLTfrom(string2double(value)); return true;
        case 3: pp->setCertLTto(string2double(value)); return true;
        default: return false;
    }
}

const char *SimpleMsgDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *SimpleMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleMsg *pp = (SimpleMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


