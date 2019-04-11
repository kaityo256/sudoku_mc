set term png
set out "mc.png"

set yla "Rating [q2]"
set xla "Steps"
set log y
set key at 700,100
p "test5_00.dat" w l t"Low Temperature"\
 ,"test0_05.dat" w l t"Middle Temperature"\
 ,"test0_01.dat" w l t"High Temperature"
