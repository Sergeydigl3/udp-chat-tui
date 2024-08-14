#!/bin/bash

containers=("chatuser1" "chatuser2")

for container in "${containers[@]}"; do
    docker stop "$container" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo "Stopped container: $container"
    else
        echo "Container $container not found or already stopped."
    fi
done
