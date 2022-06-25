rm grasp_result
g++ main.cpp -o main
for input in graphs/*
do
    ./main $input < $input >>  grasp_result
done