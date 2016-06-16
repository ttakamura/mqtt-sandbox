# coding: utf-8
require "mqtt"

user = ENV['MQTT_USER']

options = {
  :host => 'std1.mqtt.shiguredo.jp',
  :port => 1883,
  :clean_session => true,
  :username => user,
  :password => ENV['MQTT_PASS'],
  :ssl => false
}

MQTT::Client.connect(options) do |client|
  topic = "#{user}/a/b/c"
  client.publish(topic, "From Ruby!!", retain: true)
end
