find . -type f -iname "*.c" | while read -r file 
do
     echo "Compiling : $file"
    gcc -c -g "$file"
done
obj=$(find . -type f -iname "*.o")
gcc -lm -lSDL2 -g $obj -o main.out 
