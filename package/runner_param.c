#include "package/runner_param.h"

#include <stdio.h>

/*****************************************************************
 * the configure file format should be:
 * 
 * [line 1]count_of_nodes, default_degree
 * [line 2]epsilon
 * [line 3]alpha
 * [line 4]p_start, p_end, p_increment
 * [line 5]repeat
 *****************************************************************/
#define SIZE 1024

void runner_param_destroy(Runner_param *param){
	if(param->N) free(param->N);
	if(param->epsilon) free(param->epsilon);
	if(param->alpha) free(param->alpha);
	if(param->p) free(param->p);

        param->N = NULL;
	param->epsilon = NULL;
	param->alpha = NULL;
	param->p = NULL;
	free(param);
}

Runner_param *runner_param_construct(const char *path){
	FILE *file = fopen(path, "r");
	if(file == NULL) return NULL;
	char line[SIZE];
	Runner_param *param = malloc(sizeof(Runner_param));
	int nodes_count, nodes_degree;
	if(fgets(line, SIZE, file)){
		sscanf(line, "%d,%d", &nodes_count, &nodes_degree);
		double epsilon;
		if(fgets(line, SIZE, file)){
			sscanf(line, "%lf", &epsilon);
                        double alpha;
			if(fgets(line, SIZE, file)){
				sscanf(line,"%lf", &alpha);
				double p_start, p_end, p_increment;
				if(fgets(line, SIZE, file)){
					sscanf(line, "%lf,%lf,%lf",&p_start,&p_end,&p_increment);
					int repeat;
					if(fgets(line, SIZE, file)){
						sscanf(line,"%d",&repeat);
						
						/*********
                                                 * construct param
						 */
						param->num_n = 1;
  						param->N = malloc(param->num_n * sizeof(net_size_t));
  						param->N[0] = nodes_count;

  						param->epsilon_n = 1;
  						param->epsilon = malloc(param->epsilon_n * sizeof(double));
  						param->epsilon[0] = epsilon;

  						param->alpha_n = 1;
  						param->alpha = malloc(param->alpha_n * sizeof(double));
  						param->alpha[0] = alpha;

  						param->p_n = (p_end - p_start)/p_increment + 1;
  						param->p = malloc(param->p_n * sizeof(double));
  						net_size_t i;
  						for(i = 0;i < param->p_n; i++){
    							param->p[i] = p_start + p_increment * i;
  						}

  						param->repeat = repeat;
						fclose(file);
						return param;
					}
				}
			}
		}
	}
	fclose(file);
	runner_param_destroy(param);
	return NULL;
}
