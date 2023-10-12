import paho.mqtt.publish as publish
import sys

publish.single("varaha/final", sys.argv[1], hostname="mqtt.eclipseprojects.io")
