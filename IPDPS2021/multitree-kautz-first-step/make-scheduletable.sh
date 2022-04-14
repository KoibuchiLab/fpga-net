g++ schedule.cpp helper.cpp -o schedule
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 16 32 64
do
    ./schedule $i in> scheduleTable/kin$i
    ./schedule $i out> scheduleTable/kout$i
    ./schedule $i cmb> scheduleTable/kcmb$i
done
