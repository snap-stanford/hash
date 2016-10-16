perf stat -e branch-instructions,branch-misses,cache-misses,cache-references,context-switches,cpu-clock,faults,task-clock,cycles,instructions,stalled-cycles-backend,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,L1-icache-loads,LLC-load-misses,LLC-loads,dTLB-load-misses,dTLB-loads,iTLB-load-misses,iTLB-loads ./vec-test-01 100000

perf stat -e branch-instructions,branch-misses,cache-misses,cache-references,context-switches,cpu-clock,faults,task-clock,cycles,instructions,stalled-cycles-backend,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,L1-icache-loads,LLC-load-misses,LLC-loads,dTLB-load-misses,dTLB-loads,iTLB-load-misses,iTLB-loads ./vec-test-01 1000000

perf stat -e branch-instructions,branch-misses,cache-misses,cache-references,context-switches,cpu-clock,faults,task-clock,cycles,instructions,stalled-cycles-backend,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,L1-icache-loads,LLC-load-misses,LLC-loads,dTLB-load-misses,dTLB-loads,iTLB-load-misses,iTLB-loads ./vec-test-01 10000000

perf stat -e branch-instructions,branch-misses,cache-misses,cache-references,context-switches,cpu-clock,faults,task-clock,cycles,instructions,stalled-cycles-backend,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,L1-icache-loads,LLC-load-misses,LLC-loads,dTLB-load-misses,dTLB-loads,iTLB-load-misses,iTLB-loads ./vec-test-01 100000000

perf stat -e branch-instructions,branch-misses,cache-misses,cache-references,context-switches,cpu-clock,faults,task-clock,cycles,instructions,stalled-cycles-backend,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,L1-icache-loads,LLC-load-misses,LLC-loads,dTLB-load-misses,dTLB
-loads,iTLB-load-misses,iTLB-loads ./vec-test-01 1000000000

perf stat -e branch-instructions,branch-misses,cache-misses,cache-references,context-switches,cpu-clock,faults,task-clock,cycles,instructions,stalled-cycles-backend,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,L1-icache-loads,LLC-load-misses,LLC-loads,dTLB-load-misses,dTLB
-loads,iTLB-load-misses,iTLB-loads ./vec-test-01 10000000000

perf stat -e branch-instructions,branch-misses,cache-misses,cache-references,context-switches,cpu-clock,faults,task-clock,cycles,instructions,stalled-cycles-backend,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,L1-icache-loads,LLC-load-misses,LLC-loads,dTLB-load-misses,dTLB
-loads,iTLB-load-misses,iTLB-loads ./vec-test-01 100000000000



