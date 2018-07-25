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


#ifndef KickstarterStats_h
#define KickstarterStats_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define KSS_HOST "www.kickstarter.com"
#define KSS_TIMEOUT 1500


struct ProjectStats{
  long id;
  long state_changed_at;
  String state;
  int backers_count;
  long pledged;
  long comments_count;
};

class KickstarterStats
{
  public:
    KickstarterStats (Client &client);
    String makeGetRequest(String command);
    bool getProjectStats(String userName, String projectName);
    ProjectStats projectStats;
    int portNumber = 443;
    bool _debug = false;

  private:
    Client *client;
    const int maxMessageLength = 1000;
    bool checkForOkResponse(String response);
    void closeClient();
};

#endif