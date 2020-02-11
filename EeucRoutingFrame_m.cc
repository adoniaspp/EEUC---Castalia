//
// Generated file, do not edit! Created by opp_msgc 4.1 from src/node/communication/routing/EeucRouting/EeucRoutingFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "EeucRoutingFrame_m.h"

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




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("EEUCRouting_FrameType");
    if (!e) enums.getInstance()->add(e = new cEnum("EEUCRouting_FrameType"));
    e->insert(EEUC_ROUTING_COMPETE_HEAD_FRAME, "EEUC_ROUTING_COMPETE_HEAD_FRAME");
    e->insert(EEUC_ROUTING_FINALHEAD_FRAME, "EEUC_ROUTING_FINALHEAD_FRAME");
    e->insert(EEUC_ROUTING_QUITELECTION_FRAME, "EEUC_ROUTING_QUITELECTION_FRAME");
    e->insert(EEUC_ROUTING_CH_ADV_FRAME, "EEUC_ROUTING_CH_ADV_FRAME");
    e->insert(EEUC_ROUTING_CH_ADV_RELAY_FRAME, "EEUC_ROUTING_CH_ADV_RELAY_FRAME");
    e->insert(EEUC_ROUTING_JOIN_FRAME, "EEUC_ROUTING_JOIN_FRAME");
    e->insert(EEUC_ROUTING_SCHE_FRAME, "EEUC_ROUTING_SCHE_FRAME");
    e->insert(EEUC_ROUTING_AGGR_FRAME, "EEUC_ROUTING_AGGR_FRAME");
    e->insert(EEUC_ROUTING_DATA_FRAME, "EEUC_ROUTING_DATA_FRAME");
    e->insert(MAX_CLUSTER_SIZE, "MAX_CLUSTER_SIZE");
);

Register_Class(EEUCRouting_DataFrame);

EEUCRouting_DataFrame::EEUCRouting_DataFrame(const char *name, int kind) : RoutingPacket(name,kind)
{
    this->EeucRoutingPacketKind_var = 0;
    this->clusterLength_var = 0;
    for (unsigned int i=0; i<MAX_CLUSTER_SIZE; i++)
        this->schedule_var[i] = 0;
    aggregate_arraysize = 0;
    this->aggregate_var = 0;
    this->energy_var = 0;
    this->rcomp_var = 0;
    this->distanceBS_var = 0;
    this->posX_var = 0;
    this->posY_var = 0;
}

EEUCRouting_DataFrame::EEUCRouting_DataFrame(const EEUCRouting_DataFrame& other) : RoutingPacket()
{
    setName(other.getName());
    aggregate_arraysize = 0;
    this->aggregate_var = 0;
    operator=(other);
}

EEUCRouting_DataFrame::~EEUCRouting_DataFrame()
{
    for (unsigned int i=0; i<aggregate_arraysize; i++)
        drop(&(this->aggregate_var[i]));
    delete [] aggregate_var;
}

EEUCRouting_DataFrame& EEUCRouting_DataFrame::operator=(const EEUCRouting_DataFrame& other)
{
    if (this==&other) return *this;
    RoutingPacket::operator=(other);
    this->EeucRoutingPacketKind_var = other.EeucRoutingPacketKind_var;
    this->clusterLength_var = other.clusterLength_var;
    for (unsigned int i=0; i<MAX_CLUSTER_SIZE; i++)
        this->schedule_var[i] = other.schedule_var[i];
    delete [] this->aggregate_var;
    this->aggregate_var = (other.aggregate_arraysize==0) ? NULL : new ::RoutingPacket[other.aggregate_arraysize];
    aggregate_arraysize = other.aggregate_arraysize;
    for (unsigned int i=0; i<aggregate_arraysize; i++)
    {
        take(&(this->aggregate_var[i]));
        this->aggregate_var[i] = other.aggregate_var[i];
        this->aggregate_var[i].setName(other.aggregate_var[i].getName());
    }
    this->energy_var = other.energy_var;
    this->rcomp_var = other.rcomp_var;
    this->distanceBS_var = other.distanceBS_var;
    this->posX_var = other.posX_var;
    this->posY_var = other.posY_var;
    return *this;
}

