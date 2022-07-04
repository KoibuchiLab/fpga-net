g++ schedule.cpp helper.cpp -o schedule
for i in 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 32
do
    ./schedule $i in> scheduleTable/kin$i
    ./schedule $i out> scheduleTable/kout$i
    ./schedule $i cmb> scheduleTable/kcmb$i
done
