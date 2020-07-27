#!/bin/bash

trap "killall sleep; exit" SIGINT

timeout=15

echo "Sleeping for $timeout seconds..."
sleep $timeout
