#!/bin/bash

# Main script
echo "21-XXXX-NAME"

# Function to list installed applications
list() {
    dpkg --list
}

# Function to set IP address, DNS , gateway, and subnet mask
configure_network() {
    read -p "Enter IP Address: " Ip
    read -p "Enter Subnet Mask: " subnet_mask
    read -p "Enter Gateway: " gateway
    read -p "Enter DNS: " dns
    sudo ifconfig eth0 $Ip netmask $subnet_mask
    sudo route add default gw $gateway
    echo "nameserver $dns" | sudo tee /etc/resolv.conf
    echo "Network Configration updated Succesfully ."
}


# Function to create user 
create_user() {
    read -p "Enter Your username: " username
    read -p "Enter Your password: " password
    sudo useradd $username -m -p $password
    sudo usermod -aG sudo $username
    echo "Congrats! User $username is created, and administrator privileges are assigned to the user."
}



# Function to install Dropbox
install_dropbox() {
    sudo apt-get install dropbox
    echo "Dropbox is Installed."
}

# Display help information
display_help() {
    echo "Usage: $0 [OPTIONS]"
    echo "Options:"
    echo "  -ld      List installed applications."
    echo "  -ins     Install Dropbox."
    echo "  -ipcon   Configure IP address, mask, gateway, and DNS."
    echo "  -uc      Create a new user and assign administrator privileges to him."
    echo "  -help    Display this help message."
}



case "$1" in
    -uc)
        create_user
        ;;
    -ld)
        list
        ;;
    -ins)
        install_dropbox
        ;;
    -ipcon)
        configure_network
        ;;
    -help)
        display_help
        ;;
    *)
        echo "Invalid option. Use -help for usage information."
        ;;
esac
