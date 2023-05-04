#!/bin/bash

# Read the list of users

while [[ true ]]; do
    read -p "Enter user name: " user
    if [[ -z "$user" ]]; then
        break
    fi

    lastLogins=$(last | grep "$user" | head -n3)
    if [[ -z "$lastLogins" ]]; then
            echo "Utilizatorul $user nu s-a conectat"
        else
            echo "$lastLogins"
    fi

done

