g++ main.cpp -o main
for i in $(seq 1 200)
do
    ./main < input.txt >> output.txt
done 