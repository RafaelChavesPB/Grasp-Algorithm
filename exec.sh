rm output.txt 
for input in graphs/*
do
    ./main $input < $input >> output.txt
done