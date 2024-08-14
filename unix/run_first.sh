#!/bin/bash

# Define variables
NETWORK="info"
IP_ADDRESS="172.18.0.11"
PORT="7000"
CONTAINERNAME="chatuser1"
IMAGENAME="localchat"

# Run Docker container
docker run -it --rm --name "$CONTAINERNAME" --network "$NETWORK" --ip "$IP_ADDRESS" --env IP_ADDRESS="$IP_ADDRESS" --env PORT="$PORT" "$IMAGENAME"
