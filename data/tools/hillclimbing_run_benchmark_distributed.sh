cat -n hillclimbing_run_benchmark_all.sh | awk '{printf("ssh c0-%d \"%s\" &\n",$1%10,substr($0,8,length($0)))}' 
