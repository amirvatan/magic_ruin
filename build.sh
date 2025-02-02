find . -type f -iname "*.c" | while read -r file 
do
     echo "Compiling : $file"
    gcc -c "$file"
done
obj=$(find . -type f -iname "*.o")
gcc -lSDL2 -g $obj -o main.out 
