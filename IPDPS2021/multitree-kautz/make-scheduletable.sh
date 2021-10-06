g++ schedule.cpp helper.cpp -o schedule
for (( i = 2; i <= 13; i++))
do
    ./schedule $i > k$i
done