//
//  IncludeC.h
//  client
//
//  Created by liulihua on 14-9-22.
//
//

#ifndef _PBC_INCLUDE_H_
#define _PBC_INCLUDE_H_


#ifdef WIN32
#include "pbc.h"
#else
extern "C" { 
#include "pbc.h"
}
#endif // WIN32







#endif
