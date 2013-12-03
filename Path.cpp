#include "Engine.h"

Path::Path()
{
	totalTime = 1.0f;
	looping = false;
	cycling = false;
	running = false;

	previousWaypointIndex = 0;
	currentWaypointIndex = 1;
	nextWaypointIndex = 2;
	nextNextWaypointIndex = 3;

	segmentTime = 0.01f;
	currentTime = t = 0.0f;

	usingLERP		= false;
	usingCatmullrom	= true;
}

Path::Path(float time, bool loop, bool cycle)
{
	totalTime = time > 0 ? time : 0;
	looping = loop;
	cycling = cycle;
	running = false;

	previousWaypointIndex = 0;
	currentWaypointIndex = 1;
	nextWaypointIndex = 2;
	nextNextWaypointIndex = 3;
	currentTime = t = 0.0f;

	usingLERP		= false;
	usingCatmullrom	= true;
}



void Path::AddWaypointToEnd(const glm::vec3 newWaypoint)
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

	previousWaypointIndex = 0;
	currentWaypointIndex = 1;
	nextWaypointIndex = 2;
	nextNextWaypointIndex = 3;

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

		if(usingLERP)
		{
			currentState = LERP(waypoints[currentWaypointIndex], waypoints[nextWaypointIndex], t);
		}
		else if(usingCatmullrom)
		{
			currentState.x = CatmullRom(waypoints[previousWaypointIndex].x, waypoints[currentWaypointIndex].x, waypoints[nextWaypointIndex].x, waypoints[nextNextWaypointIndex].x, t);
			currentState.y = CatmullRom(waypoints[previousWaypointIndex].y, waypoints[currentWaypointIndex].y, waypoints[nextWaypointIndex].y, waypoints[nextNextWaypointIndex].y, t);
			currentState.z = CatmullRom(waypoints[previousWaypointIndex].z, waypoints[currentWaypointIndex].z, waypoints[nextWaypointIndex].z, waypoints[nextNextWaypointIndex].z, t);
		}
	}
}


void Path::NextWaypoint()
{
	currentTime = currentTime - segmentTime;

	previousWaypointIndex = currentWaypointIndex;
	currentWaypointIndex = nextWaypointIndex;
	nextWaypointIndex = nextNextWaypointIndex;
	
	if(currentWaypointIndex < (waypoints.size()-2))
		nextNextWaypointIndex+=1;
	else if(cycling)
		nextNextWaypointIndex = 0;
	else if(looping)
	{
		//This should be used for something but I am not really sure what
		//currentWaypointIndex	= 0;
		nextNextWaypointIndex	= 0;
	}
	else
	{
		//Its done, make computer explode
	}

}