void EEUCRouting_DataFrame::parsimPack(cCommBuffer *b)
{
    RoutingPacket::parsimPack(b);
    doPacking(b,this->EeucRoutingPacketKind_var);
    doPacking(b,this->clusterLength_var);
    doPacking(b,this->schedule_var,MAX_CLUSTER_SIZE);
    b->pack(aggregate_arraysize);
    doPacking(b,this->aggregate_var,aggregate_arraysize);
    doPacking(b,this->energy_var);
    doPacking(b,this->rcomp_var);
    doPacking(b,this->distanceBS_var);
    doPacking(b,this->posX_var);
    doPacking(b,this->posY_var);
}

void EEUCRouting_DataFrame::parsimUnpack(cCommBuffer *b)
{
    RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->EeucRoutingPacketKind_var);
    doUnpacking(b,this->clusterLength_var);
    doUnpacking(b,this->schedule_var,MAX_CLUSTER_SIZE);
    delete [] this->aggregate_var;
    b->unpack(aggregate_arraysize);
    if (aggregate_arraysize==0) {
        this->aggregate_var = 0;
    } else {
        this->aggregate_var = new ::RoutingPacket[aggregate_arraysize];
        doUnpacking(b,this->aggregate_var,aggregate_arraysize);
    }
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->rcomp_var);
    doUnpacking(b,this->distanceBS_var);
    doUnpacking(b,this->posX_var);
    doUnpacking(b,this->posY_var);
}

int EEUCRouting_DataFrame::getEeucRoutingPacketKind() const
{
    return EeucRoutingPacketKind_var;
}

void EEUCRouting_DataFrame::setEeucRoutingPacketKind(int EeucRoutingPacketKind_var)
{
    this->EeucRoutingPacketKind_var = EeucRoutingPacketKind_var;
}

int EEUCRouting_DataFrame::getClusterLength() const
{
    return clusterLength_var;
}

void EEUCRouting_DataFrame::setClusterLength(int clusterLength_var)
{
    this->clusterLength_var = clusterLength_var;
}

unsigned int EEUCRouting_DataFrame::getScheduleArraySize() const
{
    return MAX_CLUSTER_SIZE;
}

int EEUCRouting_DataFrame::getSchedule(unsigned int k) const
{
    if (k>=MAX_CLUSTER_SIZE) throw cRuntimeError("Array of size MAX_CLUSTER_SIZE indexed by %lu", (unsigned long)k);
    return schedule_var[k];
}

void EEUCRouting_DataFrame::setSchedule(unsigned int k, int schedule_var)
{
    if (k>=MAX_CLUSTER_SIZE) throw cRuntimeError("Array of size MAX_CLUSTER_SIZE indexed by %lu", (unsigned long)k);
    this->schedule_var[k] = schedule_var;
}

void EEUCRouting_DataFrame::setAggregateArraySize(unsigned int size)
{
    ::RoutingPacket *aggregate_var2 = (size==0) ? NULL : new ::RoutingPacket[size];
    unsigned int sz = aggregate_arraysize < size ? aggregate_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        aggregate_var2[i] = this->aggregate_var[i];
    for (unsigned int i=sz; i<size; i++)
        take(&(aggregate_var2[i]));
    aggregate_arraysize = size;
    delete [] this->aggregate_var;
    this->aggregate_var = aggregate_var2;
}

unsigned int EEUCRouting_DataFrame::getAggregateArraySize() const
{
    return aggregate_arraysize;
}

RoutingPacket& EEUCRouting_DataFrame::getAggregate(unsigned int k)
{
    if (k>=aggregate_arraysize) throw cRuntimeError("Array of size %d indexed by %d", aggregate_arraysize, k);
    return aggregate_var[k];
}

void EEUCRouting_DataFrame::setAggregate(unsigned int k, const RoutingPacket& aggregate_var)
{
    if (k>=aggregate_arraysize) throw cRuntimeError("Array of size %d indexed by %d", aggregate_arraysize, k);
    this->aggregate_var[k]=aggregate_var;
}

double EEUCRouting_DataFrame::getEnergy() const
{
    return energy_var;
}

void EEUCRouting_DataFrame::setEnergy(double energy_var)
{
    this->energy_var = energy_var;
}

double EEUCRouting_DataFrame::getRcomp() const
{
    return rcomp_var;
}

void EEUCRouting_DataFrame::setRcomp(double rcomp_var)
{
    this->rcomp_var = rcomp_var;
}

double EEUCRouting_DataFrame::getDistanceBS() const
{
    return distanceBS_var;
}

void EEUCRouting_DataFrame::setDistanceBS(double distanceBS_var)
{
    this->distanceBS_var = distanceBS_var;
}

double EEUCRouting_DataFrame::getPosX() const
{
    return posX_var;
}

