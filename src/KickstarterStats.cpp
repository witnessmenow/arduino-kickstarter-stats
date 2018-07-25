/*
   Copyright (c) 2018 Brian Lough. All right reserved.

   KickstarterStats - An Arduino library to fetch stats for kickstarter projects

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */


#include "KickstarterStats.h"

KickstarterStats::KickstarterStats(Client &client)	{
	this->client = &client;
}

String KickstarterStats::makeGetRequest(String command) {
	String headers="";
	String body="";
	bool finishedHeaders = false;
	bool currentLineIsBlank = true;
	unsigned long now;
	bool avail;
	// Connect with kickstarter over ssl
	if (client->connect(KSS_HOST, portNumber)) {
		if(_debug) { Serial.println(".... connected to server"); }
		String a="";
		char c;
		int ch_count=0;
		client->println("GET " + command + " HTTP/1.1");
		client->print("HOST: ");
		client->println(KSS_HOST);
		client->println();
		now=millis();
		avail=false;
		while (millis() - now < KSS_TIMEOUT) {
			while (client->available()) {

				// Allow body to be parsed before finishing
				avail = finishedHeaders;
				char c = client->read();
				//Serial.write(c);

				if(!finishedHeaders){
					if (currentLineIsBlank && c == '\n') {
						finishedHeaders = true;
					}
					else{
						headers = headers + c;

					}
				} else {
					if (ch_count < maxMessageLength)  {
						body=body+c;
						ch_count++;
					}
				}

				if (c == '\n') {
					currentLineIsBlank = true;
				}else if (c != '\r') {
					currentLineIsBlank = false;
				}
			}
			if (avail) {
				if(_debug) {
					Serial.println("Body:");
					Serial.println(body);
					Serial.println("END");
				}
				break;
			}
		}
	}
	closeClient();
	return body;
}

bool KickstarterStats::getProjectStats(String userName, String projectName){
	String command="/projects/"+ userName + "/" + projectName + "/stats.json?v=1";
	if(_debug) { Serial.println(F("Closing client")); }
	String response = makeGetRequest(command); 
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(response);
	if(root.success()) {
		if (root.containsKey("project")) {
			long id = root["project"]["id"];
			long state_changed_at = root["project"]["state_changed_at"];
			String state = root["project"]["state"];
			int backers_count = root["project"]["backers_count"];
            long pledged = root["project"]["pledged"];
            long comments_count = root["project"]["comments_count"];

            projectStats.id = id;
            projectStats.state_changed_at = state_changed_at;
            projectStats.state = state;
			projectStats.backers_count = backers_count;
			projectStats.pledged = pledged;
			projectStats.comments_count = comments_count;

			return true;
		}
	}

	return false;
}

void KickstarterStats::closeClient() {
	if(client->connected()) {
		if(_debug) { Serial.println(F("Closing client")); }
		client->stop();
	}
}