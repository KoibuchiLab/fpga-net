g++ schedule.cpp helper.cpp -o schedule
for (( i = 2; i <= 13; i++))
do
    ./schedule $i in> kin$i
    ./schedule $i out> kout$i
done