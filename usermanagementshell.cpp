#!/bin/bash

# Function to add user
add_user() {
    echo "Enter username to add:"
    read username

    sudo useradd $username

    if [ $? -eq 0 ]; then
        echo "User '$username' added successfully."
    else
        echo "Failed to add user."
    fi
}

# Function to delete user
delete_user() {
    echo "Enter username to delete:"
    read username

    sudo userdel $username

    if [ $? -eq 0 ]; then
        echo "User '$username' deleted successfully."
    else
        echo "Failed to delete user."
    fi
}

# Function to modify password
modify_password() {
    echo "Enter username to change password:"
    read username

    sudo passwd $username
}

# Function to display all existing users
display_users() {
    echo "Existing users in the system:"
    cut -d: -f1 /etc/passwd
}

# Main menu
while true
do
    echo "-----------------------------"
    echo " USER MANAGEMENT SYSTEM "
    echo "-----------------------------"
    echo "1. Add User"
    echo "2. Delete User"
    echo "3. Modify Password"
    echo "4. Display All Users"
    echo "5. Exit"
    echo "Enter your choice:"

    read choice

    case $choice in
        1)
            add_user
            ;;
        2)
            delete_user
            ;;
        3)
            modify_password
            ;;
        4)
            display_users
            ;;
        5)
            echo "Exiting..."
            break
            ;;
        *)
            echo "Invalid choice!"
            ;;
    esac
done

to run:
chmod +x user_management.sh
sudo ./user_management.sh