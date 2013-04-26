model = net.o node.o path_node.o path_tree.o lattice_net.o
util  = array.o arraymap.o heap.o kvset.o set.o link.o
operator = connector.o simple_connector.o lattice_guassi_connector.o attack.o net_constructor.o
statistic = dijkstra.o clusters.o betweenness.o stat_on_lattice_net.o
package = runner.o load_break.o runner_param.o net_experiment.o net_diagnose_pak.o
CC    = gcc 
FLAG  = -g -lgmp -lm -I. 
tmp   = tmp
build = build

all:program

program:$(model) $(util) $(operator) $(statistic) $(package)

test_%:$(model) $(util) $(operator) $(statistic) $(package)
	$(CC) $(addprefix $(tmp)/, $^) test/$@.c -o $(build)/$@ $(FLAG)

$(model):
	$(CC) -c model/$(@:.o=.c) -o $(tmp)/$@ $(FLAG)

$(util):
	$(CC) -c util/$(@:.o=.c) -o $(tmp)/$@ $(FLAG)

$(operator):
	$(CC) -c operator/$(@:.o=.c) -o $(tmp)/$@ $(FLAG)

$(statistic):
	$(CC) -c statistic/$(@:.o=.c) -o $(tmp)/$@ $(FLAG)

$(package):
	$(CC) -c package/$(@:.o=.c) -o $(tmp)/$@ $(FLAG)

clean:
	rm $(tmp)/*
