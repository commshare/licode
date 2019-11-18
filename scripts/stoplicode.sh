#!/bin/sh
sudo killall node
brew services restart rabbitmq
brew services restart mongodb-community@4.0

