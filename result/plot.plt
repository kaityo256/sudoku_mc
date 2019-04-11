# ランダム探索と性能を比較する図
set term png 
set out "remc.png"
set xla "Time [ms]"
set yla "Rating [q2]"

set log xy
p "q2_with_ua.dat"w l t "Random Search"\
,"remc.dat"  w l t "REMC"
