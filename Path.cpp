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
	currentTimeCmR = 0.f;

	usingLERP		= false;
	usingCatmullrom	= true;
	current_arc_length = 0;
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
	currentTimeCmR = 0.f;

	usingLERP		= false;
	usingCatmullrom	= true;
	current_arc_length = 0;
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
	currentTimeCmR = 0.f;

	if(waypoints.size())
		currentState = waypoints[0];
}


float Path::Update(float dt)
{
	if(running)
	{
		currentTime += dt;
		//speed control
		currentTimeCmR += dt;

		if(currentTime > segmentTime)
		{
			NextWaypoint();
		}
		
		t = currentTime / segmentTime;	//[0,1]
		//speed control
		ut = currentTimeCmR / totalT;
		
		//std::cout<<totalT<<std::endl;


		if(usingLERP)
		{
			currentState = LERP(waypoints[currentWaypointIndex], waypoints[nextWaypointIndex], t);
		}
		else if(usingCatmullrom)
		{
			//find how far we should be along the curve by arc length
			current_arc_length = catmull_table.arc_length[catmull_table.arc_length.size()-1] * Bezier(speed_profile[0],speed_profile[1],speed_profile[2],speed_profile[3],ut).y;
			int i = 0;
			float tempu;

			//find the index number nearest to current arc length
			while (current_arc_length > catmull_table.arc_length[i]){
				++i;
			}
			
			//increment way points if necessary, find the u value at index
			if( i != 0){
				int j = i/10;
				while (j > 0){
					NextWaypoint();
					--j;
				}
				tempu = catmull_table.u[i%10];
			} else {
				tempu = 0;
			}
			
			//find u value if current distance doesn't match something in table
			if(current_arc_length != catmull_table.arc_length[i]){
				tempu = LERP(catmull_table.u[(i%10)-1],catmull_table.u[(i%10)],(current_arc_length - catmull_table.arc_length[i])/(catmull_table.arc_length[i]-catmull_table.arc_length[i-1]));
			}

			currentState = CatmullRom(waypoints[previousWaypointIndex], waypoints[currentWaypointIndex], waypoints[nextWaypointIndex], waypoints[nextNextWaypointIndex], tempu);
		}
		return 0;
	}
	return 2;
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
		nextNextWaypointIndex	= 0;
	}
	else
	{
		this->Stop();
	}

}

//for speed control along curve
void Path::genCatmullTable(){
	catmull_table.arc_length.clear();
	catmull_table.arc_length.push_back(0.f);
	catmull_table.u.clear();
	catmull_table.ustep = 0.1f;
	current_arc_length = 0.f;

	speed_profile[0] = glm::vec2(0,0);
	speed_profile[1] = glm::vec2(5,0);
	speed_profile[2] = glm::vec2(1,0);
	speed_profile[3] = glm::vec2(1,1);

	std::vector<glm::vec3> temp_catmullpoints;

	//fill u intervals
	for(int i = 0; i<10;++i){
		catmull_table.u.push_back((float)catmull_table.ustep * i);
	}

	//find points along curve
	for(int i = 1, size = waypoints.size()-2; i<size; ++i){
		temp_catmullpoints.push_back(waypoints[i-1]);
		for(int j = 1; j < 10; ++j){
			
			temp_catmullpoints.push_back(CatmullRom(waypoints[i-1],waypoints[i],waypoints[i+1],waypoints[i+2],catmull_table.u[j]));
		}
		
	}
	temp_catmullpoints.push_back(waypoints[waypoints.size()-2]);
	for(int i = 1; i < 10; ++i){
		
		temp_catmullpoints.push_back(CatmullRom(waypoints[waypoints.size()-3],waypoints[waypoints.size()-2],waypoints[waypoints.size()-1],waypoints[waypoints.size()-1],catmull_table.u[i]));
	}
	//calculate arc length between points
	for(int i = 1, size = temp_catmullpoints.size(); i < size; ++i){
		float temp_arclength = abs(glm::l2Norm(temp_catmullpoints[i]-temp_catmullpoints[i-1]));
		catmull_table.arc_length.push_back(temp_arclength+catmull_table.arc_length[i-1]);
	}
	//time it should take to run the curve
	totalT = catmull_table.arc_length[catmull_table.arc_length.size()-1]/200;
}