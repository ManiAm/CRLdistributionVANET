//
// Generated file, do not edit! Created by opp_msgc 4.3 from msg/CRL_Piece.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "CRL_Piece_m.h"

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




Register_Class(CRL_Piece);

CRL_Piece::CRL_Piece(const char *name, int kind) : cPacket(name,kind)
{
    this->CRLversion_var = 0;
    this->Timestamp_var = 0;
    this->SeqNo_var = 0;
    this->CAid_var = 0;
    this->pad_var = 0;
    this->payload_var = 0;
}

CRL_Piece::CRL_Piece(const CRL_Piece& other) : cPacket(other)
{
    copy(other);
}

CRL_Piece::~CRL_Piece()
{
}

CRL_Piece& CRL_Piece::operator=(const CRL_Piece& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void CRL_Piece::copy(const CRL_Piece& other)
{
    this->CRLversion_var = other.CRLversion_var;
    this->Timestamp_var = other.Timestamp_var;
    this->SeqNo_var = other.SeqNo_var;
    this->CAid_var = other.CAid_var;
    this->pad_var = other.pad_var;
    this->payload_var = other.payload_var;
}

void CRL_Piece::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->CRLversion_var);
    doPacking(b,this->Timestamp_var);
    doPacking(b,this->SeqNo_var);
    doPacking(b,this->CAid_var);
    doPacking(b,this->pad_var);
    doPacking(b,this->payload_var);
}

void CRL_Piece::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->CRLversion_var);
    doUnpacking(b,this->Timestamp_var);
    doUnpacking(b,this->SeqNo_var);
    doUnpacking(b,this->CAid_var);
    doUnpacking(b,this->pad_var);
    doUnpacking(b,this->payload_var);
}

int CRL_Piece::getCRLversion() const
{
    return CRLversion_var;
}

void CRL_Piece::setCRLversion(int CRLversion)
{
    this->CRLversion_var = CRLversion;
}

int CRL_Piece::getTimestamp() const
{
    return Timestamp_var;
}

void CRL_Piece::setTimestamp(int Timestamp)
{
    this->Timestamp_var = Timestamp;
}

int CRL_Piece::getSeqNo() const
{
    return SeqNo_var;
}

void CRL_Piece::setSeqNo(int SeqNo)
{
    this->SeqNo_var = SeqNo;
}

int CRL_Piece::getCAid() const
{
    return CAid_var;
}

void CRL_Piece::setCAid(int CAid)
{
    this->CAid_var = CAid;
}

int CRL_Piece::getPad() const
{
    return pad_var;
}

void CRL_Piece::setPad(int pad)
{
    this->pad_var = pad;
}

const char * CRL_Piece::getPayload() const
{
    return payload_var.c_str();
}

void CRL_Piece::setPayload(const char * payload)
{
    this->payload_var = payload;
}

class CRL_PieceDescriptor : public cClassDescriptor
{
  public:
    CRL_PieceDescriptor();
    virtual ~CRL_PieceDescriptor();

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

Register_ClassDescriptor(CRL_PieceDescriptor);

CRL_PieceDescriptor::CRL_PieceDescriptor() : cClassDescriptor("CRL_Piece", "cPacket")
{
}

CRL_PieceDescriptor::~CRL_PieceDescriptor()
{
}

bool CRL_PieceDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CRL_Piece *>(obj)!=NULL;
}

const char *CRL_PieceDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CRL_PieceDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int CRL_PieceDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *CRL_PieceDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "CRLversion",
        "Timestamp",
        "SeqNo",
        "CAid",
        "pad",
        "payload",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int CRL_PieceDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='C' && strcmp(fieldName, "CRLversion")==0) return base+0;
    if (fieldName[0]=='T' && strcmp(fieldName, "Timestamp")==0) return base+1;
    if (fieldName[0]=='S' && strcmp(fieldName, "SeqNo")==0) return base+2;
    if (fieldName[0]=='C' && strcmp(fieldName, "CAid")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "pad")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "payload")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CRL_PieceDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
        "int",
        "int",
        "string",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *CRL_PieceDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CRL_PieceDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CRL_PieceDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCRLversion());
        case 1: return long2string(pp->getTimestamp());
        case 2: return long2string(pp->getSeqNo());
        case 3: return long2string(pp->getCAid());
        case 4: return long2string(pp->getPad());
        case 5: return oppstring2string(pp->getPayload());
        default: return "";
    }
}

bool CRL_PieceDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        case 0: pp->setCRLversion(string2long(value)); return true;
        case 1: pp->setTimestamp(string2long(value)); return true;
        case 2: pp->setSeqNo(string2long(value)); return true;
        case 3: pp->setCAid(string2long(value)); return true;
        case 4: pp->setPad(string2long(value)); return true;
        case 5: pp->setPayload((value)); return true;
        default: return false;
    }
}

const char *CRL_PieceDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<6) ? fieldStructNames[field] : NULL;
}

void *CRL_PieceDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


