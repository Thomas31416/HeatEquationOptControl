stats basename.'.dat' using 1:4

set terminal gif animate delay (500 / STATS_max_x)
set output basename.'.gif'

set zrange [STATS_min_y:STATS_max_y]

do for [n=0:STATS_max_x] {
    splot basename.'.dat' using 2:3:(column(1)==n?column(4):NaN) with lines title legend
}
