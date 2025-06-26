#!/usr/bin/bash

VERSION_CODENAME=$(grep -oP '(?<=^VERSION_CODENAME=).+' /etc/os-release | tr -d '"')
echo "Detected VERSION_CODENAME: $VERSION_CODENAME"

# Add ubuntu repository
sudo add-apt-repository -y "deb [arch=amd64] http://archive.ubuntu.com/ubuntu $VERSION_CODENAME \
        main universe"

# Install dependencies
sudo apt-get -y install make gcc libudev-dev devscripts --no-upgrade --no-install-recommends --no-install-suggests

# Install test specific
if [[ "$1" == "musl" ]]; then
        sudo apt-get -y install make gcc musl musl-dev musl-tools build-essential linux-headers-generic
else
        sudo apt-get -y install gcc-$1 --no-upgrade --no-install-recommends --no-install-suggests
fi
