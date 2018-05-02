// nat.h contains the Network Address Translation information

#ifndef _NAT_H
#define _NAT_H

#include "internet.h"

struct nat {
      unsigned char proxy_input_subnet[IP_ADDRESS_LEN];
      unsigned char proxy_output_src_subnet[IP_ADDRESS_LEN];
      unsigned char proxy_output_dest_subnet[IP_ADDRESS_LEN];
      unsigned char proxy_subnet_mask[IP_ADDRESS_LEN];
};

extern struct nat nat_table[];
#endif // _NAT_H
