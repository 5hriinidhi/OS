#!/bin/bash

# Functions for arithmetic operations

add() {
    echo "Result = $(($1 + $2))"
}

sub() {
    echo "Result = $(($1 - $2))"
}

mul() {
    echo "Result = $(($1 * $2))"
}

div() {
    if [ $2 -eq 0 ]; then
        echo "Division by zero is not possible"
    else
        echo "Result = $(($1 / $2))"
    fi
}

# Input numbers
echo "Enter first number:"
read a

echo "Enter second number:"
read b

# Menu
echo "Choose operation:"
echo "1. Addition"
echo "2. Subtraction"
echo "3. Multiplication"
echo "4. Division"

read choice

# Case statement
case $choice in
    1)
        add $a $b
        ;;
    2)
        sub $a $b
        ;;
    3)
        mul $a $b
        ;;
    4)
        div $a $b
        ;;
    *)
        echo "Invalid choice"
        ;;
esac

run using:
chmod +x calculator.sh
./calculator.sh