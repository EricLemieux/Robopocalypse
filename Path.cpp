#include "Engine.h"

Path::Path()
{
	totalTime = 1.0f;
	looping = true;
	cycling = false;
	running = false;

	currentWaypointIndex = 0;
	nextWaypointIndex = 1;
	currentTime = t = 0.0f;
}

Path::Path(float time, bool loop, bool cycle)
{
	totalTime = time > 0 ? time : 0;
	looping = loop;
	cycling = cycle;
	running = false;

	currentWaypointIndex = 0;
	nextWaypointIndex = 1;
	currentTime = t = 0.0f;
}



void Path::AddWaypointToEnd(const sf::Vector2f newWaypoint)
{
	if(!running)
		waypoints.push_back(newWaypoint);
}

void Path::RemoveLastWaypoint()
{
	if(!running)
		waypoints.pop_back();
}

void Path::RemoveAllWaypoints()
{
	if(!running)
		waypoints.clear();
}


void Path::SetTotalTime(float time)
{
	if(!running && t > 0.0f)
	{
		totalTime = time;

		unsigned int numSegments = 
			cycling ? (waypoints.size()) : (waypoints.size() - 1);

		totalTime = totalTime / (float)numSegments;
	}
}


void Path::Reset()
{
	running = false;

	currentWaypointIndex = 0;
	nextWaypointIndex = 1;
	currentTime = t = 0.0f;

	if(waypoints.size())
		currentState = waypoints[0];
}


void Path::Update(float dt)
{
	if(running)
	{
		currentTime += dt;

		if(currentTime > segmentTime)
		{
			NextWaypoint();
		}

		t = currentTime / segmentTime;	//[0,1]

		currentState = LERP(waypoints[currentWaypointIndex], waypoints[nextWaypointIndex], t);
	}
}


void Path::NextWaypoint()
{
	currentTime = currentTime - segmentTime;

	currentWaypointIndex = nextWaypointIndex;
	
	if(currentWaypointIndex < (waypoints.size()-1))
		nextWaypointIndex+=1;
	else if(cycling)
		nextWaypointIndex = 0;
	else if(looping)
	{
		currentWaypointIndex	= 0;
		nextWaypointIndex		= 1;
	}
	else
	{
		//Its done, make computer explode
	}

}
