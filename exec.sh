g++ main.cpp -o main
for i in $(seq 1 5)
do
    time ./main < input.txt > $i
    echo $i
done 