// $Id$

// ******  Code generated by the The ACE ORB (TAO) IDL Compiler *******
// TAO ORB and the TAO IDL Compiler have been developed by Washington 
// University Computer Science's Distributed Object Computing Group.
//
// Information on TAO is available at
//                 http://www.cs.wustl.edu/~schmidt/TAO.html

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
#include "Object_KeyC.i"
#endif // !defined INLINE

ACE_RCSID(tao, Object_KeyC, "$Id$")

#if !defined (_TAO__TAO_SEQ_OCTET_CS_)
#define _TAO__TAO_SEQ_OCTET_CS_

const CORBA::Long _oc_TAO__tao_seq_Octet[] =
{
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_octet,

  0,
};
CORBA::TypeCode _tc__tc_TAO__tao_seq_Octet (CORBA::tk_sequence, sizeof (_oc_TAO__tao_seq_Octet), (char *) &_oc_TAO__tao_seq_Octet, 0);
CORBA::TypeCode_ptr TAO_tc__tao_seq_Octet = &_tc__tc_TAO__tao_seq_Octet;


#endif // end #if !defined

const CORBA::Long _oc_TAO_ObjectKey[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  22, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x54414f2f), ACE_NTOHL (0x4f626a65), ACE_NTOHL (0x63744b65), ACE_NTOHL (0x793a312e), ACE_NTOHL (0x3000fdfd),  // repository ID = IDL:TAO/ObjectKey:1.0
  10, ACE_NTOHL (0x4f626a65), ACE_NTOHL (0x63744b65), ACE_NTOHL (0x7900fdfd),  // name = ObjectKey
  CORBA::tk_sequence, // typecode kind
  12, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_octet,

  0,
};
CORBA::TypeCode _tc__tc_TAO_ObjectKey (CORBA::tk_alias, sizeof (_oc_TAO_ObjectKey), (char *) &_oc_TAO_ObjectKey, 0);
CORBA::TypeCode_ptr TAO_tc_ObjectKey = &_tc__tc_TAO_ObjectKey;

