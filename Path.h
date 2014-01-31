
#ifndef _PATH_H
#define _PATH_H



struct ParaTable{
	std::vector<float> arc_length;
	std::vector<float> u;
	float ustep;
	
};


class Path
{
public: 

	// ctors
	Path();
	Path(float time, bool loop, bool cycle);


	inline void SetLooping(bool loop) { looping = loop; }
	inline void SetCycling(bool cycle) { cycling = cycle; }

	// get properties
	inline bool GetLooping() const { return looping; }
	inline bool GetCycling() const { return cycling; }

	// add and remove waypoints
	void AddWaypointToEnd(const glm::vec3 newWaypoint);
	void RemoveLastWaypoint();
	void RemoveAllWaypoints();


	void SetTotalTime(float time);

	// start and stop animating along the path
	inline void Start() { running = true; }
	inline void Stop() { running = false; }

	// stop go back to start of path
	void Reset();

	// actually update over time!
	float Update(float dt);

	// **** get our current state!!!!
	inline glm::vec3 GetCurrentState(){ return currentState;}

	void genCatmullTable();


private: 

	// function to increment the waypoint properly
	void NextWaypoint();



	float current_arc_length;
	ParaTable catmull_table;
	glm::vec2 speed_profile[4];
	float totalT;
	float ut;
	float currentTimeCmR;
	
	// **** need something to store our waypoints
	std::vector<glm::vec3> waypoints;
	glm::vec3 prevState;//for speed control


	unsigned int previousWaypointIndex;
	unsigned int currentWaypointIndex;
	unsigned int nextWaypointIndex;
	unsigned int nextNextWaypointIndex;

	float segmentTime;

	float currentTime;

	float t;
	

	// **** what is our current state?
	glm::vec3 currentState;


	float totalTime;
	//if continue running after hitting end
	bool looping;
	//if first node is connected to last node
	bool cycling;

	bool running;

	bool usingLERP;
	bool usingCatmullrom;

};


#endif	// _PATH_H
