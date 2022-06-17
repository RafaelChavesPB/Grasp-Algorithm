g++ main.cpp -o main && rm output.txt &&
for i in $(seq 1 10)
do
    ./main < input.txt >> output.txt
done 