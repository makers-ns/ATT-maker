#!/usr/bin/env python3

# Copyright 2017 AllThingsTalk

#
# AllThingsTalk Counter example
#

from time import sleep

from allthingstalk import Client, Device, IntegerAsset

# Parameters used to authorize and identify your device
# Get them on maker.allthingstalk.com
DEVICE_TOKEN = 'maker:4J26u3qy2LrnG0lqFxhPOTExaK1WiTBAQW2cjlC'
DEVICE_ID = '7lNNbZqb4QCMVDkDxzUhZkdx'

class Counter(Device):
    counter = IntegerAsset()

client = Client(DEVICE_TOKEN)
device = Counter(client=client, id=DEVICE_ID)

for i in range(10):
    device.counter = i
    sleep(1)
