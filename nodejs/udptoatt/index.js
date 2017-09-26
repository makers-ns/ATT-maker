'use strict'

var dgram = require('dgram');
var mqtt = require('mqtt');

var topic = 'asset/[AssetId]/state';

var mqttClient = mqtt.connect('http://api.allthingstalk.io', {
    username: "[DeviceToken]",
    password: "DummyPassword"
})

var temp = 0;

mqttClient.on('connect', function () {
    console.log('MQTT Connected');

})

var server = dgram.createSocket('udp4');

server.on('message', (msg, rinfo) => {
    temp = (msg[0] + msg[1]*256) / 1000.0;
    mqttClient.publish(topic, '{"value" : ' + temp.toString() + '}' );
    console.log(`Temperatura ${temp}`);
});

server.on('listening', () => {
  const address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
});

server.bind(6666);
