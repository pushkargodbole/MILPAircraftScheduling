# This is a file to compile the cplex codes in cpp
# Give the filename without the '.cpp' extension as the input
# Keep this file in the same directory as the cpp file
# An executable by the same name as the file will be created in the working directory
if [ $# -lt 1 ]; then
    echo "Run the script with filename as arguement"  
    exit
fi

FILENAME=$1

g++ -c -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -I/pkgs/ilog/cplex125/cplex/include -I/pkgs/ilog/cplex125/concert/include $FILENAME.cpp -o $FILENAME.o

g++ -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -I/pkgs/ilog/cplex125/cplex/include -I/pkgs/ilog/cplex125/concert/include $FILENAME.o -o $FILENAME -L/pkgs/ilog/cplex125/cplex/lib/x86-64_sles10_4.1/static_pic -lilocplex -lcplex -L/pkgs/ilog/cplex125/concert/lib/x86-64_sles10_4.1/static_pic -lconcert -lm -pthread

