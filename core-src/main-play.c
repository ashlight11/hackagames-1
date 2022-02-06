#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <mosquitto.h>

#define mqtt_host "localhost"
#define mqtt_port 1883

void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
	printf("connect callback, rc=%d\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	bool match = 0;
	printf("got message '%.*s' for topic '%s'\n", message->payloadlen, (char*) message->payload, message->topic);

	mosquitto_topic_matches_sub("/devices/wb-adc/controls/+", message->topic, &match);
	if (match) {
		printf("got message for ADC topic\n");
	}

}

int main(int argc, char *argv[])
{
	struct mosquitto *mosq;
	int rc = 0;
	
	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, true, NULL);

	if(mosq){
		mosquitto_connect_callback_set(mosq, connect_callback);
		mosquitto_message_callback_set(mosq, message_callback);

	    rc = mosquitto_connect(mosq, "localhost", 2014, 60);

		mosquitto_subscribe(mosq, NULL, "/hackagames", 0);

		while(1){
			rc = mosquitto_loop(mosq, -1, 1);
			if(rc){
				printf("connection error!\n");
				sleep(10);
				mosquitto_reconnect(mosq);
			}
		}
		mosquitto_destroy(mosq);
	}

	mosquitto_lib_cleanup();

	return rc;
}