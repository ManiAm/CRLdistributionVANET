//
// Generated file, do not edit! Created by opp_msgc 4.3 from msg/Piece_list.msg.
//

#ifndef _PIECE_LIST_M_H_
#define _PIECE_LIST_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0403
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>msg/Piece_list.msg</tt> by opp_msgc.
 * <pre>
 * packet Piece_list
 * {
 *     int serial;
 *     string list;
 * }
 * </pre>
 */
class Piece_list : public ::cPacket
{
  protected:
    int serial_var;
    opp_string list_var;

  private:
    void copy(const Piece_list& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Piece_list&);

  public:
    Piece_list(const char *name=NULL, int kind=0);
    Piece_list(const Piece_list& other);
    virtual ~Piece_list();
    Piece_list& operator=(const Piece_list& other);
    virtual Piece_list *dup() const {return new Piece_list(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getSerial() const;
    virtual void setSerial(int serial);
    virtual const char * getList() const;
    virtual void setList(const char * list);
};

inline void doPacking(cCommBuffer *b, Piece_list& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Piece_list& obj) {obj.parsimUnpack(b);}


#endif // _PIECE_LIST_M_H_
