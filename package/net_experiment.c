#include "package/net_experiment.h"
#include "package/runner_param.h"
#include "package/runner.h"
#include "operator/lattice_guassi_connector.h"
#include "model/net.h"
#include "util/link.h"

struct net_experiment{
  Net_Exp_Pak **packages;
  int n_Paks;
};

Net_Exp *net_experiment_create(){
  Net_Exp *ret = malloc(sizeof(Net_Exp));
  if(ret == NULL) return NULL;
  ret->packages = NULL;
  ret->n_Paks = 0;
  return ret;
}

int net_experiment_add_package(Net_Exp_Pak *pak, Net_Exp *net_exp){
  if(net_exp->packages == NULL){
    net_exp->packages = malloc(sizeof(Net_Exp_Pak*));
  }else{
    net_exp->packages = realloc(net_exp->packages, net_exp->n_Paks*sizeof(Net_Exp_Pak*));
  }
  if(net_exp->packages == NULL) return 0;
  net_exp->packages[net_exp->n_Paks++] = pak;
  return 1;
}

static int run_oper(int oper_idx, Net *net, BC *bc, Exp_State *state, Net_Exp *net_exp){
  if(net_exp->n_Paks == 0) return 0;
  int i = 0;
  for(; i < net_exp->n_Paks; i++){
    Net_Exp_Pak *package = net_exp->packages[i];
    if(package->opers[oper_idx] != NULL){
      package->opers[oper_idx](net, bc, state, package);
    }
  }
  return 1;
}

int net_experiment_run(const char *config_file, Net_Exp *net_exp){
  Runner_param *param = runner_param_construct(config_file);

  if(param == NULL) return 0;
  //loop for n   <the net>
  //loop for epsilon  <the connector>
  //loop for alpha <the net runner>
  //loop for p  <the attacker>
  
  net_size_t i_n, i_ep, i_al, i_p, i_rep;
  
  Exp_State state;
  for(i_n = 0; i_n < param->num_n; i_n++){
    Net *net = net_create(4, param->N[i_n]);
    state.n = param->N[i_n];
    run_oper(OPER_AFTER_SIZE, net, NULL, &state, net_exp);
    for(i_ep = 0; i_ep < param->epsilon_n; i_ep++){
      Connector *conn = lattice_guassi_connector_create(param->epsilon[i_ep], time(0));
      state.epsilon = param->epsilon[i_ep];
      run_oper(OPER_AFTER_CONNECT, net, NULL, &state, net_exp);
      for(i_al = 0; i_al < param->alpha_n; i_al++){
	connector_connect(net, conn);
	Runner *runner = runner_create(net, param->alpha[i_al], RUNNER_CREATE_SILENT);
	state.alpha = param->alpha[i_al];
	run_oper(OPER_AFTER_CAPACITY, net, runner_getbc(runner), &state, net_exp);
	for(i_p = 0; i_p < param->p_n; i_p++){
	  int attack_num = ((1-param->p[i_p]) * param->N[i_n] + 0.5);
	  state.p = param->p[i_p];
	  for(i_rep = 0; i_rep < param->repeat; i_rep++){
	    /**
	     * first reconstruct the net
	     * then attack attack_num nodes
	     */
	    connector_connect(net, conn);
	    state.repeat = i_rep;
	    state.steps = 0;
	    //printf("load_break.c::construct net\n");
	    struct timeval time;
	    gettimeofday(&time, NULL);
	    //printf("%ld\n", time.tv_usec);
	    attack(attack_num, net, time.tv_usec);
	    
	    run_oper(OPER_AFTER_ATTACK, net, runner_getbc(runner), &state, net_exp);  

	    net_size_t broken;
	    while((broken = runner_loop(net, runner, RUNNER_LOOP_SILENT))){
	      state.steps ++;
	      run_oper(OPER_AFTER_EACH_LOOP, net, runner_getbc(runner), &state, net_exp);
	    }
	    run_oper(OPER_AFTER_STEADY, net, runner_getbc(runner), &state, net_exp);
	  }//i_rep
	}//i_p
	runner_destroy(runner);
      }//i_al
      connector_destroy(conn);
    }//i_ep
    run_oper(OPER_DESTROY, NULL, NULL, NULL, net_exp);
    net_destroy(net);
  }//i_n
}

void net_experiment_destroy(Net_Exp *net_exp){
  free(net_exp);
}