void EEUCRouting_DataFrame::setPosX(double posX_var)
{
    this->posX_var = posX_var;
}

double EEUCRouting_DataFrame::getPosY() const
{
    return posY_var;
}

void EEUCRouting_DataFrame::setPosY(double posY_var)
{
    this->posY_var = posY_var;
}

class EEUCRouting_DataFrameDescriptor : public cClassDescriptor
{
  public:
    EEUCRouting_DataFrameDescriptor();
    virtual ~EEUCRouting_DataFrameDescriptor();

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

Register_ClassDescriptor(EEUCRouting_DataFrameDescriptor);

EEUCRouting_DataFrameDescriptor::EEUCRouting_DataFrameDescriptor() : cClassDescriptor("EEUCRouting_DataFrame", "RoutingPacket")
{
}

EEUCRouting_DataFrameDescriptor::~EEUCRouting_DataFrameDescriptor()
{
}

bool EEUCRouting_DataFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<EEUCRouting_DataFrame *>(obj)!=NULL;
}

const char *EEUCRouting_DataFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int EEUCRouting_DataFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount(object) : 9;
}

unsigned int EEUCRouting_DataFrameDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISCOBJECT | FD_ISCOWNEDOBJECT,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *EEUCRouting_DataFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "EeucRoutingPacketKind",
        "clusterLength",
        "schedule",
        "aggregate",
        "energy",
        "rcomp",
        "distanceBS",
        "posX",
        "posY",
    };
    return (field>=0 && field<9) ? fieldNames[field] : NULL;
}

int EEUCRouting_DataFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='E' && strcmp(fieldName, "EeucRoutingPacketKind")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "clusterLength")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "schedule")==0) return base+2;
    if (fieldName[0]=='a' && strcmp(fieldName, "aggregate")==0) return base+3;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "rcomp")==0) return base+5;
    if (fieldName[0]=='d' && strcmp(fieldName, "distanceBS")==0) return base+6;
    if (fieldName[0]=='p' && strcmp(fieldName, "posX")==0) return base+7;
    if (fieldName[0]=='p' && strcmp(fieldName, "posY")==0) return base+8;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *EEUCRouting_DataFrameDescriptor::getFieldTypeString(void *object, int field) const
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
        "RoutingPacket",
        "double",
        "double",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : NULL;
}

const char *EEUCRouting_DataFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "EEUCRouting_FrameType";
            return NULL;
        default: return NULL;
    }
}

int EEUCRouting_DataFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    EEUCRouting_DataFrame *pp = (EEUCRouting_DataFrame *)object; (void)pp;
    switch (field) {
        case 2: return MAX_CLUSTER_SIZE;
        case 3: return pp->getAggregateArraySize();
        default: return 0;
    }
}

std::string EEUCRouting_DataFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    EEUCRouting_DataFrame *pp = (EEUCRouting_DataFrame *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getEeucRoutingPacketKind());
        case 1: return long2string(pp->getClusterLength());
        case 2: return long2string(pp->getSchedule(i));
        case 3: {std::stringstream out; out << pp->getAggregate(i); return out.str();}
        case 4: return double2string(pp->getEnergy());
        case 5: return double2string(pp->getRcomp());
        case 6: return double2string(pp->getDistanceBS());
        case 7: return double2string(pp->getPosX());
        case 8: return double2string(pp->getPosY());
        default: return "";
    }
}

bool EEUCRouting_DataFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    EEUCRouting_DataFrame *pp = (EEUCRouting_DataFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setEeucRoutingPacketKind(string2long(value)); return true;
        case 1: pp->setClusterLength(string2long(value)); return true;
        case 2: pp->setSchedule(i,string2long(value)); return true;
        case 4: pp->setEnergy(string2double(value)); return true;
        case 5: pp->setRcomp(string2double(value)); return true;
        case 6: pp->setDistanceBS(string2double(value)); return true;
        case 7: pp->setPosX(string2double(value)); return true;
        case 8: pp->setPosY(string2double(value)); return true;
        default: return false;
    }
}

const char *EEUCRouting_DataFrameDescriptor::getFieldStructName(void *object, int field) const
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
        "RoutingPacket",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<9) ? fieldStructNames[field] : NULL;
}

void *EEUCRouting_DataFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    EEUCRouting_DataFrame *pp = (EEUCRouting_DataFrame *)object; (void)pp;
    switch (field) {
        case 3: return (void *)static_cast<cObject *>(&pp->getAggregate(i)); break;
        default: return NULL;
    }
}


