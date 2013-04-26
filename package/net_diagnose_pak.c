#include "package/net_diagnose_pak.h"
#include "model/net.h"
#include "package/net_experiment.h"

static int diagnose_net(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *ctx){
  net_diagnose(net);
  return 0;
}

Net_Exp_Pak *net_diagnose_package(){
  Net_Exp_Pak *pak = malloc(sizeof(Net_Exp_Pak));
  if(pak == NULL) return NULL;
  pak->opers[OPER_AFTER_CONNECT] = diagnose_net;
  pak->opers[OPER_AFTER_ATTACK] = diagnose_net;
  pak->opers[OPER_AFTER_EACH_LOOP] = diagnose_net;
  pak->opers[OPER_AFTER_STEADY] = diagnose_net;
  return pak;
}

